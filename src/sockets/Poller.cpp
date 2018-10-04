
#include <sockets/Poller.h>
#include <sockets/GlobalDefinitions.h>
#include <Log.h>
#include <algorithm>
#include <vector>
#include <unistd.h>

Poller::Poller() 
: timeoutms_(100) {
    pthread_mutex_init(&mutex_, NULL);
    pollset_.clear();
    start();
}

Poller::~Poller() throw(){
    stop();
    pollset_.clear();
    pthread_mutex_destroy(&mutex_);
}

bool Poller::add_fd (int fd, IEvents* events) {
    
    pthread_mutex_lock(&mutex_);
    
    pollfd pfd = {fd, 0, 0};
    pollset_.push_back(pfd);
    
    PoolData data;
    data.events = events;
    data.index = pollset_.size() - 1;
    
    dataMap_.insert( std::pair<int, PoolData>(fd, data) );
    
    LOG->LogMessage(LOG_INFO,"Poller::add_fd() fd %d added to pool. Pool size %d", fd, pollset_.size());
    
    pthread_mutex_unlock(&mutex_);
    return true;
}

bool Poller::rm_fd (int fd)
{
    pthread_mutex_lock(&mutex_);
    
    data_map_t::iterator it = dataMap_.find( fd );
    if (it == dataMap_.end()){
        pthread_mutex_unlock(&mutex_);
        return false;
    }

    int removedIndex = it->second.index;

    //Borramos las referencias al fd removido
    dataMap_.erase(it);
    pollset_.erase (pollset_.begin() + removedIndex);    

    //Acomodamos las estructuras
    for (pollset_t::size_type i = removedIndex; i < pollset_.size(); i++) {
        dataMap_[ pollset_ [i].fd ].index = i;
    }
    
    LOG->LogMessage(LOG_INFO,"Poller::rm_fd() fd %d removed from pool. Pool size %d", fd, pollset_.size());
    
    pthread_mutex_unlock(&mutex_);
    return true;
}

void Poller::set_pollin( int fd )
{
    pthread_mutex_lock(&mutex_);
    data_map_t::iterator it = dataMap_.find( fd );
    if (it != dataMap_.end())
        pollset_[ it->second.index ].events |= POLLIN;
    pthread_mutex_unlock(&mutex_);
}

void Poller::reset_pollin( int fd )
{
    pthread_mutex_lock(&mutex_);
    data_map_t::iterator it = dataMap_.find( fd );
    if (it != dataMap_.end())
        pollset_[ it->second.index ].events &= ~((short) POLLIN);
    pthread_mutex_unlock(&mutex_);
}

void Poller::set_pollout( int fd )
{
    pthread_mutex_lock(&mutex_);
    data_map_t::iterator it = dataMap_.find( fd );
    if (it != dataMap_.end())
        pollset_[ it->second.index ].events |= POLLOUT;
    pthread_mutex_unlock(&mutex_);
}

void Poller::reset_pollout( int fd )
{
    pthread_mutex_lock(&mutex_);
    data_map_t::iterator it = dataMap_.find( fd );
    if (it != dataMap_.end())
        pollset_[ it->second.index ].events &= ~((short) POLLOUT);
    pthread_mutex_unlock(&mutex_);
}

void Poller::setTimeout( unsigned int timeoutms ) { 
    pthread_mutex_lock(&mutex_);         
    timeoutms_ = timeoutms;              
    pthread_mutex_unlock(&mutex_);    
}

unsigned int Poller::getTimeout() { 
    return timeoutms_;
}

void Poller::run() {
    
    while (!m_canceled){

        pthread_mutex_lock(&mutex_);
                
        if (!pollset_.size()) {
            pthread_mutex_unlock(&mutex_); 
            sleep(1);
            continue;
        }
        
        //Hacemos copias de todo y trabajamos con ellas, liberando al poolset y el map
        pollset_t pollsetCopy( pollset_.size() ); 
        std::copy ( pollset_.begin(), pollset_.end(), pollsetCopy.begin() );
        //data_map_t dataMapCopy = dataMap_;
        unsigned int timeoutmsCopy = timeoutms_ ? timeoutms_ : -1;
        
        pthread_mutex_unlock(&mutex_); 
        
        //  Wait for events.
        int rc = poll (&pollsetCopy[0], pollsetCopy.size(), timeoutmsCopy); 

        if ((rc == -1 /* pool error */) || (rc == 0 /* timeout */)) {
            continue;
        }

        for (pollset_t::size_type i = 0; i < pollsetCopy.size(); i++) {

            if (pollsetCopy[i].fd == 0){
                LOG->LogMessage(LOG_WARNING,"Poller::run() INVALID FD!!! (original %d)", pollset_[i].fd);
                continue;
            }
            
            if (pollsetCopy[i].revents != 0) {

                pthread_mutex_lock(&mutex_); 
                data_map_t::iterator it = dataMap_.find( pollsetCopy[i].fd );
                if (it == dataMap_.end()){
                    pthread_mutex_unlock(&mutex_); 
                    continue;
                } else {
                    
                    LOG->LogMessage(LOG_WARNING,"Poller::run() Leyendo FD %d!", pollset_[i].fd);
                    
                    IEvents* events = it->second.events;
                    pthread_mutex_unlock(&mutex_); 
                //}
                
                //if (it != dataMap_.end()){
                    if (pollsetCopy[i].revents & (POLLERR | POLLNVAL)) {
                        events->onError_event(); 
                        continue;
                    }

                    if (pollsetCopy[i].revents & POLLHUP) {
                        events->onHangUp_event();                        
                        continue;
                    }

                    if (pollsetCopy[i].revents & POLLOUT) {
                        if (events->onWrite_event() != SocketReturnCode_OK){
                             events->onHangUp_event();       
                             continue;
                        }
                    }

                    if (pollsetCopy[i].revents & POLLIN) {
                        SocketReturnCode result = events->onRead_event();
                        if ((result != SocketReturnCode_OK) && (result != SocketReturnCode_InvalidData)){
                             events->onHangUp_event();   
                             continue;
                        }
                    }
                }
            }
        }
    }
}

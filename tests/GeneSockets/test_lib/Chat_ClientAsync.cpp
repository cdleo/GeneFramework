/* 
 * File:   Chat_ClientAsync.cpp
 * Author: cleo
 * 
 * Created on October 8, 2014, 11:32 AM
 */

#include <sockets/Poller.h>
#include <Chat_ClientAsync.h>

Chat_ClientAsync::Chat_ClientAsync()
: PeerINET_Async(), outstream_(""), pendingMsg(NULL) {
    pthread_mutex_init(&readMutex_, NULL);
    pthread_mutex_init(&writeMutex_, NULL);
}

Chat_ClientAsync::~Chat_ClientAsync(){
    pthread_mutex_destroy(&readMutex_);
    pthread_mutex_destroy(&writeMutex_);
}

bool Chat_ClientAsync::connectToHost(const char* ip, const int port, const int timeoutSeconds){
    if (PeerINET_Async::connectToHost(ip,port,timeoutSeconds)){
        return initialize();
    }
    return false;  
}

int Chat_ClientAsync::setFD(int fd){
    if (PeerINET_Async::setFD(fd)){
        return initialize();
    }
    return false;
}

bool Chat_ClientAsync::initialize() {
    if (!POLLER->add_fd( getFD(), this ))
        return false;
    
    POLLER->set_pollin( getFD() );
    return true;
}

ssize_t Chat_ClientAsync::read(Chat_Msg*& msg, const int timeoutSeconds) {
    pthread_mutex_lock(&readMutex_);
    
    if (instream_.empty()) {
        pthread_mutex_unlock(&readMutex_);
        return 0;     
    }
    
    msg = instream_.front();
    instream_.pop();
    pthread_mutex_unlock(&readMutex_);
    return 1; 
}

ssize_t Chat_ClientAsync::write(Chat_Msg* msg, const int timeoutSeconds) {
    pthread_mutex_lock(&writeMutex_);
    outstream_.append( msg->toStream() );
    POLLER->set_pollout( getFD() );
    pthread_mutex_unlock(&writeMutex_);
    return 1;
}

SocketReturnCode Chat_ClientAsync::onRead_event(){
    pthread_mutex_lock(&readMutex_);
    
    Chat_Msg* msg = new Chat_Msg();
    SocketReturnCode rc = PeerINET_Async::gRead(msg->getHeader()->getData(), msg->getHeader()->getMaxSize());
    if (rc <= 0) {
        pthread_mutex_unlock(&readMutex_);
        return rc;
    }
        
    msg->getHeader()->decode();
    
    rc = PeerINET_Async::gRead(msg->getBody()->getData(), msg->getBodyLength());
    if (rc <= 0) {
        pthread_mutex_unlock(&readMutex_);
        return rc;
    }
    
    msg->getBody()->decode();
    
    instream_.push(msg);
    
    pthread_mutex_unlock(&readMutex_);
    return rc;   
    
    /*SocketReturnCode rc;
    if (pendingMsg == NULL) {
        pendingMsg = new Chat_Msg();

        rc = PeerINET_Async::peek(pendingMsg->getHeader()->getData(), pendingMsg->getHeader()->getMaxSize());
        if (rc <= 0) {
            delete pendingMsg;
            pendingMsg = NULL;
            pthread_mutex_unlock(&readMutex_);
            return rc;
        }

        rc = PeerINET_Async::read(pendingMsg->getHeader()->getData(), pendingMsg->getHeader()->getMaxSize());
        if (rc <= 0) {
            delete pendingMsg;
            pendingMsg = NULL;
            pthread_mutex_unlock(&readMutex_);
            return rc;
        }

        pendingMsg->getHeader()->decode();
    }
    
    rc = PeerINET_Async::peek(pendingMsg->getBody()->getData(), pendingMsg->getBodyLength());
    if (rc <= 0) {
        pthread_mutex_unlock(&readMutex_);
        return rc;
    }
    
    rc = PeerINET_Async::read(pendingMsg->getBody()->getData(), pendingMsg->getBodyLength());
    if (rc <= 0) {
        delete pendingMsg;
        pendingMsg = NULL;
        pthread_mutex_unlock(&readMutex_);
        return rc;
    }
    
    pendingMsg->getBody()->decode();
    
    instream_.push(pendingMsg);
    pendingMsg = NULL;  //No lo borramos
    
    pthread_mutex_unlock(&readMutex_);
    return rc;  
    */  
}

SocketReturnCode Chat_ClientAsync::onWrite_event(){
    pthread_mutex_lock(&writeMutex_);
    SocketReturnCode result = PeerINET_Async::gWrite(outstream_.c_str(), outstream_.length());
    outstream_.clear();
    POLLER->reset_pollout( getFD() );
    pthread_mutex_unlock(&writeMutex_);
    return result;
}

void Chat_ClientAsync::onError_event(){
    POLLER->rm_fd( getFD() );
    disconnect();
}
void Chat_ClientAsync::onHangUp_event(){
    POLLER->rm_fd( getFD() );
    disconnect();
}
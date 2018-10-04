
#include <sockets/PeerUnix.h>

#include <fcntl.h>

PeerUnix::PeerUnix() {
}

PeerUnix::~PeerUnix(){
}

SocketReturnCode PeerUnix::gRead(void* buff, const size_t buffSize, const int timeoutms){

    pthread_mutex_lock (&mutex_);

    pollfd pfd = {fd_, POLLIN | POLLHUP | POLLERR, 0};
    poll(&pfd, 1, timeoutms);
    
    if(pfd.revents == 0) {
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_TimedOut; //timedout    
    }
    
    if(pfd.revents != POLLIN) {    
        LOG->LogMessage(LOG_WARNING, "PeerUnix::read() Error! revents = %d", pfd.revents);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    ssize_t result = read(pfd.fd, buff, buffSize);
    if (result != (ssize_t) buffSize) {
        logError(errno);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    pthread_mutex_unlock (&mutex_);
    return SocketReturnCode_OK;
}

SocketReturnCode PeerUnix::gWrite(const void* buff, const size_t buffSize, const int timeoutms){
    
    pthread_mutex_lock (&mutex_);

    pollfd pfd = {fd_, POLLOUT | POLLHUP | POLLERR, 0};
    poll(&pfd, 1, timeoutms);
    
    if(pfd.revents == 0) {
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_TimedOut; //timedout    
    }

    if(pfd.revents != POLLOUT) {
        LOG->LogMessage(LOG_WARNING, "PeerUnix::write() Error! revents = %d", pfd.revents);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    ssize_t result = write(pfd.fd, buff, buffSize);
    if (result != (ssize_t) buffSize) {
        logError(errno, false);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    pthread_mutex_unlock (&mutex_);
    return SocketReturnCode_OK;    
}



#include <sockets/PeerINET.h>

#include <fcntl.h>

#include "sockets/Poller.h"

PeerINET::PeerINET(){
}

SocketReturnCode PeerINET::gRead(void* buff, const size_t buffSize, const int timeoutms){

    pthread_mutex_lock (&mutex_);

    pollfd pfd = {fd_, POLLIN, 0};
    poll(&pfd, 1, timeoutms);
    
    if(pfd.revents == 0) {
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_TimedOut; //timedout    
    }
    
    if(pfd.revents != POLLIN) {
        LOG->LogMessage(LOG_WARNING, "PeerINET::read() Error! revents = %d", pfd.revents);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    //ssize_t result = recv(poolfd_.fd, buff, buffSize, MSG_WAITALL);
    socklen_t sockSize = sizeof(addr_);
    size_t result = recvfrom(pfd.fd, buff, buffSize, MSG_WAITALL, (struct sockaddr*) &addr_, &sockSize);
    if (result != buffSize) {
        logError(errno);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    pthread_mutex_unlock (&mutex_);
    return SocketReturnCode_OK;
}

SocketReturnCode PeerINET::gPeek(void* buff, const size_t buffSize, const int timeoutms){

    pthread_mutex_lock (&mutex_);
    pollfd pfd = {fd_, POLLIN, 0};
    pfd.events = POLLIN;
    poll(&pfd, 1, timeoutms);
    
    if(pfd.revents == 0) {
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_TimedOut; //timedout    
    } 
    
    if(pfd.revents != POLLIN) {
        LOG->LogMessage(LOG_WARNING, "PeerINET::peek() Error! revents = %d", pfd.revents);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    //ssize_t result = recv(poolfd_.fd, buff, buffSize, MSG_PEEK);
    socklen_t sockSize = sizeof(addr_);
    size_t result = recvfrom(pfd.fd, buff, buffSize, MSG_PEEK, (struct sockaddr*) &addr_, &sockSize);
    if (result != buffSize) {
        logError(errno);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    pthread_mutex_unlock (&mutex_);
    return SocketReturnCode_OK;
}

SocketReturnCode PeerINET::gWrite(const void* buff, const size_t buffSize, const int timeoutms){
    
    pthread_mutex_lock (&mutex_);
    pollfd pfd = {fd_, POLLOUT, 0};
    poll(&pfd, 1, timeoutms);
    
    if(pfd.revents == 0) {
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_TimedOut; //timedout    
    }
      
    if(pfd.revents != POLLOUT) {
        LOG->LogMessage(LOG_WARNING, "PeerINET::write() Error! revents = %d", pfd.revents);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    //ssize_t result = send(poolfd_.fd, (void*) buff, buffSize, 0);
    size_t result = sendto(pfd.fd, buff, buffSize, 0, (struct sockaddr*) &addr_, (socklen_t) sizeof(addr_));
    if (result != buffSize) {
        logError(errno, false);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    pthread_mutex_unlock (&mutex_);
    return SocketReturnCode_OK;    
}


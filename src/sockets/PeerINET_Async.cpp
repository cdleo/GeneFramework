
#include <sockets/PeerINET_Async.h>

#include <fcntl.h>

PeerINET_Async::PeerINET_Async(){
}

SocketReturnCode PeerINET_Async::gRead(void* buff, const size_t buffSize){

    //ssize_t result = recv(poolfd_.fd, buff, buffSize, MSG_WAITALL);
    if (!isConnected()) return SocketReturnCode_Error;
    
    socklen_t sockSize = sizeof(addr_);
    size_t result = recvfrom(fd_, buff, buffSize, MSG_WAITALL, (struct sockaddr*) &addr_, &sockSize);
    if (result != buffSize) {
        LOG->LogMessage(LOG_WARNING, "PeerINET_Async::read() Invalid data amount from socket %d. Expected %d, Received %d", fd_, buffSize, result );
        logError(errno);
        return SocketReturnCode_Error;
    }
    
    return SocketReturnCode_OK;
}

SocketReturnCode PeerINET_Async::gPeek(void* buff, const size_t buffSize){

    if (!isConnected()) return SocketReturnCode_Error;
    
    //ssize_t result = recv(poolfd_.fd, buff, buffSize, MSG_PEEK);
    socklen_t sockSize = sizeof(addr_);
    size_t result = recvfrom(fd_, buff, buffSize, MSG_PEEK, (struct sockaddr*) &addr_, &sockSize);
    if (result != buffSize) {
        LOG->LogMessage(LOG_WARNING, "PeerINET_Async::peek() Invalid data amount from socket %d. Expected %d, Received %d", fd_, buffSize, result );
        logError(errno);
        return SocketReturnCode_NoEnoughData;
    }
    
    return SocketReturnCode_OK;
}

SocketReturnCode PeerINET_Async::gWrite(const void* buff, const size_t buffSize){
    
    if (!isConnected()) return SocketReturnCode_Error;
    
    //ssize_t result = send(poolfd_.fd, (void*) buff, buffSize, 0);
    size_t result = sendto(fd_, buff, buffSize, 0, (struct sockaddr*) &addr_, (socklen_t) sizeof(addr_));
    if (result != buffSize) {
        LOG->LogMessage(LOG_WARNING, "PeerINET_Async::write() Invalid data amount from socket %d. Expected %d, Received %d", fd_, buffSize, result );
        logError(errno, false);
        return SocketReturnCode_Error;
    }
    
    return SocketReturnCode_OK;    
}



#include <sockets/Peer.h>
#include <sockets/GlobalDefinitions.h>

#include <fcntl.h>

Peer::Peer()
: fd_(0) {
    pthread_mutex_init(&mutex_, NULL);
}

Peer::~Peer(){
    disconnect();
    pthread_mutex_destroy(&mutex_);
}

void Peer::disconnect(){
    ScopedLock lock(mutex_);
    if (fd_ != 0) {
        close(fd_);
        fd_ = 0;
    }
}   

int Peer::set_option (int level, int option, void *optval, int optlen) {

    if (!isConnected())
        return SOCKET_ERROR_CODE;
    
    return setsockopt(fd_, level, option, (char *) optval, optlen);
}

int Peer::get_option (int level,int option,void *optval, int *optlen) {
  
    if (!isConnected())
        return SOCKET_ERROR_CODE;    
    
    return getsockopt(fd_, level, option, (char *) optval, (socklen_t*) optlen);
}

void Peer::logError(int errorNumber, bool inRecvOperation) {
    switch (errorNumber){
        case EBADF:  
            LOG->WARNING("Peer::ERROR: The s file descriptor is invalid.");
            break;
            
        case EINVAL:
            if (inRecvOperation)
                LOG->WARNING("Peer::ERROR: The MSG_OOB flag is set and no out-of-band data is available.");
            else
                LOG->WARNING("Peer::ERROR: The len argument overflows a ssize_t.");
            break;
            
        case EINTR:
            LOG->WARNING("Peer::ERROR: The operation is interrupted by the delivery of a signal before any data is available to be received.");
            break;
            
        case EIO:
            LOG->WARNING("Peer::ERROR: An I/O error occurs while reading from or writing to the file system.");
            break;
            
        case ENOMEM:
            LOG->WARNING("Peer::ERROR: Insufficient user memory is available  to complete operation.");
            break;
            
        case ENOSR:
            LOG->WARNING("Peer::ERROR: Insufficient STREAMS resources are available for the operation to complete.");
            break;
            
        case ENOTSOCK:
            LOG->WARNING("Peer::ERROR: s is not a socket.");
            break;
            
        case ESTALE:
            LOG->WARNING("Peer::ERROR: A stale NFS file handle exists.");
            break;
            
        case EWOULDBLOCK:
            LOG->WARNING("Peer::ERROR: The socket is marked non-blocking and the requested operation would block.");
            break;
            
        case ECONNREFUSED:
            LOG->WARNING("Peer::ERROR: The requested connection was refused by the peer.");
            break;
     
        case EMSGSIZE:
            LOG->WARNING("Peer::ERROR: The socket requires that the message be sent atomically and the message is too long.");
            break;
            
        default:
            LOG->LogMessage(LOG_WARNING,"Peer::ERROR: Codigo no reconocido (%d)",errorNumber);
            
    }
}

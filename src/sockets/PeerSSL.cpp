
#include <sockets/PeerSSL.h>
#include <sockets/GlobalDefinitions.h>

#include <fcntl.h>

bool PeerSSL::sslInitialized_ = false;

PeerSSL::PeerSSL(const char* certificateFilePath, const char* keyFilePath) 
: ssl_(NULL), certificate_(certificateFilePath), key_(keyFilePath) {
    initializeSSL();    
}

bool PeerSSL::connectToHost(const char* host, const int port, const int timeoutms){
    
    if ( PeerIN::connectToHost( host, port, timeoutms ) ) {

        SSL_CTX* sslctx = initCTX( SSLv23_client_method() );
        ssl_ = SSL_new(sslctx);        
        SSL_set_fd(ssl_, fd_ );           /* attach the socket descriptor */

        int ssl_err = SSL_connect(ssl_);
        if ( ssl_err != 1 ) {   /* perform the connn */
            LOG->LogMessage(LOG_WARNING,"PeerSSL::connectToHost() SSL Error: %d", SSL_get_error( ssl_, ssl_err));   
            ShutdownSSL();
        } else {  
            SSL_set_connect_state(ssl_);
            LOG->LogMessage(LOG_INFO, "PeerSSL::connectToHost() Connected to %s:%d with %s encryption", host, port, SSL_get_cipher(ssl_) );
       }        
        
    }
    
    return isConnected();
}

int PeerSSL::setFD(int fd) {
    
    int result = PeerIN::setFD( fd );
    if (result == SOCKET_OK_CODE) {
    
        SSL_CTX* sslctx = initCTX( SSLv23_server_method() );
        ssl_ = SSL_new(sslctx);        
        SSL_set_fd(ssl_, fd_ );       /* attach the socket descriptor */

        //Here is the SSL Accept portion.  Now all reads and writes must use SSL
        int ssl_err = SSL_accept(ssl_);
        if ( ssl_err != 1 )  {
            LOG->LogMessage(LOG_WARNING,"PeerSSL::setFD() SSL Error: %s", SSL_get_error( ssl_, ssl_err));   
            ShutdownSSL();  //log and close down ssl
            disconnect();
            result = SOCKET_ERROR_CODE;        
        } else {
            SSL_set_accept_state(ssl_);
            LOG->LogMessage(LOG_INFO, "PeerSSL::setFD() Connected to %s:%d with %s encryption", getRemoteIP().c_str(), getRemotePort(), SSL_get_cipher(ssl_) );
            result = SOCKET_OK_CODE;
        } 
    }

    return result;
}

SocketReturnCode PeerSSL::gRead(void* buff, const size_t buffSize, const int timeoutms){

    pthread_mutex_lock (&mutex_);

    pollfd pfd = {fd_, POLLIN, 0};
    poll(&pfd, 1, timeoutms);
    
    if(pfd.revents == 0) {    
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_TimedOut; //timedout    
    }
    
    if(pfd.revents != POLLIN) {
        LOG->LogMessage(LOG_WARNING, "PeerSSL::read() Error! revents = %d", pfd.revents);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    size_t result = SSL_read(ssl_, buff, buffSize);
    if (result != buffSize) {
        logError(errno);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    pthread_mutex_unlock (&mutex_);
    return SocketReturnCode_OK;
}

SocketReturnCode PeerSSL::gReadMore(void* buff, const size_t buffSize){
    pthread_mutex_lock (&mutex_);
    
    size_t result = SSL_read(ssl_, buff, buffSize);
    if (result != buffSize) {
        logError(errno);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    pthread_mutex_unlock (&mutex_);
    return SocketReturnCode_OK;    
}

SocketReturnCode PeerSSL::gWrite(const void* buff, const size_t buffSize, const int timeoutms){
    
    pthread_mutex_lock (&mutex_);

    pollfd pfd = {fd_, POLLOUT, 0};
    poll(&pfd, 1, timeoutms);
    
    if(pfd.revents == 0) {     
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_TimedOut; //timedout    
    }
      
    if(pfd.revents != POLLOUT) {
        LOG->LogMessage(LOG_WARNING, "PeerSSL::write() Error! revents = %d", pfd.revents);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    size_t result = SSL_write(ssl_, buff, buffSize);
    if (result != buffSize) {
        logError(errno, false);
        pthread_mutex_unlock (&mutex_);
        return SocketReturnCode_Error;
    }
    
    pthread_mutex_unlock (&mutex_);
    return SocketReturnCode_OK;    
}


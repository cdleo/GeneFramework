
#include <sockets/PeerSSL_Async.h>
#include <sockets/GlobalDefinitions.h>

#include <fcntl.h>

bool PeerSSL_Async::sslInitialized_ = false;

PeerSSL_Async::PeerSSL_Async(const char* certificateFilePath, const char* keyFilePath) 
: ssl_(NULL), certificate_(certificateFilePath), key_(keyFilePath) {
    initializeSSL();    
}

PeerSSL_Async::~PeerSSL_Async() {
    disconnect();
    //DestroySSL();
}

bool PeerSSL_Async::connectToHost(const char* host, const int port, const int timeoutms){
    
    if ( PeerIN::connectToHost( host, port, timeoutms ) ) {

        SSL_CTX* sslctx = initCTX( SSLv23_client_method() );
        ssl_ = SSL_new(sslctx);        
        SSL_set_fd(ssl_, fd_ );           /* attach the socket descriptor */

        int ssl_err = SSL_connect(ssl_);
        if ( ssl_err != 1 ) {   /* perform the connn */
            LOG->LogMessage(LOG_WARNING,"PeerSSL_Async::connectToHost() SSL Error: %d", SSL_get_error( ssl_, ssl_err));   
            ShutdownSSL();
            disconnect();
        } else {  
            SSL_set_connect_state(ssl_);
            LOG->LogMessage(LOG_INFO, "PeerSSL_Async::connectToHost() Connected to %s:%d with %s encryption", host, port, SSL_get_cipher(ssl_) );
       }        
        
    }
    
    return isConnected();
}

int PeerSSL_Async::setFD(int fd) {
    
    int result = PeerIN::setFD( fd );
    if (result == SOCKET_OK_CODE) {
    
        SSL_CTX* sslctx = initCTX( SSLv23_server_method() );
        ssl_ = SSL_new(sslctx);        
        SSL_set_fd(ssl_, fd_ );       /* attach the socket descriptor */

        //Here is the SSL Accept portion.  Now all reads and writes must use SSL
        int ssl_err = SSL_accept(ssl_);
        if ( ssl_err != 1 )  {
            LOG->LogMessage(LOG_WARNING,"PeerSSL_Async::setFD() SSL Error: %d", SSL_get_error( ssl_, ssl_err));   
            ShutdownSSL();  //log and close down ssl
            disconnect();
            result = SOCKET_ERROR_CODE;        
        } else {
            SSL_set_accept_state(ssl_);
            LOG->LogMessage(LOG_INFO, "PeerSSL_Async::setFD() Connected to %s:%d with %s encryption", getRemoteIP().c_str(), getRemotePort(), SSL_get_cipher(ssl_) );
            result = SOCKET_OK_CODE;                      
        } 
    }

    return result;
}

void PeerSSL_Async::disconnect(){
    ShutdownSSL();
    PeerIN::disconnect();
}

SocketReturnCode PeerSSL_Async::gRead(void* buff, const size_t buffSize){

    if (!isConnected()) return SocketReturnCode_Error;
    
    size_t result = SSL_read(ssl_, buff, buffSize);
    if (result != buffSize) {
        logError(errno);
        return SocketReturnCode_Error;
    }

    return SocketReturnCode_OK;
}

SocketReturnCode PeerSSL_Async::gWrite(const void* buff, const size_t buffSize){
    
    if (!isConnected()) return SocketReturnCode_Error;
    
    size_t result = SSL_write(ssl_, buff, buffSize);
    if (result != buffSize) {
        logError(errno, false);
        return SocketReturnCode_Error;
    }
    
    return SocketReturnCode_OK;    
}



#include <sockets/PeerUN.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <bits/basic_string.h>

PeerUN::PeerUN(){
}

PeerUN::~PeerUN(){
}

bool PeerUN::connectToHost(const char* destination, const int timeoutms) {

    if (isConnected())
        disconnect();
    
    fd_ = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd_ < 0)
      LOG->WARNING("PeerUN::connect() ERROR opening socket");
    
    bzero((void *) &addr_, sizeof(addr_));
    addr_.sun_family = AF_UNIX;
    strcpy(addr_.sun_path, destination);
    socklen_t size = (offsetof (struct sockaddr_un, sun_path) + strlen (addr_.sun_path) + 1);
    
    int sockFlags = fcntl(fd_, F_GETFL, 0);      /* Obtenemos los flags x defecto del socket */
    fcntl(fd_, F_SETFL, sockFlags | O_NONBLOCK); /*Lo seteamos como NO bloqueante */
    
    bool connected_ = (connect(fd_, (struct sockaddr*) &addr_, size) == 0);
    if (!connected_) {
        if (errno != EINPROGRESS) {
             LOG->LogMessage(LOG_WARNING, "PeerUN::ERROR connecting to %s", destination );
             return false;
        }
        
        pollfd pfd = {fd_, POLLOUT, 0};
        poll(&pfd, 1, timeoutms);
        connected_ = (pfd.revents == POLLOUT);

        if (!connected_)
            LOG->LogMessage(LOG_WARNING, "TIMEOUT connecting to %s:%d", destination );     
        else
            fcntl(fd_, F_SETFL, sockFlags ); /* Volvemos a dejarlo con los flags x defecto */
    }
    
    if (connected_)
         LOG->LogMessage(LOG_DEBUG, "PeerUN::connectToHost() Connected to %s", destination );
    
    return isConnected();  
}

int PeerUN::setFD(int fd) {
    if (isConnected())
        disconnect();

    fd_ = fd;
    socklen_t peeraddrlen = sizeof(addr_);
    getpeername(fd, (struct sockaddr*) &addr_, &peeraddrlen);     

    LOG->LogMessage(LOG_DEBUG, "PeerUN::setFD() Connected to %s", getRemoteDestination().c_str() );
    
    return SOCKET_OK_CODE;
}    

std::string PeerUN::getRemoteDestination() { 
    char aux[108];
    strcpy(aux, addr_.sun_path);  
    return std::string(aux);
}
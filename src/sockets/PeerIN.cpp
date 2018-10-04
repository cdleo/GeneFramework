
#include <sockets/PeerIN.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <bits/basic_string.h>

PeerIN::PeerIN(){
}

PeerIN::~PeerIN(){
}

bool PeerIN::connectToHost(const char* ip, const int port, const int timeoutms) {

    if (isConnected())
        disconnect();

    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ <= 0) {
      LOG->WARNING("PeerIN::connect() ERROR opening socket");
      return false;
    }

    hostent* remote = gethostbyname( ip );
    if (remote == NULL) {
        LOG->LogMessage(LOG_WARNING, "PeerIN::ERROR, no such host (%s)", ip );
        return false;
    }

    bzero((void *) &addr_, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);    
    bcopy((char *)remote->h_addr, (char *)&addr_.sin_addr.s_addr, remote->h_length);

    //int sockFlags = fcntl(fd_, F_GETFL, 0);      /* Obtenemos los flags x defecto del socket */
    //fcntl(fd_, F_SETFL, sockFlags | O_NONBLOCK); /*Lo seteamos como NO bloqueante */

    bool connected_ = (connect(fd_, (struct sockaddr*) &addr_, sizeof(addr_)) == 0);
    if (!connected_) {
        if (errno != EINPROGRESS) {
             LOG->LogMessage(LOG_WARNING, "PeerIN::ERROR connecting to %s:%d", ip, port );
             return false;
        }

        pollfd pfd = {fd_, POLLOUT, 0};
        poll(&pfd, 1, timeoutms );
        connected_ = (pfd.revents == POLLOUT);

        if (!connected_) {
            disconnect();
            LOG->LogMessage(LOG_WARNING, "TIMEOUT connecting to %s:%d", ip, port );     
            return false;
        }
    }
    
    //fcntl(fd_, F_SETFL, sockFlags ); /* Volvemos a dejarlo con los flags x defecto */

    LOG->LogMessage(LOG_DEBUG, "PeerIN::connectToHost() Connected to %s:%d", ip, port );
    return true;    
}

int PeerIN::setFD(int fd){
    if (isConnected())
        disconnect();

    fd_ = fd;
    socklen_t peeraddrlen = sizeof(addr_);
    getpeername(fd_, (struct sockaddr*) &addr_, &peeraddrlen);

    //int sockFlags = fcntl(fd_, F_GETFL, 0);      /* Obtenemos los flags x defecto del socket */
    //fcntl(fd_, F_SETFL, sockFlags | O_NONBLOCK); /*Lo seteamos como NO bloqueante */            

    LOG->LogMessage(LOG_DEBUG, "PeerIN::setFD() Connected to %s:%d", getRemoteIP().c_str(), getRemotePort() );
    
    return SOCKET_OK_CODE;
}    

std::string PeerIN::getRemoteIP() { 
    char aux[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &addr_.sin_addr, aux, INET_ADDRSTRLEN );        
    return std::string(aux);
}

in_port_t PeerIN::getRemotePort() { 
    return ntohs(addr_.sin_port);
}

std::string PeerIN::getLocalIP() { 
    char aux[INET_ADDRSTRLEN];
    struct sockaddr_in  localAddr;
    socklen_t localAddrlen = sizeof(localAddr);

    getsockname(fd_, (struct sockaddr*) &localAddr, &localAddrlen);    
    strncpy (aux, inet_ntoa(localAddr.sin_addr), INET_ADDRSTRLEN);
    return std::string(aux);
}

in_port_t PeerIN::getLocalPort() { 

    struct sockaddr_in  localAddr;
    socklen_t localAddrlen = sizeof(localAddr);

    getsockname(fd_, (struct sockaddr*) &localAddr, &localAddrlen); 
    return ntohs(localAddr.sin_port);
}

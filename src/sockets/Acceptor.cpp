
#include <sockets/Acceptor.h>
#include <sockets/GlobalDefinitions.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <string.h>

#include <iostream>

Acceptor::Acceptor()
: listening_(false) {
}

Acceptor::~Acceptor(){
}

bool Acceptor::startListen(const int port, const int maxBacklog){
    
    poolfd_.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (poolfd_.fd < 0) {
        LOG->WARNING("Acceptor::open Error! socket failed");
        return false;
    }

    /*************************************************************/
    /* Allow socket descriptor to be reuseable                   */
    /*************************************************************/
    int on = 1;
    int rc = setsockopt(poolfd_.fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
    if (rc < 0) {
        LOG->WARNING("Acceptor::open Error! setsockopt failed");
        stopListen();
        return false;
    }

    /*************************************************************/
    /* Set socket to be nonblocking. All of the sockets for    */
    /* the incoming connections will also be nonblocking since  */
    /* they will inherit that state from the listening socket.   */
    /*************************************************************/
    rc = ioctl(poolfd_.fd, FIONBIO, (char *)&on);
    if (rc < 0) {
        LOG->WARNING("Acceptor::open Error! ioctl failed");
        stopListen();
        return false;
    }

    /*************************************************************/
    /* Bind the socket                                           */
    /*************************************************************/
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family      = AF_INET;
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_.sin_port        = htons(port);
    rc = bind(poolfd_.fd, (struct sockaddr *)&addr_, sizeof(addr_));
    if (rc < 0) {
        LOG->WARNING("Acceptor::open Error! bind failed");
        close(poolfd_.fd);
        return false;
    }

    /*************************************************************/
    /* Set the listen back log                                   */
    /*************************************************************/
    listening_ = (listen(poolfd_.fd, maxBacklog) == 0);
    if (!listening_) {
        LOG->WARNING("Acceptor::open Error! listen failed");
        stopListen();
        return false;
    }    
    
    return isListening();
}

bool Acceptor::stopListen(){
    if (isListening()) {
        close(poolfd_.fd);
        listening_ = false;
    }
    return !isListening();
}

int Acceptor::acceptRequest(PeerIN* peer, const int timeoutSeconds){
    
    struct sockaddr_storage addr;

    poolfd_.events = POLLIN;
    poll(&poolfd_, 1, (timeoutSeconds * 1000 ));
    
    if(poolfd_.revents == 0)
      return SOCKET_TIMEOUT_CODE; //timedout    
    
    if(poolfd_.revents != POLLIN) {
        LOG->LogMessage(LOG_WARNING, "Acceptor::accept() Error! revents = %d", poolfd_.revents);
        return SOCKET_ERROR_CODE;
    }

    int new_sd = accept(poolfd_.fd, NULL, NULL);
    if (new_sd <= 0) {
        if (errno != EWOULDBLOCK) {
          LOG->WARNING("Acceptor::accept() Error! Accept failed");
        }
        return SOCKET_ERROR_CODE;
    }

    /*****************************************************/
    /* Retrieve the remote host and port and log them    */
    /*****************************************************/  
    socklen_t len = sizeof(addr);
    getpeername(new_sd, (struct sockaddr*)&addr, &len);
    if (addr.ss_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        char ipstr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
        int port = ntohs(s->sin_port);
        LOG->LogMessage(LOG_INFO, "Acceptor::accept() New incoming connection from %s:%d (socket %d)", ipstr, port, new_sd);
    }
    
    /************************************************************/
    /* Add the new incoming connection to the pollfd structure  */
    /************************************************************/    
    return peer->setFD( new_sd );
}


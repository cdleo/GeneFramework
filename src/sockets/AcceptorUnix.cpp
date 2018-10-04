
#include <sockets/AcceptorUnix.h>
#include <sockets/GlobalDefinitions.h>

#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <string.h>

AcceptorUnix::AcceptorUnix()
: listening_(false) {
}

AcceptorUnix::~AcceptorUnix(){
}

bool AcceptorUnix::startListen(const char* destination, const int maxBacklog){
    
    poolfd_.fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (poolfd_.fd < 0) {
        LOG->WARNING("AcceptorUnix::open Error! socket failed");
        return false;
    }

    /*************************************************************/
    /* Allow socket descriptor to be reuseable                   */
    /*************************************************************/
    int on = 1;
    int rc = setsockopt(poolfd_.fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
    if (rc < 0) {
        LOG->WARNING("AcceptorUnix::open Error! setsockopt failed");
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
        LOG->WARNING("AcceptorUnix::open Error! ioctl failed");
        stopListen();
        return false;
    }

    /*************************************************************/
    /* Erase the old socket                                      */
    /*************************************************************/    
    unlink(destination);
    
    /*************************************************************/
    /* Bind the socket                                           */
    /*************************************************************/
    memset(&addr_, 0, sizeof(addr_));
    addr_.sun_family = AF_UNIX;
    strcpy(addr_.sun_path, destination);
    socklen_t size = (offsetof (struct sockaddr_un, sun_path) + strlen (addr_.sun_path) + 1);
    
    rc = bind(poolfd_.fd, (sockaddr*)&addr_,size);
    if (rc < 0) {
        LOG->WARNING("AcceptorUnix::open Error! bind failed");
        stopListen();
        return false;
    }

    /*************************************************************/
    /* Set the listen back log                                   */
    /*************************************************************/
    listening_ = (listen(poolfd_.fd, maxBacklog) == 0);
    if (!listening_) {
        LOG->WARNING("AcceptorUnix::open Error! listen failed");
        close(poolfd_.fd);
        return false;
    }    
    
    return isListening();
}

bool AcceptorUnix::stopListen(){
    if (isListening()) {
        close(poolfd_.fd);
        listening_ = false;
    }
    return !isListening();
}

int AcceptorUnix::acceptRequest(PeerUN* peer, const int timeoutSeconds){

    poolfd_.events = POLLIN;
    poll(&poolfd_, 1, (timeoutSeconds * 1000 ));
    
    if(poolfd_.revents == 0)
      return SOCKET_TIMEOUT_CODE; //timedout    
    
    if(poolfd_.revents != POLLIN) {
        LOG->LogMessage(LOG_WARNING, "AcceptorUnix::accept() Error! revents = %d", poolfd_.revents);
        return SOCKET_ERROR_CODE;
    }

    int new_sd = accept(poolfd_.fd, NULL, NULL);
    if (new_sd < 0) {
        if (errno != EWOULDBLOCK) {
          LOG->WARNING("AcceptorUnix::accept() Error! Accept failed");
        }
        return SOCKET_ERROR_CODE;
    }
    
    /************************************************************/
    /* Add the new incoming connection to the pollfd structure  */
    /************************************************************/    
    peer->setFD( new_sd );
    
    return SOCKET_OK_CODE;
}


/* 
 * File:   Acceptor.h
 * Author: cleo
 *
 * Created on May 17, 2014, 5:29 PM
 */

#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <sockets/PeerIN.h>

class Acceptor {
public:
    Acceptor();
    virtual ~Acceptor();
    
    bool startListen(const int port, const int maxBacklog);
    
    bool stopListen();  
    
    unsigned int getPort() { return ntohs(addr_.sin_port); }
    
    int acceptRequest(PeerIN* peer, const int timeoutSeconds);

    bool isListening() { return listening_; }
    
protected:
    struct sockaddr_in  addr_;
    pollfd              poolfd_;
    bool                listening_;    
};

#endif /* ACCEPTOR_H */

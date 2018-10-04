/* 
 * File:   AcceptorUnix.h
 * Author: cleo
 *
 * Created on June 18, 2014, 12:33 PM
 */

#ifndef ACCEPTORUNIX_H
#define	ACCEPTORUNIX_H

#include <sockets/PeerUN.h>

class AcceptorUnix {
public:
    AcceptorUnix();
    virtual ~AcceptorUnix();
    
    bool startListen(const char* destination, const int maxBacklog);
    
    bool stopListen();
    
    int acceptRequest(PeerUN* peer, const int timeoutSeconds);

    bool isListening() { return listening_; }
    
protected:
    struct sockaddr_un  addr_;
    pollfd              poolfd_;
    bool                listening_;    
};


#endif	/* ACCEPTORUNIX_H */


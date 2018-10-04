/* 
 * File:   PeerUN.h
 * Author: cleo
 *
 * Created on June 23, 2014, 3:08 PM
 */

#ifndef PEERUN_H
#define	PEERUN_H

#include <sockets/Peer.h>

#include <sys/un.h>

class PeerUN : public Peer {
public:
    PeerUN();
    virtual ~PeerUN();
    
    virtual bool connectToHost(const char* destination, const int timeoutms) = 0;
    virtual int setFD(int fd);  
    
    std::string getRemoteDestination();
    
protected:
    struct sockaddr_un          addr_;
};


#endif	/* PEERUN_H */


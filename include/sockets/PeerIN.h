/* 
 * File:   PeerIN.h
 * Author: cleo
 *
 * Created on June 23, 2014, 3:07 PM
 */

#ifndef PEERIN_H
#define	PEERIN_H

#include <sockets/Peer.h>

#include <arpa/inet.h>

class PeerIN : public Peer {
public:
    PeerIN();
    virtual ~PeerIN();
    
    virtual bool connectToHost(const char* ip, const int port, const int timeoutms);    
    virtual int setFD(int fd);
    
    virtual std::string getRemoteIP();
    virtual in_port_t getRemotePort();

    virtual std::string getLocalIP();
    virtual in_port_t getLocalPort();
        
protected:
    struct sockaddr_in          addr_;
};

#endif	/* PEERIN_H */


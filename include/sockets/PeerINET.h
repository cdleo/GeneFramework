/* 
 * File:   PeerINET.h
 * Author: cleo
 *
 * Created on May 17, 2014, 5:29 PM
 */

#ifndef PEERINET_H
#define	PEERINET_H

#include <sockets/PeerIN.h>
#include <sockets/ISync.h>

class PeerINET : public PeerIN, public ISync {
public:
    PeerINET();
    virtual ~PeerINET() {};

    virtual SocketReturnCode gRead(void* buff, const size_t buffSize, const int timeoutms);
    virtual SocketReturnCode gPeek(void* buff, const size_t buffSize, const int timeoutms);
    virtual SocketReturnCode gWrite(const void* buff, const size_t buffSize, const int timeoutms); 

};

#endif	/* PEERINET_H */


/* 
 * File:   PeerINET_Async_Async.h
 * Author: cleo
 *
 * Created on May 17, 2014, 5:29 PM
 */

#ifndef PEERINET_ASYNC_H
#define	PEERINET_ASYNC_H

#include <sockets/PeerIN.h>
#include <sockets/IAsync.h>

class PeerINET_Async : public PeerIN, public IAsync {
public:
    PeerINET_Async();
    virtual ~PeerINET_Async() {};

    virtual SocketReturnCode gRead(void* buff, const size_t buffSize);
    virtual SocketReturnCode gPeek(void* buff, const size_t buffSize);
    virtual SocketReturnCode gWrite(const void* buff, const size_t buffSize); 
};

#endif	/* PEERINET_ASYNC_H */


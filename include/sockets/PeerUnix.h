/* 
 * File:   PeerUnix.h
 * Author: cleo
 *
 * Created on June 18, 2014, 12:33 PM
 */

#ifndef PEERUNIX_H
#define	PEERUNIX_H

#include <sockets/PeerUN.h>
#include <sockets/ISync.h>

class PeerUnix : public PeerUN, public ISync {
public:
    PeerUnix();
    virtual ~PeerUnix();

    virtual SocketReturnCode gRead(void* buff, const size_t buffSize, const int timeoutms);
    virtual SocketReturnCode gPeek(void* buff, const size_t buffSize, const int timeoutms) { return SocketReturnCode_Error; };
    virtual SocketReturnCode gWrite(const void* buff, const size_t buffSize, const int timeoutms); 
    
protected:
    struct sockaddr_un          addr_;
};

#endif	/* PEERUNIX_H */


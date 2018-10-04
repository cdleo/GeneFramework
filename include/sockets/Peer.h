/* 
 * File:   Peer.h
 * Author: cleo
 *
 * Created on June 23, 2014, 1:51 PM
 */

#ifndef PEER_H
#define	PEER_H

#include <Log.h>

#include <poll.h>
#include <errno.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>
#include <sys/socket.h>

#include <sockets/GlobalDefinitions.h>

#include <threads/ScopedLock.h>

class Peer {
public:
    Peer();
    virtual ~Peer();
   
    virtual void disconnect();     
    
    virtual int setFD(int fd) = 0;
    virtual int getFD() const { return fd_; }
    
    virtual bool isConnected() { 
        ScopedLock lock(mutex_);
        return (fd_ != 0);
    }
    
    virtual int set_option (int level,int option,void *optval, int optlen);
    virtual int get_option (int level,int option,void *optval, int *optlen);
    
protected:
    void logError(int errorNumber, bool inRecvOperation = true);
    
    pthread_mutex_t             mutex_;
    
    int                         fd_;
    
};

#endif	/* PEER_H */


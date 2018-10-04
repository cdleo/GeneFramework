/* 
 * File:   ISync.h
 * Author: cleo
 *
 * Created on June 23, 2014, 3:07 PM
 */

#ifndef ISYNC_H
#define	ISYNC_H

#include <sockets/GlobalDefinitions.h>

class ISync {
public:
    virtual ~ISync(){};
    
    virtual SocketReturnCode gRead(void* buff, const size_t buffSize, const int timeoutms) = 0;
    virtual SocketReturnCode gPeek(void* buff, const size_t buffSize, const int timeoutms) = 0;
    virtual SocketReturnCode gWrite(const void* buff, const size_t buffSize, const int timeoutms) = 0; 
};

#endif	/* ISYNC_H */



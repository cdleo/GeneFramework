/* 
 * File:   IAsync.h
 * Author: cleo
 *
 * Created on June 23, 2014, 3:07 PM
 */

#ifndef IASYNC_H
#define	IASYNC_H

#include <sockets/GlobalDefinitions.h>

class IAsync {
public:
    virtual ~IAsync(){};
    
    virtual SocketReturnCode gRead(void* buff, const size_t buffSize) = 0;
    virtual SocketReturnCode gPeek(void* buff, const size_t buffSize) = 0;
    virtual SocketReturnCode gWrite(const void* buff, const size_t buffSize) = 0;    
};

#endif	/* IASYNC_H */


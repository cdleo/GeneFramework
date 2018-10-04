/* 
 * File:   IEvents.h
 * Author: cleo
 *
 * Created on October 4, 2014, 7:43 PM
 */

#ifndef IEVENTS_H
#define	IEVENTS_H

#include <sockets/GlobalDefinitions.h>

class IEvents{
public:
    virtual ~IEvents(){};
    
    virtual SocketReturnCode onRead_event() = 0;
    virtual SocketReturnCode onWrite_event() = 0;
    
    virtual void onError_event() = 0;
    virtual void onHangUp_event() = 0;
    
};


#endif	/* IEVENTS_H */


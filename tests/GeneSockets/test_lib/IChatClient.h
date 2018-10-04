/* 
 * File:   IChatClient.h
 * Author: cleo
 *
 * Created on October 8, 2014, 11:54 AM
 */

#ifndef ICHATCLIENT_H
#define	ICHATCLIENT_H

#include <Chat_Msg.h>

class IChatClient{
public:
    virtual ~IChatClient(){};
    
    virtual ssize_t read(Chat_Msg*& msg, const int timeoutSeconds) = 0;
    virtual ssize_t write(Chat_Msg* msg, const int timeoutSeconds) = 0;
    
    virtual bool isConn() = 0;
    virtual void disconn() = 0;
};

#endif	/* ICHATCLIENT_H */


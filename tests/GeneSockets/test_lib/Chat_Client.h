/* 
 * File:   Chat_Client.h
 * Author: cleo
 *
 * Created on April 23, 2014, 12:41 PM
 */

#ifndef CHAT_CLIENT_H
#define	CHAT_CLIENT_H

#include <IChatClient.h>

#include <sockets/PeerINET.h>

class Chat_Client : public IChatClient, public PeerINET {
public:
    Chat_Client();
    virtual ~Chat_Client();
   
    virtual ssize_t read(Chat_Msg*& msg, const int timeoutSeconds);
    virtual ssize_t write(Chat_Msg* msg, const int timeoutSeconds);

    virtual bool isConn() { return PeerINET::isConnected(); };
    virtual void disconn() { PeerINET::disconnect(); };    
    
};

#endif	/* CHAT_CLIENT_H */


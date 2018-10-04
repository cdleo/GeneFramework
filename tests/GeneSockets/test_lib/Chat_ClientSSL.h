/* 
 * File:   Chat_ClientSSLSSL.h
 * Author: cleo
 *
 * Created on June 23, 2014, 12:26 PM
 */

#ifndef CHAT_CLIENTSSL_H
#define	CHAT_CLIENTSSL_H

#include <IChatClient.h>

#include <sockets/PeerSSL.h>

class Chat_ClientSSL : public IChatClient, public PeerSSL {
public:
    Chat_ClientSSL();
    virtual ~Chat_ClientSSL();
   
    virtual ssize_t read(Chat_Msg*& msg, const int timeoutSeconds);
    virtual ssize_t write(Chat_Msg* msg, const int timeoutSeconds);

    virtual bool isConn() { return PeerSSL::isConnected(); };
    virtual void disconn() { PeerSSL::disconnect(); };            
    
};

#endif	/* CHAT_CLIENTSSL_H */


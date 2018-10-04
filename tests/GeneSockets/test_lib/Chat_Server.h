/* 
 * File:   Chat_Server.h
 * Author: cleo
 *
 * Created on April 23, 2014, 12:41 PM
 */

#ifndef CHAT_SERVER_H
#define	CHAT_SERVER_H

#include <Chat_Session.h>
#include <Chat_SessionSSL.h>
#include <Chat_SessionAsync.h>

#include <sockets/Acceptor.h>

class Chat_Server {
public:
    Chat_Server();
    virtual ~Chat_Server();
    
    bool listen(int port);
    
    int accept(Chat_Client* peer, int requestTimeout);
    
    int accept(Chat_ClientSSL* peer, int requestTimeout);
    
    int accept(Chat_ClientAsync* peer, int requestTimeout);
    
private:
    Acceptor* server_;
};


#endif	/* CHAT_SERVER_H */


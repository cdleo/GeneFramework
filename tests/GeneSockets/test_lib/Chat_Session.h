/* 
 * File:   Chat_Session.h
 * Author: cleo
 *
 * Created on April 28, 2014, 11:49 AM
 */

#ifndef CHAT_SESSION_H
#define	CHAT_SESSION_H

#include <Chat_Msg.h>
#include <Chat_Client.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

class Chat_Session {
public:
    Chat_Session();
    virtual ~Chat_Session();
    
    void add(Chat_Client* peer);
    void run();
    
private:
    void readAllMsg(Chat_Client* peer, const int timeoutSeconds);
    void broadcast(Chat_Msg* msg, const int timeoutSeconds);
    
    static void* threadFunc(void* pTr);
    
    //boost::thread               echoThread_;
    pthread_t                   echoThread_;
    
    std::map<int,Chat_Client*>  sessions_;
    bool                        closing_;
};

#endif	/* CHAT_SESSION_H */


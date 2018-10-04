/* 
 * File:   Chat_SessionSSLSSL.h
 * Author: cleo
 *
 * Created on June 23, 2014, 3:32 PM
 */

#ifndef CHAT_SESSIONSSL_H
#define	CHAT_SESSIONSSL_H

#include <Chat_Msg.h>
#include <Chat_ClientSSL.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

class Chat_SessionSSL {
public:
    Chat_SessionSSL();
    virtual ~Chat_SessionSSL();
    
    void add(Chat_ClientSSL* peer);

    void run();
    
private:
    static void* threadFunc(void* pTr);
    
    void readAllMsg(Chat_ClientSSL* peer, const int timeoutSeconds);
    void broadcast(Chat_Msg* msg, const int fdOwner, const int timeoutSeconds);
    
    //boost::thread               echoThread_;
    pthread_t                   echoThread_;
    
    std::map<int,Chat_ClientSSL*>   sessions_;
    bool                        closing_;
};


#endif	/* CHAT_SESSIONSSL_H */


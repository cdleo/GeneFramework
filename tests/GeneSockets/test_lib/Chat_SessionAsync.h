/* 
 * File:   Chat_SessionAsync.h
 * Author: cleo
 *
 * Created on October 8, 2014, 11:33 AM
 */

#ifndef CHAT_SESSIONASYNC_H
#define	CHAT_SESSIONASYNC_H

#include <Chat_Msg.h>
#include <Chat_ClientAsync.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

typedef std::vector<Chat_ClientAsync*> peers_t;

class Chat_SessionAsync {
public:
    Chat_SessionAsync();
    virtual ~Chat_SessionAsync();
    
    void add(Chat_ClientAsync* peer);

    void run();
    
private:
    static void* threadFunc(void* pTr);
    
    void readAllMsg(Chat_ClientAsync* peer, const int timeoutSeconds);
    void broadcast(Chat_Msg* msg, const int fdOwner, const int timeoutSeconds);
    
    //boost::thread               echoThread_;
    pthread_t                   echoThread_;
    
    peers_t                     sessions_;
    bool                        closing_;
};

#endif	/* CHAT_SESSIONASYNC_H */


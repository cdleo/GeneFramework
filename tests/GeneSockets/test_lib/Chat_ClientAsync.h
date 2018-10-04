/* 
 * File:   Chat_ClientAsync.h
 * Author: cleo
 *
 * Created on October 8, 2014, 11:32 AM
 */

#ifndef CHAT_CLIENTASYNC_H
#define	CHAT_CLIENTASYNC_H

#include <IChatClient.h>
#include <sockets/IEvents.h>
#include <sockets/PeerINET_Async.h>

#include <queue>

class Chat_ClientAsync : public IChatClient, public PeerINET_Async, public IEvents {
public:
    Chat_ClientAsync();
    virtual ~Chat_ClientAsync();
   
    virtual ssize_t read(Chat_Msg*& msg, const int timeoutSeconds);
    virtual ssize_t write(Chat_Msg* msg, const int timeoutSeconds);

    virtual bool isConn() { return PeerINET_Async::isConnected(); };
    virtual void disconn() { PeerINET_Async::disconnect(); };        
    
    virtual bool connectToHost(const char* ip, const int port, const int timeoutSeconds);
    virtual int setFD(int fd);    
    
protected:
    virtual SocketReturnCode onRead_event();
    virtual SocketReturnCode onWrite_event();
    
    virtual void onError_event();
    virtual void onHangUp_event();        
    
private:
    bool initialize();
    
    std::string         outstream_;
    std::queue<Chat_Msg*> instream_;
    
    pthread_mutex_t     readMutex_;
    pthread_mutex_t     writeMutex_;
    
    Chat_Msg*           pendingMsg;
    
};

#endif	/* CHAT_CLIENTASYNC_H */


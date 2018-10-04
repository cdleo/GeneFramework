/* 
 * File:   Chat_Observer.h
 * Author: cleo
 *
 * Created on April 24, 2014, 2:53 PM
 */

#ifndef CHAT_OBSERVER_H
#define	CHAT_OBSERVER_H

#include <Chat_Msg.h>
#include <boost/shared_ptr.hpp>

class Chat_Observer {
public:
    virtual void onConnect() = 0;
    virtual void onDisconnect() = 0;
    
    virtual void onDataSent(Chat_Msg& msg) = 0;    
    virtual void onDataReceived(Chat_Msg& msg) = 0;
    virtual void onDataDelivered(Chat_Msg& msg) = 0;
};

typedef boost::shared_ptr<Chat_Observer> CHAT_OBSERVER_PTR;

#endif	/* CHAT_OBSERVER_H */


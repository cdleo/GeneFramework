/* 
 * File:   Factory.h
 * Author: cleo
 *
 * Created on April 28, 2014, 8:49 AM
 */

#ifndef FACTORY_H
#define	FACTORY_H

#include <SessionsFactory.h>
#include <Chat_Session.h>
#include <Chat_Observer.h>

class Factory : public SessionsFactory {
public:
    Factory(CHAT_OBSERVER_PTR observer) : observer_(observer) {};
    
    TCPSESSION_PTR createTCPSession(boost::asio::io_service& io_service, Channel& channel) {
        return TCPSESSION_PTR(new Chat_Session(io_service, channel, observer_));
    }

private:
    CHAT_OBSERVER_PTR       observer_;

};

#endif	/* FACTORY_H */


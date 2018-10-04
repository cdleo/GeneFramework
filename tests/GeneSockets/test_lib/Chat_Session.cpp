/* 
 * File:   Chat_Session.cpp
 * Author: cleo
 * 
 * Created on April 28, 2014, 11:49 AM
 */

#include "Chat_Session.h"

Chat_Session::Chat_Session() 
: closing_(false) {
    //echoThread_ = boost::thread( boost::bind(&Chat_Session::run, this) );
    int rc = pthread_create (&echoThread_, NULL, threadFunc,this);
    if ( rc != 0 ) {        
        throw std::runtime_error("Error in thread creation... (pthread_create())");
    }      
}

Chat_Session::~Chat_Session() {
    closing_ = true;
    //echoThread_.join();
    pthread_join(echoThread_, NULL);
}

void* Chat_Session::threadFunc(void* pTr){
    Chat_Session* pThis = static_cast<Chat_Session*>(pTr);
    pThis->run();   
    pthread_exit(0);
}

void Chat_Session::add(Chat_Client* peer) {
    
    sessions_.insert( std::pair<int, Chat_Client*>(peer->getFD(), peer) );
    
}

void Chat_Session::run() {
    while (!closing_) {
        for(std::map<int,Chat_Client*>::iterator it = sessions_.begin(); it != sessions_.end(); it++) {
            readAllMsg(it->second, 2);
        }
        sleep(1);
    }
    
    for(std::map<int,Chat_Client*>::iterator it = sessions_.begin(); it != sessions_.end(); it++) {
        Chat_Client* peer = it->second;
        peer->disconnect();
        sessions_.erase(sessions_.find(peer->getFD()));
    }    
    
}

void Chat_Session::readAllMsg(Chat_Client* peer, const int timeoutSeconds) {    
    
    Chat_Msg* msg = NULL;
    int rc = peer->read(msg, timeoutSeconds);
    while (rc > 0) {
        std::cout << msg->getMsg().c_str() << std::endl;
        broadcast(msg, timeoutSeconds);
        delete msg;
        msg = NULL;        
        rc = peer->read(msg, timeoutSeconds);
    }

    if (rc < 0) {
        std::cout << "Client disconnected!!" << std::endl;
        sessions_.erase(sessions_.find(peer->getFD()));
    } 
}

void Chat_Session::broadcast(Chat_Msg* msg, const int timeoutSeconds) {    
    for(std::map<int,Chat_Client*>::iterator it = sessions_.begin(); it != sessions_.end(); it++) {
        if (it->second->write(msg, timeoutSeconds) < 0) {
            std::cout << "Client disconnected!!" << std::endl;
            sessions_.erase(it);
        }
    }
}

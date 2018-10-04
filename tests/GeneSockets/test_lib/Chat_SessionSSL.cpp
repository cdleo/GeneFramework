/* 
 * File:   Chat_SessionSSL.cpp
 * Author: cleo
 * 
 * Created on April 28, 2014, 11:49 AM
 */

#include "Chat_SessionSSL.h"

Chat_SessionSSL::Chat_SessionSSL() 
: closing_(false) {
    //echoThread_ = boost::thread( boost::bind(&Chat_SessionSSL::run, this) );
    int rc = pthread_create (&echoThread_, NULL, threadFunc,this);
    if ( rc != 0 ) {        
        throw std::runtime_error("Error in thread creation... (pthread_create())");
    }      
}

Chat_SessionSSL::~Chat_SessionSSL() {
    closing_ = true;
    //echoThread_.join();
    pthread_join(echoThread_, NULL);
}

void* Chat_SessionSSL::threadFunc(void* pTr){
    Chat_SessionSSL* pThis = static_cast<Chat_SessionSSL*>(pTr);
    pThis->run();   
    pthread_exit(0);
}

void Chat_SessionSSL::add(Chat_ClientSSL* peer) {
    
    sessions_.insert( std::pair<int, Chat_ClientSSL*>(peer->getFD(), peer) );
    
}

void Chat_SessionSSL::run() {
    while (!closing_) {
        for(std::map<int,Chat_ClientSSL*>::iterator it = sessions_.begin(); it != sessions_.end(); it++) {
            readAllMsg(it->second, 2);
        }
        usleep(100000);
    }
    
    for(std::map<int,Chat_ClientSSL*>::iterator it = sessions_.begin(); it != sessions_.end(); it++) {
        Chat_ClientSSL* peer = it->second;
        peer->disconnect();
        sessions_.erase(sessions_.find(peer->getFD()));
    }      
}

void Chat_SessionSSL::readAllMsg(Chat_ClientSSL* peer, const int timeoutSeconds) {    
    
    Chat_Msg* msg = NULL;
    int rc = peer->read(msg, timeoutSeconds);
    while (rc > 0) {
        std::cout << msg->getMsg().c_str() << std::endl;
        broadcast(msg, peer->getFD(), timeoutSeconds);
        delete msg;
        msg = NULL;
        rc = peer->read(msg, timeoutSeconds);
    }

    if (rc < 0) {
        std::cout << "Client disconnected!!" << std::endl;
        sessions_.erase(sessions_.find(peer->getFD()));
    } 
}

void Chat_SessionSSL::broadcast(Chat_Msg* msg, const int fdOwner, const int timeoutSeconds) {    
    for(std::map<int,Chat_ClientSSL*>::iterator it = sessions_.begin(); it != sessions_.end(); it++) {
        if ((it->second->getFD() != fdOwner) || (sessions_.size() == 1)) {
            if (it->second->write(msg, timeoutSeconds) < 0) {
                std::cout << "Client disconnected!!" << std::endl;
                sessions_.erase(it);
            }
        }
    }
}

/* 
 * File:   Chat_SessionAsync.cpp
 * Author: cleo
 * 
 * Created on October 8, 2014, 11:33 AM
 */

#include "Chat_SessionAsync.h"

Chat_SessionAsync::Chat_SessionAsync() 
: closing_(false) {
    //echoThread_ = boost::thread( boost::bind(&Chat_SessionAsync::run, this) );
    int rc = pthread_create (&echoThread_, NULL, threadFunc,this);
    if ( rc != 0 ) {        
        throw std::runtime_error("Error in thread creation... (pthread_create())");
    }      
}

Chat_SessionAsync::~Chat_SessionAsync() {
    closing_ = true;
    //echoThread_.join();
    pthread_join(echoThread_, NULL);
}

void* Chat_SessionAsync::threadFunc(void* pTr){
    Chat_SessionAsync* pThis = static_cast<Chat_SessionAsync*>(pTr);
    pThis->run();   
    pthread_exit(0);
}

void Chat_SessionAsync::add(Chat_ClientAsync* peer) {
    
    sessions_.push_back(peer);
    
}

void Chat_SessionAsync::run() {
    
    while (!closing_) {
        
        peers_t::iterator it = sessions_.begin();
        while ( it != sessions_.end()){        
            Chat_ClientAsync* peer = *it;
            if (!peer->isConnected()) {
                std::cout << "Client disconnected!!" << std::endl;
                delete peer;                
                it = sessions_.erase(it);
            } else {
               readAllMsg(peer, 2);
               ++it;
            }             
        }
        
        usleep(100000);
    }
    
    while( sessions_.size() ){
        sessions_.front()->disconnect();
        delete sessions_.front();
        sessions_.erase( sessions_.begin() );
    }    
}

void Chat_SessionAsync::readAllMsg(Chat_ClientAsync* peer, const int timeoutSeconds) {    
    
    Chat_Msg* msg = NULL;
    int rc = peer->read(msg, timeoutSeconds);
    while (rc > 0) {
        std::cout << msg->getMsg().c_str() << std::endl;
        broadcast(msg, peer->getFD(), timeoutSeconds);
        delete msg;
        msg = NULL;
        rc = peer->read(msg, timeoutSeconds);
    }
}

void Chat_SessionAsync::broadcast(Chat_Msg* msg, const int fdOwner, const int timeoutSeconds) {    
    for(peers_t::iterator it = sessions_.begin(); it != sessions_.end(); it++) {
        Chat_ClientAsync* peer = *it;
        if ((peer->getFD() != fdOwner) || (sessions_.size() == 1)) {
            if (peer->write(msg, timeoutSeconds) < 0) {
                peer->disconnect();
            }
        }
    }
}



#include <Chat_Server.h>

Chat_Server::Chat_Server() 
: server_( new Acceptor() ) {
    
}

Chat_Server::~Chat_Server(){
    server_->stopListen();
    if (server_) {
        delete server_;
        server_ = NULL;
    }
}

bool Chat_Server::listen(int port){
    return server_->startListen(port, 200);
    //return server_->startListen("/home/cleo/test",200);
}

int Chat_Server::accept(Chat_Client* peer, int requestTimeout) {
    return server_->acceptRequest(peer, requestTimeout); 
}

int Chat_Server::accept(Chat_ClientSSL* peer, int requestTimeout) {
    return server_->acceptRequest(peer, requestTimeout); 
}

int Chat_Server::accept(Chat_ClientAsync* peer, int requestTimeout) {
    return server_->acceptRequest(peer, requestTimeout); 
}

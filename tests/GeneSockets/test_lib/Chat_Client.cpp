
#include <Chat_Client.h>

Chat_Client::Chat_Client() {
}

Chat_Client::~Chat_Client(){
 
}

ssize_t Chat_Client::read(Chat_Msg*& msg, const int timeoutSeconds) {
    
    msg = new Chat_Msg();
    int rc = PeerINET::gRead(msg->getHeader()->getData(), msg->getHeader()->getMaxSize(), timeoutSeconds);
    if (rc <= 0)
        return rc;

    msg->getHeader()->decode();
    
    rc = PeerINET::gRead(msg->getBody()->getData(), msg->getBodyLength(), timeoutSeconds);
    if (rc <= 0)
        return rc;
    
    msg->getBody()->decode();
    
    return rc;
}

ssize_t Chat_Client::write(Chat_Msg* msg, const int timeoutSeconds) {
    std::string stream = msg->toStream();
    return PeerINET::gWrite(stream.c_str(), stream.length(), timeoutSeconds);
}



#include <Chat_ClientSSL.h>

Chat_ClientSSL::Chat_ClientSSL()
: PeerSSL("/home/cleo/cert.pem", "/home/cleo/key.pem"){
}

Chat_ClientSSL::~Chat_ClientSSL(){
 
}

ssize_t Chat_ClientSSL::read(Chat_Msg*& msg, const int timeoutSeconds) {
    
    msg = new Chat_Msg();
    int rc = PeerSSL::gRead(msg->getHeader()->getData(), msg->getHeader()->getMaxSize(), timeoutSeconds);
    if (rc <= 0)
        return rc;

    msg->getHeader()->decode();
    
    rc = PeerSSL::gReadMore(msg->getBody()->getData(), msg->getBodyLength());
    if (rc <= 0)
        return rc;
    
    msg->getBody()->decode();
    
    return rc;
}

ssize_t Chat_ClientSSL::write(Chat_Msg* msg, const int timeoutSeconds) {
    std::string stream = msg->toStream();
    return PeerSSL::gWrite(stream.c_str(), stream.length(), timeoutSeconds);
}


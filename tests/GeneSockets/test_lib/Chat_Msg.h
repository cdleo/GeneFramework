/* 
 * File:   Chat_Msg.h
 * Author: cleo
 *
 * Created on April 24, 2014, 9:49 AM
 */

#ifndef CHAT_MSG_H
#define	CHAT_MSG_H

#include <Chat_MsgHeader.h>
#include <Chat_MsgBody.h>

class Chat_Msg {
public:
    Chat_Msg() {
        /*header_ = new Chat_MsgHeader();
        body_ = new Chat_MsgBody();*/
    }
    
    ~Chat_Msg() {
        /*if (body_) {
            delete body_;
            body_ = NULL;
        }
        
        if (header_) {
            delete header_;
            header_ = NULL;
        }*/
    }
    
    Chat_Msg(const Chat_Msg& other)
    : header_(other.header_), body_( other.body_ ){
    }   
    
    void setMsg(std::string msg) {
       header_.setBodyLength(msg.size());
       body_.setBody(msg.c_str());
    }

    std::string getMsg(){
        return body_.getBody();
    }
    
    std::string toStream(){
        header_.encode();
        body_.encode();
        std::string aux( header_.getData() );
        aux.append( body_.getData() );
        return aux;
    }    
    
    size_t getBodyLength() { return header_.getBodyLength(); }
    
    Data* getHeader() const{
        return (Data*) &header_;
    }

    Data* getBody() const{
        return (Data*) &body_;
    }
    
    
private:
    Chat_MsgHeader  header_;
    Chat_MsgBody    body_;
    
};

#endif	/* CHAT_MSG_H */


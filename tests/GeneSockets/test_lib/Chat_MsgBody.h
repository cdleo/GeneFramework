/* 
 * File:   CHAT_MsgBody.h
 * Author: cleo
 *
 * Created on April 24, 2014, 9:49 AM
 */

#ifndef CHAT_MSGBODY_H
#define	CHAT_MSGBODY_H

#include <Data.h>

#include <string>

class Chat_MsgBody : public Data {
public:
    enum { max_body_length = 2048 };
    
    Chat_MsgBody() : Data(max_body_length) {}
    virtual ~Chat_MsgBody(){};
    
    Chat_MsgBody(const Chat_MsgBody& other) 
    : Data(max_body_length) {
        body_.assign(other.body_);
        dataLength_ = other.dataLength_;
        maxSize_ = other.maxSize_;
        setData(other.data_, other.dataLength_);
    }    
    
    void setBody(std::string body) { body_ = body; }
    std::string getBody() { return body_; }
    
    void encode() {
        memcpy(data_, body_.c_str(), body_.length());
        data_[body_.length()] = '\0';
        setLength(body_.length());
    }
    
    void decode() {
        body_.assign( data_ );
    }
    
private:
    std::string body_;
};

#endif	/* CHAT_MSGBODY_H */


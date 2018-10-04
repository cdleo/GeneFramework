/* 
 * File:   Chat_MsgHeader.h
 * Author: cleo
 *
 * Created on April 24, 2014, 9:49 AM
 */

#ifndef CHAT_MSGHEADER_H
#define	CHAT_MSGHEADER_H

#include <Data.h>
#include <stdio.h>
#include <stdlib.h>

class Chat_MsgHeader : public Data {
public:
    enum { header_length = 4 };
    
    Chat_MsgHeader() : Data(header_length), bodyLength_(0) {}
    virtual ~Chat_MsgHeader(){};
    
    Chat_MsgHeader(const Chat_MsgHeader& other) 
    : Data(header_length){
        bodyLength_ = other.bodyLength_;
        dataLength_ = other.dataLength_;
        maxSize_ = other.maxSize_;
        setData(other.data_, other.dataLength_);
    }
    
    void setBodyLength(size_t length) { bodyLength_ = length; }
    size_t getBodyLength() { return bodyLength_; }
    
    void encode() {
        char aux[header_length + 1] = "";
        sprintf(aux, "%4d", bodyLength_);
        memcpy(data_, aux, header_length);
        data_[header_length] = '\0';
        
        setLength(header_length);
    }
    
    void decode() {
        bodyLength_ = atoi(data_);
    }
    
private:
    size_t bodyLength_;
};

#endif	/* CHAT_MSGHEADER_H */


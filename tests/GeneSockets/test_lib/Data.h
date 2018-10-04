/* 
 * File:   Data.h
 * Author: cleo
 *
 * Created on April 22, 2014, 4:50 PM
 */

#ifndef DATA_H
#define	DATA_H

#include <stddef.h>
#include <string.h>
#include <string>
#include <deque>

class Data {
public:
    Data(size_t maxPacketSize) : data_(NULL), dataLength_(0), maxSize_(maxPacketSize) {
        data_ = new char[maxSize_];
        memset(data_, '\0', maxSize_);
    }
    
    virtual ~Data(){
        if (data_) {
            delete[] data_;
            data_ = NULL;
        }
    }

    size_t getMaxSize() { return maxSize_; };
    
    size_t getLength() { return dataLength_; };
    
    void setLength(size_t length) { dataLength_ = length; };
    
    char* getData() { return data_; };
    
    void setData(char* stream, size_t length) {
        setData(stream, length, 0);
    };    
    
    void setData(char* stream, size_t length, size_t offset) {
        if (maxSize_ < (length + offset))
            memcpy(data_ + offset, stream, maxSize_ - offset); 
        else
            memcpy(data_ + offset, stream, length); 
    };     
    
    virtual void encode() = 0;
    virtual void decode() = 0;
    
protected:
    char*       data_;
    size_t      dataLength_;
    
    size_t      maxSize_;
};

#endif	/* DATA_H */


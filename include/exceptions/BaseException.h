/* 
 * File:   Exception.h
 * Author: cleo
 *
 * Created on October 19, 2012, 12:20 PM
 */

#ifndef BASEEXCEPTION_H
#define	BASEEXCEPTION_H

#include <string>
#include <iostream>
#include <stdexcept>

namespace Exceptions {
    
    class BaseException : public std::runtime_error{
    public:
        BaseException(const std::string &msg = "") : runtime_error(msg) {}
    };

}

#endif	/* BASEEXCEPTION_H */



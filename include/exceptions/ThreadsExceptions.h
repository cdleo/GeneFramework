/* 
 * File:   ThreadsExceptions.h
 * Author: cleo
 *
 * Created on September 6, 2014, 9:38 PM
 */

#ifndef THREADSEXCEPTIONS_H
#define	THREADSEXCEPTIONS_H

#include <exceptions/BaseException.h>

namespace Exceptions {

    namespace ThreadsExceptions {

        class GeneralException : public BaseException {
        public:
            GeneralException(const std::string &msg = "") : BaseException(msg) {}
        }; 

        class UnableToCreateException : public GeneralException {
        public:
            UnableToCreateException(const std::string &msg = "") : GeneralException(msg) {}
        };   
        
        class UnableToJoinException : public GeneralException {
        public:
            UnableToJoinException(const std::string &msg = "") : GeneralException(msg) {}
        };          
        
    }

}

#endif	/* THREADSEXCEPTIONS_H */


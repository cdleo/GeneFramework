/* 
 * File:   WSException.h
 * Author: cleo
 *
 * Created on September 27, 2013, 10:50 AM
 */

#ifndef WSEXCEPTION_H
#define	WSEXCEPTION_H

#include <exceptions/BaseException.h>

namespace Exceptions {

    namespace WSExceptions {

        class GeneralException : public BaseException {
        public:
            GeneralException(const std::string &msg = "") : BaseException(msg) {}
        };

        class WrongDataType : public GeneralException {
        public:
            WrongDataType(const std::string &msg = "") : GeneralException(msg) {}
        };            
        
    }

}

#endif	/* WSEXCEPTION_H */


/* 
 * File:   ConnectionExceptions.h
 * Author: cleo
 *
 * Created on September 23, 2013, 11:30 AM
 */

#ifndef CONNECTIONEXCEPTIONS_H
#define	CONNECTIONEXCEPTIONS_H

#include <exceptions/BaseException.h>

namespace Exceptions {

    namespace ConnectionExceptions {

        class GeneralException : public BaseException {
        public:
            GeneralException(const std::string &msg = "") : BaseException(msg) {}
        };

        class UnableToConnectException : public GeneralException {
        public:
            UnableToConnectException(const std::string &msg = "") : GeneralException(msg) {}
        };            

        class ConnectionRefusedException : public GeneralException {
        public:
            ConnectionRefusedException(const std::string &msg = "") : GeneralException(msg) {}
        };  
        
        class ConnectionClosedException : public GeneralException {
        public:
            ConnectionClosedException(const std::string &msg = "") : GeneralException(msg) {}
        };  
        
        class ConnectionErrorException : public GeneralException {
        public:
            ConnectionErrorException(const std::string &msg = "") : GeneralException(msg) {}
        };          
                
        class ConnectionTimedOutException : public GeneralException {
        public:
            ConnectionTimedOutException(const std::string &msg = "") : GeneralException(msg) {}
        };   
        
        class DataException : public GeneralException {
        public:
            DataException(const std::string &msg = "") : GeneralException(msg) {}
        };    

    }

}

#endif	/* CONNECTIONEXCEPTIONS_H */


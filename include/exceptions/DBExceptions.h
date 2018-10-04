/* 
 * File:   DBException.h
 * Author: cleo
 *
 * Created on September 13, 2013, 10:50 AM
 */

#ifndef DBEXCEPTION_H
#define	DBEXCEPTION_H

#include <exceptions/BaseException.h>

namespace Exceptions {

    namespace DBExceptions {

        class GeneralException : public BaseException {
        public:
            GeneralException(const std::string &msg = "") : BaseException(msg) {}
        };

        class DataException : public GeneralException {
        public:
            DataException(const std::string &msg = "") : GeneralException(msg) {}
        };
        
        class QueryException : public GeneralException {
        public:
            QueryException(const std::string &msg = "") : GeneralException(msg) {}
        }; 

        class DataBaseNotReachable : public GeneralException {
        public:
            DataBaseNotReachable(const std::string &msg = "") : GeneralException(msg) {}
        };            

        class ConnectionFailed : public GeneralException {
        public:
            ConnectionFailed(const std::string &msg = "") : GeneralException(msg) {}
        }; 
    }

}

#endif	/* DBEXCEPTION_H */


/* 
 * File:   QueueExceptions.h
 * Author: cleo
 *
 * Created on December 20, 2012, 5:00 PM
 */

#ifndef QUEUEEXCEPTIONS_H
#define	QUEUEEXCEPTIONS_H

#include <exceptions/BaseException.h>

namespace Exceptions {

    namespace QueueExceptions {

        class GeneralException : public BaseException {
        public:
            GeneralException(const std::string &msg = "") : BaseException(msg) {}
        };

        class QueueFullException : public GeneralException {      
        public:
            QueueFullException(const std::string &msg = "") : GeneralException(msg) {}
        };    

        class QueueFailConnection : public GeneralException {
        public:
            QueueFailConnection(const std::string &msg = "") : GeneralException(msg) {}
        };

    }

}
#endif	/* QUEUEEXCEPTIONS_H */


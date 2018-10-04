/* 
 * File:   CreationExceptions.h
 * Author: cleo
 *
 * Created on November 4, 2013, 4:03 PM
 */

#ifndef CREATIONEXCEPTIONS_H
#define	CREATIONEXCEPTIONS_H

#include <exceptions/BaseException.h>

namespace Exceptions {

    namespace CreationExceptions {

        class GeneralException : public Exceptions::BaseException {
        public:
            GeneralException(const std::string &msg = "") : BaseException(msg) {};
        };

        class FactoryException : public GeneralException {
        public:
            FactoryException(const std::string &msg = "") : GeneralException(msg) {};
        };            

    }
}

#endif	/* CREATIONEXCEPTIONS_H */


/* 
 * File:   SignalsExceptions.h
 * Author: cleo
 *
 * Created on August 24, 2014, 12:57 PM
 */

#ifndef SIGNALSEXCEPTIONS_H
#define	SIGNALSEXCEPTIONS_H

#include <exceptions/BaseException.h>

namespace Exceptions {

    namespace SignalsExceptions {

        class GeneralException : public BaseException {
        public:
            GeneralException(const std::string &msg = "") : BaseException(msg) {}
        }; 

        class UnableToSetupSignalException : public GeneralException {
        public:
            UnableToSetupSignalException(const std::string &msg = "") : GeneralException(msg) {}
        };         
        
    }

}

#endif	/* SIGNALSEXCEPTIONS_H */


/* 
 * File:   ParametersExceptions.h
 * Author: cleo
 *
 * Created on October 19, 2012, 12:20 PM
 */

#ifndef PARAMETERSEXCEPTIONS_H
#define	PARAMETERSEXCEPTIONS_H

#include <exceptions/BaseException.h>

namespace Exceptions {

    namespace ParametersExceptions {

        class GeneralException : public BaseException {
        public:
            GeneralException(const std::string &msg = "") : BaseException(msg) {}
        };

        class RandomLimitsException : public GeneralException {
        public:
            RandomLimitsException(const std::string &msg = "") : GeneralException(msg) {}
        };

        class ValidationException : public GeneralException {
        public:
            ValidationException(const std::string &msg = "") : GeneralException(msg) {}
        };
        
        class SecurityException : public GeneralException {
        public:
            SecurityException(const std::string &msg = "") : GeneralException(msg) {}
        };    

    }
    
}
#endif	/* PARAMETERSEXCEPTIONS_H */


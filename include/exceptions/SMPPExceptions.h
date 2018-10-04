/* 
 * File:   SMPPExceptions.h
 * Author: cleo
 *
 * Created on September 23, 2013, 11:30 AM
 */

#ifndef SMPPEXCEPTIONS_H
#define	SMPPEXCEPTIONS_H

#include <exceptions/BaseException.h>

namespace Exceptions {

    namespace SMPPExceptions {

        class GeneralException : public BaseException {
        public:
            GeneralException(const std::string &msg = "") : BaseException(msg) {}
        }; 

    }

}

#endif	/* SMPPEXCEPTIONS_H */


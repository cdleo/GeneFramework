/* 
 * File:   FileException.h
 * Author: cleo
 *
 * Created on August 6, 2013, 1:35 PM
 */

#ifndef IOEXCEPTIONS_H
#define	IOEXCEPTIONS_H

#include <exceptions/BaseException.h>

namespace Exceptions {

    namespace IOExceptions {

        class GeneralException : public BaseException {
        public:
            GeneralException(const std::string &msg = "") : BaseException(msg) {}
        };

        class FileException : public GeneralException {
        public:
            FileException(const std::string &msg = "") : GeneralException(msg) {}
        };    

    }

}

#endif	/* IOEXCEPTIONS_H */


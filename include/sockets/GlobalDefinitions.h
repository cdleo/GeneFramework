/* 
 * File:   GlobalDefinitions.h
 * Author: cleo
 *
 * Created on May 20, 2014, 9:27 AM
 */

#ifndef GLOBALDEFINITIONS_BASESOCKETS_H
#define	GLOBALDEFINITIONS_BASESOCKETS_H

#include <string.h>

typedef enum SocketReturnCode {
    SocketReturnCode_TimedOut = 0,
    SocketReturnCode_OK = 1,
    SocketReturnCode_Error = -1,
    SocketReturnCode_InvalidData = -2,
    SocketReturnCode_NoEnoughData = -3
} SocketReturnCode;

#define SOCKET_DATA_ERROR_CODE -2
#define SOCKET_ERROR_CODE -1
#define SOCKET_TIMEOUT_CODE 0
#define SOCKET_OK_CODE 1

#define ERROR_MAXLENGTH 256
#define IP_HEADER_LENGTH 20

typedef struct PingReply {
    PingReply() : icmp_type(0), icmp_code(0), msec(0.0) {
        error[0] = '\0';
    }
    
    PingReply( const PingReply& other ) {
        strncpy(error, other.error, ERROR_MAXLENGTH);
        icmp_type = other.icmp_type;
        icmp_code = other.icmp_code;
        msec = other.msec;
    }
    
    char error[ERROR_MAXLENGTH];
    unsigned short icmp_type;
    unsigned short icmp_code;
    double msec;
} PingReply;

#endif	/* GLOBALDEFINITIONS_BASESOCKETS_H */


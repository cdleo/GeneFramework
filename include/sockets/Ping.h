/* 
 * File:   ping.h
 * Author: cleo
 *
 * Created on March 30, 2014, 3:34 PM
 */

#ifndef PING_H
#define PING_H

#include <string>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/timex.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include <sockets/GlobalDefinitions.h>

class Ping {
public:
    Ping(const char* destination, unsigned int timeout);
    PingReply Send();
  
private:
    unsigned short ip_checksum(void* vdata,size_t length);
    
    unsigned int id, seq, size, timeout;
    std::string destination;
};

#endif // PING_H 


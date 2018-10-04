/* 
 * File:   Poller.h
 * Author: cleo
 *
 * Created on May 17, 2014, 5:29 PM
 */

#ifndef POOLER_H
#define POOLER_H

#include <threads/Thread.h>
#include <sockets/IEvents.h>

#include <sys/poll.h>

#include <SingletonBase.h>
#include <vector>
#include <map>

//  Pollset to pass to the poll function.
typedef std::vector<pollfd> pollset_t;

typedef struct PoolData {
    int             index;
    IEvents*        events;
} PoolData;

//  Pollset index & events object map.
typedef std::map<int /*fd*/, PoolData /*data*/> data_map_t;

#define POLLER Poller::getInstance()

class Poller : public SingletonBase<Poller>, private Thread {
public:
    friend class SingletonBase<Poller>;
    
    bool add_fd (int fd, IEvents* events);
    bool rm_fd (int fd);
    
    void set_pollin( int fd );
    void reset_pollin( int fd );
    
    void set_pollout( int fd );
    void reset_pollout( int fd );

    void setTimeout( unsigned int timeoutms );
    unsigned int getTimeout();
    
private:
    Poller();
    virtual ~Poller() throw();
    
protected:
    virtual void run();
    
    unsigned int        timeoutms_;
    
    pthread_mutex_t     mutex_; 

    pollset_t           pollset_;    
    data_map_t          dataMap_;        
};

#endif /* POOLER_H */

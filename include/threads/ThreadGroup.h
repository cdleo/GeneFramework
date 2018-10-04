/* 
 * File:   ThreadGroup.h
 * Author: cleo
 *
 * Created on September 6, 2014, 9:52 PM
 */

#ifndef THREADGROUP_H
#define	THREADGROUP_H

#include <SingletonBase.h>
#include <Log.h>

#include <threads/Thread.h>

#include <vector>

class ThreadGroup {
public:
    ThreadsManager(unsigned int maxThreads = 0) 
    : maxThreads_(maxThreads) {
        pthread_mutex_init(&mutex_, NULL);
    };
    
    virtual ~ThreadsManager() throw() {
        joinAll();
        pthread_mutex_destroy(&mutex_);        
    };
    
    bool addThread(Thread* thread){   
        pthread_mutex_lock(&mutex_);   
        bool maxThreadsReached = ( (maxThreads_ > 0) && (threads_.size() >= maxThreads_) );
        if (maxThreadsReached) {
            for(std::vector<Thread*>::iterator it = threads_.begin(); it != threads_.end(); it++) {
                Thread* item = *it;
                if (item->isFinished()){
                    try {
                        item->join();
                    } catch(...) {
                        LOG->WARNING("[ThreadGroup::addThread] Join throw an exception!");
                    }
                    delete item;
                    threads_.erase(it);
                    maxThreadsReached = false;
                    break;
                }
            }
        }
        
        if (!maxThreadsReached) {
            threads_.push_back(thread);
            thread->start();
        }
        
        pthread_mutex_unlock(&mutex_);   
        return (!maxThreadsReached);
    }
    
    void removeThread(Thread* thread) {       
        pthread_mutex_lock(&mutex_);   
        for(std::vector<Thread*>::iterator it = threads_.begin(); it != threads_.end(); it++) {
            Thread* item = *it;
            if (item == thread) {
                threads_.erase(it);
                break;
            }
        }
        pthread_mutex_unlock(&mutex_);   
    }    
    
    void joinAll() {
        pthread_mutex_lock(&mutex_);   
        for(std::vector<Thread*>::iterator it = threads_.begin(); it != threads_.end(); it++) {
            Thread* item = *it;
            try {
                item->stop();
                item->join();
            } catch(...) {}
            delete item;
        }
        threads_.clear();
        pthread_mutex_unlock(&mutex_);   
    }
    
private:
    std::vector<Thread*>        threads_;
    
    unsigned int                maxThreads_;
    
    pthread_mutex_t             mutex_;
    
};


#endif	/* THREADGROUP_H */


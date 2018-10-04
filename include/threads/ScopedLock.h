/* 
 * File:   ScopedLock.h
 * Author: cleo
 *
 * Created on October 27, 2014, 1:40 PM
 */

#ifndef SCOPEDLOCK_H
#define	SCOPEDLOCK_H

#include <pthread.h>

class ScopedLock {
public: 
   ScopedLock(pthread_mutex_t& mutex)
   : mutex_(mutex) {
       pthread_mutex_lock(&mutex_);
   };
   
   virtual ~ScopedLock(){
       pthread_mutex_unlock(&mutex_);
   }

private:
    ScopedLock(const ScopedLock& other);
    
    pthread_mutex_t& mutex_;  

};

#endif	/* SCOPEDLOCK_H */


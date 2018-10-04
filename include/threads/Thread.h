/* 
 * File:   Thread.h
 * Author: cleo
 *
 * Created on March 31, 2014, 9:12 AM
 */

#ifndef THREADA_H
#define	THREADA_H

#include <errno.h>
#include <string>
#include <string.h>
#include <exceptions/ThreadsExceptions.h>

using namespace std;
using namespace Exceptions;
 
/** 
*  Abstract class for Thread management 
*/
class Thread {
public: 
    /**
     *   Default Constructor for thread
     */
   Thread();

   /**
     *   virtual destructor  
     */
   virtual ~Thread();

   /**
     *   Thread functionality Pure virtual function  , it will be re implemented in derived classes  
     */
   virtual void run() = 0;

   /**
    *   Function to start thread. 
    */
   void start() throw(ThreadsExceptions::GeneralException);

   /**
    *   Function to set flag informing the thread that must stop. 
    */   
   void stop() {
       m_canceled = true;
   }
   
   /**
    *   Function to join thread. 
    */
   void join() throw(ThreadsExceptions::UnableToJoinException);
   
   /**
    *   Function to set flag informing at the caller of thread finished. 
    */      
   void setFinished() {
       m_finished = true;
   }
   
   /**
    *   Function to get the flag informing thread finished. 
    */    
   bool isFinished() {
       return m_finished;
   }   
   
private:
    /**
    *   private Function to create thread. 
    */
    void createThread() throw(ThreadsExceptions::UnableToCreateException);

    /**
    *   Call back Function Passing to pthread create API
    */
    static void* threadFunc(void* pTr);

    /**
    *   Internal pthread ID.. 
    */
    pthread_t m_Tid;
     
    /**
    *   Internal flag to indicate thread finished.. 
    */
    bool m_finished;
    
protected:
    /**
    *   Internal flag to indicate thread stop from outside...
    */
    bool m_canceled;  

};

#endif	/* THREADA_H */


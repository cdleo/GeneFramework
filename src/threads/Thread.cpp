/* 
 * File:   Thread.cpp
 * Author: cleo
 * 
 * Created on March 31, 2014, 9:12 AM
 */
#include <threads/Thread.h>
#include <cstring>
#include <cstdlib>
 
Thread::Thread()
: m_Tid(0), m_finished(false), m_canceled(false) {
 
}
 
Thread::~Thread()
{
 
}
 
void Thread::start() throw(ThreadsExceptions::GeneralException)
{
   createThread();
}

void Thread::join() throw(ThreadsExceptions::UnableToJoinException)
{
    int rc = pthread_join(m_Tid, NULL);
    if ( rc != 0 )
    {        
        throw ThreadsExceptions::GeneralException("Error in thread join.... (pthread_join())");
    }  
}
 
void* Thread::threadFunc( void* pTr )
{
    Thread* pThis = static_cast<Thread*>(pTr);
    pThis->run();
    pThis->setFinished();
    pthread_exit(0); 
}
 
void Thread::createThread() throw(ThreadsExceptions::UnableToCreateException)
{
    int rc = pthread_create (&m_Tid, NULL, threadFunc,this);
    if ( rc != 0 )
    {        
        throw ThreadsExceptions::UnableToCreateException("Error in thread creation... (pthread_create())");
    }  
}

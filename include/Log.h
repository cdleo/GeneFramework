/*
 * File:   Log.h
 * Author: cleo
 *
 * Created on September 12, 2013, 2:56 PM
 */


#ifndef LOG_H
#define LOG_H

#include <SingletonBase.h>

#include <string>
#include <syslog.h>
#include <stdarg.h>

#define LOG Log::getInstance()

class Log : public SingletonBase<Log> {
public:
    friend class SingletonBase<Log>;

    void initialize(const std::string identifier, int facility = LOG_USER, int option = LOG_CONS | LOG_NDELAY | LOG_PID);
    void LogMessage(int logLevel, const std::string format, ...);
    void DEBUG(const std::string message);
    void INFO(const std::string message);
    void WARNING(const std::string message);
    void ERROR(const std::string message);
    void CRITICAL(const std::string message);

private:    
    Log();
    virtual ~Log() throw();
    Log& operator=(Log& orig);
    
    bool        _bIsInitialized;
};

#endif  /* LOG_H */


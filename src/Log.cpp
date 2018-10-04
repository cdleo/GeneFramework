/*
 * File:   Log.cpp
 * Author: cleo
 *
 * Created on September 12, 2013, 2:56 PM
 */

#include <Log.h>

Log::Log() {
    _bIsInitialized = false;
}

Log::~Log() throw() {
    closelog();
}

void Log::initialize(const std::string identifier, int facility, int option) {
    openlog( identifier.c_str(), option , facility);
    _bIsInitialized = true;
}

void Log::LogMessage(int logLevel, const std::string format, ...) {
    if(!_bIsInitialized) return;
    va_list args;
    va_start(args, format);
    vsyslog(logLevel, format.c_str(), args);
    va_end(args);
}

void Log::DEBUG(const std::string message) {
    if(!_bIsInitialized) return;
    syslog(LOG_DEBUG, "%s", message.c_str() );
}

void Log::INFO(const std::string message) {
    if(!_bIsInitialized) return;
    syslog(LOG_INFO, "%s", message.c_str() );
}

void Log::WARNING(const std::string message) {
    if(!_bIsInitialized) return;
    syslog(LOG_WARNING, "%s", message.c_str() );
}

void Log::ERROR(const std::string message) {
    if(!_bIsInitialized) return;
    syslog(LOG_ERR, "%s", message.c_str() );
}

void Log::CRITICAL(const std::string message) {
    if(!_bIsInitialized) return;
    syslog(LOG_CRIT, "%s", message.c_str() );
}

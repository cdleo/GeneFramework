/* 
 * File:   DBConnector.h
 * Author: cleo
 *
 * Created on September 13, 2013, 11:26 AM
 */

#ifndef DBCONNECTOR_H
#define	DBCONNECTOR_H

#include <exceptions/DBExceptions.h>
#include <Log.h>

#include <sstream>
#include <string>

class DBConnector {
public:
    virtual void setConnectionParameters(std::string sDB
                                       , std::string sHost
                                       , std::string sUser
                                       , std::string sPassword) = 0;
    
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() = 0;
    virtual void doPing() = 0;
    
    virtual bool execute(std::string sqlQuery) = 0;
    
    virtual bool beginTransaction() = 0;
    virtual bool commit() = 0;
    virtual bool rollback() = 0;      
    
protected:
    DBConnector() {};
    virtual ~DBConnector() throw() {};
    
    void fillError(const char* src, int errorCode, const char* errorDescription) { 
        if (Log::exists()) {
            std::ostringstream logInfo;
            logInfo << src << ">> [ERROR] Num: " <<  errorCode << " Desc: " << errorDescription;
            LOG->WARNING( logInfo.str() );        
        }
    }        
    
    std::string         _sDB;
    std::string         _sHost;
    std::string         _sUser;
    std::string         _sPassword; 
};

#endif	/* DBCONNECTOR_H */


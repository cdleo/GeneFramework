/* 
 * File:   MySQLConnector.h
 * Author: cleo
 * 
 * Created on September 13, 2013, 2:52 PM
 */

#ifndef MYSQLCONNECTOR_H
#define	MYSQLCONNECTOR_H

#include <mysql++.h>

#include <pthread.h>

#include <SingletonBase.h>
#include <db/DBConnector.h>

#define dbConnector MySQLConnector::getInstance()

typedef std::pair<std::string, std::string> SP_Parameter;

class MySQLConnector : public SingletonBase<MySQLConnector>
                     , public DBConnector {
public:
    friend class SingletonBase<MySQLConnector>;

    virtual void setConnectionParameters(std::string sDB
                                       , std::string sHost
                                       , std::string sUser
                                       , std::string sPassword);
    virtual bool connect(std::string sDB
                       , std::string sHost
                       , std::string sUser
                       , std::string sPassword); 
    virtual bool connect();
    virtual void disconnect();
    virtual bool isConnected();
    virtual void doPing(); 

    virtual bool execute(std::string sqlQuery);
    virtual bool queryData(std::string sqlQuery
                         , mysqlpp::StoreQueryResult& queryResult);

    virtual bool callProcedure(std::string procedure
                             , std::vector<SP_Parameter>& parameters
                             , mysqlpp::StoreQueryResult& queryResult);

    virtual bool beginTransaction();
    virtual bool commit();
    virtual bool rollback();
    
    virtual void enableExceptions(bool value);

private:
    MySQLConnector();
    virtual ~MySQLConnector() throw();

    long long getNow();

    mysqlpp::Connection*        _pConnection; 

    pthread_mutex_t             mutex_;
    
    long long                   _llLastPingLog;
};

#endif	/* MYSQLCONNECTOR_H */



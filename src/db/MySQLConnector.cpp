/* 
 * File:   MySQLConnector.cpp
 * Author: cleo
 * 
 * Created on September 13, 2013, 2:52 PM
 */

#include <db/MySQLConnector.h>
#include <BaseUtilities.h>
#include <unistd.h>

#define MYSQL_PING_RETRY  3
#define MYSQL_SLEEP_PING  5
#define LOG_PING_INTERVAL 30

MySQLConnector::MySQLConnector()
: _llLastPingLog(0) {
    _pConnection = new mysqlpp::Connection(false);
    _pConnection->disable_exceptions(); //Por defecto las desactivamos, hasta que explicitamente las activen
    
    _pConnection->set_option(new mysqlpp::MultiStatementsOption(true));
    _pConnection->set_option(new mysqlpp::MultiResultsOption(true));
    _pConnection->set_option(new mysqlpp::IgnoreSpaceOption(true));        
    
    pthread_mutex_init(&mutex_, NULL);
}
    
MySQLConnector::~MySQLConnector() throw() {
    if(_pConnection) {
        this->disconnect();
        
        delete _pConnection;
        _pConnection = NULL;
    }
    pthread_mutex_destroy(&mutex_);
}
    
bool MySQLConnector::connect(std::string sDB
                           , std::string sHost
                           , std::string sUser
                           , std::string sPassword) {
    this->setConnectionParameters(sDB, sHost, sUser, sPassword);
    return this->connect();
}

void MySQLConnector::setConnectionParameters(std::string sDB
                                           , std::string sHost
                                           , std::string sUser
                                           , std::string sPassword) {
    _sDB = sDB;
    _sHost = sHost;
    _sUser = sUser;
    _sPassword = sPassword;
}
    
bool MySQLConnector::connect() {
    
    bool result = false;
    LOG->LogMessage(LOG_DEBUG, "MySQLConnector::connect(%s,%s)", _sHost.c_str(), _sDB.c_str());
    
    try {        
        result = _pConnection->connect(_sDB.c_str(), _sHost.c_str(), _sUser.c_str(), _sPassword.c_str());
        if (!result) {
            LOG->LogMessage(LOG_WARNING, "MySQLConnector::connect(), %s", _pConnection->error());
        }        
        
    } catch (mysqlpp::Exception& e) {
        LOG->LogMessage(LOG_WARNING, "MySQLConnector::connect(), exception: %s", e.what());
    }
    return result;
}

void MySQLConnector::disconnect() {
    LOG->DEBUG("MySQLConnector::disconnect()");
    try {
        if(this->isConnected()) {
            _pConnection->disconnect();
        }   
    } catch(mysqlpp::Exception& e) {
        LOG->LogMessage(LOG_WARNING, "MySQLConnector::disconnect(), exception: %s",e.what());
    }
}

bool MySQLConnector::isConnected() {
    if (!_pConnection)
        return false;
    
    return _pConnection->connected();
}

void MySQLConnector::doPing() {
    bool result = false;
    
    try {
        if(_pConnection->connected())
            result = _pConnection->ping();
        
        for(int i = 0; ((i < MYSQL_PING_RETRY) && !result); i++) {
            sleep(MYSQL_SLEEP_PING);
            LOG->LogMessage(LOG_WARNING, 
                    "MySQLConnector::doPing(): Reintento %d despues del sleep.",i+1);
            this->disconnect();
            result = this->connect();
        }
        
        if(result) {
            long long now = getNow();
            if((now - _llLastPingLog) > LOG_PING_INTERVAL) {
                _llLastPingLog = now;
                LOG->DEBUG( std::string("MySQLConnector::doPing(): Estado= OK") );
            }    
        } else {
            LOG->DEBUG( std::string("MySQLConnector::doPing() Estado= NOT CONNECTED!") ); 
            throw Exceptions::DBExceptions::DataBaseNotReachable("Imposible reconectar a la DB!");
        }
        
    } catch (mysqlpp::Exception& e) {
        LOG->WARNING("MySQLConnector::doPing(), DB desconectada!");
        throw Exceptions::DBExceptions::DataBaseNotReachable("Imposible reconectar a la DB!");
    }
}
    
bool MySQLConnector::execute(std::string sqlQuery) {

    pthread_mutex_lock(&mutex_);
    
    doPing();

    LOG->LogMessage(LOG_DEBUG, "MySQLConnector::execute(), Ejecutando query: %s", sqlQuery.c_str() );    
    
    bool result = true;
    mysqlpp::Query query = _pConnection->query();
    if(!query.exec(sqlQuery)) {
        fillError("MySQLConnector::execute()", query.errnum(), query.error());
        result = false;
    }   
    
    pthread_mutex_unlock(&mutex_);
    return result;
}

bool MySQLConnector::queryData(std::string sqlQuery, mysqlpp::StoreQueryResult& queryResult) {
    
    pthread_mutex_lock(&mutex_);

    doPing();
    
    std::string logMsg = "MySQLConnector::queryData(), Ejecutando query: " + sqlQuery;
    LOG->DEBUG(logMsg);    
    
    bool result = true;    
    mysqlpp::Query query = _pConnection->query();
    queryResult = query.store(sqlQuery.c_str(), sqlQuery.length());
    if(query.errnum() != 0) {
        fillError("MySQLConnector::queryData()", query.errnum(), query.error());
        result = false;
    }   
    
    pthread_mutex_unlock(&mutex_);
    return result;
}

bool MySQLConnector::callProcedure(std::string procedure, std::vector<SP_Parameter>& parameters, mysqlpp::StoreQueryResult& queryResult) {
    
    pthread_mutex_lock(&mutex_);
    
    doPing();
    
    std::string parametersAux = "";
    mysqlpp::Query query = _pConnection->query();
    for(unsigned int i = 0; i < parameters.size(); i++){
        if (!parametersAux.empty())
            parametersAux.append(",");

        SP_Parameter tupla = parameters.at(i);
        parametersAux.append( tupla.first );
        std::string parameter = std::string("SET " + tupla.first + "=\'" + tupla.second + "\'; ");
        LOG->LogMessage(LOG_DEBUG, "MySQLConnector::callProcedure(), Seteando valor a parametro: %s", parameter.c_str() ); 
        if (!query.exec( parameter ) ) {
            fillError("MySQLConnector::callProcedure()", query.errnum(), query.error());
            pthread_mutex_unlock(&mutex_);
            return false;
        }
    }

    std::string sqlstr;
    sqlstr.assign( "CALL " + procedure + "(" + parametersAux + ");");
    
    std::string logMsg = "MySQLConnector::callProcedure(), Ejecutando Stored Procedure: ";
    logMsg.append(sqlstr);
    LOG->DEBUG(logMsg);      
    
    if (!query.exec ( sqlstr ) ) {
        fillError("MySQLConnector::callProcedure()", query.errnum(), query.error());
        pthread_mutex_unlock(&mutex_);
        return false;
    }
    
    sqlstr.assign( "SELECT " + parametersAux + ";" );
    
    logMsg = "MySQLConnector::callProcedure(), Obteniendo resultados con query: ";
    logMsg.append(sqlstr);
    LOG->DEBUG(logMsg);    
    
    queryResult = query.store(sqlstr.c_str(), sqlstr.length());
    if (!query.good() ) {
        fillError("MySQLConnector::callProcedure()", query.errnum(), query.error());
        pthread_mutex_unlock(&mutex_);
        return false;
    }
    
    pthread_mutex_unlock(&mutex_);
    return true;
}

long long MySQLConnector::getNow() {        
    char buffer[15];
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", timeinfo);

    return UTILITIES->lexical_cast<long long>(std::string(buffer));
} 

bool MySQLConnector::beginTransaction() {
    //MySQL automáticamente desactiva el autocommit hasta que se haga un COMMIT o ROLLBACK
    return this->execute("START TRANSACTION;"); 
}

bool MySQLConnector::commit(){
    return this->execute("COMMIT;");
}

bool MySQLConnector::rollback(){
    return this->execute("ROLLBACK;");
}

void MySQLConnector::enableExceptions(bool value) {
    if (value)
        _pConnection->enable_exceptions();
    else
        _pConnection->disable_exceptions();
}

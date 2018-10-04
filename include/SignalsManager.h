/* 
 * File:   SignalsManager.h
 * Author: cleo
 *
 * Created on December 26, 2012, 4:00 PM
 */

#ifndef SIGNALSMANAGER2_H
#define	SIGNALSMANAGER2_H

#include <SingletonBase.h>

#include <vector>

#include <signal.h>
#include <bits/signum.h>

#define SIGNALS SignalsManager::getInstance()

//Clase abstracta, implementada por el cliente y utilizada para notificar la señal atrapada
class SignalsHandler {
public:
    virtual void onSignalTrapped(int signal_number) = 0;
        
protected:
    SignalsHandler() {};
    virtual ~SignalsHandler() {};
};

class SignalsManager : public SingletonBase<SignalsManager> {
public:
    friend class SingletonBase<SignalsManager>;
    
    void initialize(SignalsHandler* handler, std::vector<int> signals);
        
    static void staticHandler (int signal_number);
    
private:
    SignalsManager();
    virtual ~SignalsManager() throw();
    
    void removeTraps();
    void onSignalTrapped (int signal_number);
    
    /*boost::asio::signal_set*            signalSet;
    boost::asio::io_service             ioService;
    boost::asio::io_service::work*      ioWork;
    boost::thread*                      ioWorkThread;*/
    
    std::vector<int>                    signals_;
    SignalsHandler*                     handler_;
    
};

#endif	/* SIGNALSMANAGER2_H */


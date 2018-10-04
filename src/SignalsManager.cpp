/* 
 * File:   SignalsManager.cpp
 * Author: cleo
 *
 * Created on December 26, 2012, 4:00 PM
 */

#include <SignalsManager.h>
#include <exceptions/SignalsExceptions.h>

#include <algorithm>
#include <iomanip>

SignalsManager::SignalsManager(){
    signals_.clear();
}
// =============================================================================

SignalsManager::~SignalsManager() throw() {
    removeTraps();
}
// =============================================================================

void SignalsManager::initialize(SignalsHandler* handler, std::vector<int> signals) {
    
    removeTraps();
    
    handler_ = handler;
    signals_.reserve(signals.size());
    std::copy(signals.begin(), signals.end(), std::back_inserter(this->signals_));
    
    for (std::vector<int>::iterator it = signals_.begin() ; it != this->signals_.end(); ++it) {
        if (signal((int) *it, SignalsManager::staticHandler) == SIG_ERR)
        {
            throw Exceptions::SignalsExceptions::UnableToSetupSignalException("!!!!! Error setting up signal handler !!!!!");
        }
    }        
}
// =============================================================================

void SignalsManager::removeTraps() {
    
    for (std::vector<int>::iterator it = signals_.begin() ; it != this->signals_.end(); ++it) {
        signal((int) *it, SIG_DFL);
    }
    signals_.clear();
}

// =============================================================================

void SignalsManager::staticHandler(int signal_number) {
    SIGNALS->onSignalTrapped(signal_number);
}
// =============================================================================

void SignalsManager::onSignalTrapped(int signal_number){
    handler_->onSignalTrapped(signal_number);
}


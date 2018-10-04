/* 
 * File:   HeartBeatFileTimestampUpdater.cpp
 * Author: cleo
 * 
 * Created on October 15, 2012, 2:41 PM
 */

#include <HeartBeatFileTimestampUpdater.h>
#include <Log.h>

#include <stdlib.h>
#include <fstream>

HeartBeatFileTimestampUpdater::HeartBeatFileTimestampUpdater()
: heartBeatFileName(""), refreshRatioInSeconds(0), timerTouchFile(0)  //Inicializamos timerTouchFile en 0, para que la primera vez no espere para crear el archivo
{    
}

HeartBeatFileTimestampUpdater::~HeartBeatFileTimestampUpdater() throw() {
}

void HeartBeatFileTimestampUpdater::initialize(std::string heartBeatFileName, const int refreshingRatioSeconds) {
    this->heartBeatFileName.assign(heartBeatFileName);
    refreshRatioInSeconds = refreshingRatioSeconds;
}

void HeartBeatFileTimestampUpdater::updateIfTimeHasBeenReached() {
    if (!heartBeatFileName.empty() && (time(NULL) - timerTouchFile > refreshRatioInSeconds)) {
        timerTouchFile = time(NULL);
        if (touch() && Log::exists()) {
            LOG->LogMessage(LOG_DEBUG, "[PROGRAM IS ALIVE] Heartbeat timestamp updated on file %s", heartBeatFileName.c_str());
        }
    }
}

bool HeartBeatFileTimestampUpdater::touch(void) {
    std::ofstream f;
    f.open(heartBeatFileName.c_str(), std::ofstream::out);
    if (f.fail()) {
        if (Log::exists())
            LOG->LogMessage(LOG_WARNING, "[ERROR] Unable to create/update heartbeat file %s", heartBeatFileName.c_str());
        return false;
    }    
    f.close();
    return true;
}

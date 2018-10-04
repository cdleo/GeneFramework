
#ifndef HEARTBEATFILETIMESTAMPUPDATER_H
#define	HEARTBEATFILETIMESTAMPUPDATER_H

#include <SingletonBase.h>
#include <string>

#define HEART_BEAT HeartBeatFileTimestampUpdater::getInstance()

class HeartBeatFileTimestampUpdater : public SingletonBase<HeartBeatFileTimestampUpdater> {
public:
    friend class SingletonBase<HeartBeatFileTimestampUpdater>;
    
    void initialize(std::string heartBeatFileName, const int refreshingRatioSeconds);
    void updateIfTimeHasBeenReached();
    
private:
    HeartBeatFileTimestampUpdater();
    HeartBeatFileTimestampUpdater(const HeartBeatFileTimestampUpdater& orig);
    virtual ~HeartBeatFileTimestampUpdater() throw();
    
    bool touch();

    std::string     heartBeatFileName;
    int             refreshRatioInSeconds;
    time_t          timerTouchFile;    
   
};

#endif	/* HEARTBEATFILETIMESTAMPUPDATER_H */


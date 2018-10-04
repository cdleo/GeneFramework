/*
 * File:   Paraqmeters.h
 * Author: cleo
 *
 * Created on July 29, 2014, 2:56 PM
 */


#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <SingletonBase.h>

#include <map>
#include <string>
#include <vector>

#define PARAMETERS Parameters::getInstance()

typedef std::map< std::string, std::vector<std::string> > ParametersMap;

class Parameters : public SingletonBase<Parameters> {
public:
    friend class SingletonBase<Parameters>;

    bool initialize(std::string routeToConfigFile);

    bool getValue(std::string key, long& value);
    bool getValue(std::string key, std::string& value);
    bool getValue(std::string key, std::vector<std::string>& values);
    
    bool addValue(std::string key, std::string value);
    bool setValue(std::string key, std::string value);
    bool setValue(std::string key, std::vector<std::string> values);
    
    bool existValue(std::string key, std::string value);
    
private:    
    Parameters();
    virtual ~Parameters() throw();
    Parameters& operator=(Parameters& orig);
    
    bool                _bIsInitialized;
    ParametersMap       parameters_;
};

#endif  /* PARAMETERS_H */


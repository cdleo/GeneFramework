#ifndef BASEUTILITIES_H
#define BASEUTILITIES_H

#include <SingletonBase.h>

#include <unistd.h>
#include <pthread.h>

#include <string>
#include <vector>
#include <sstream>

#include <boost/lexical_cast.hpp>

#define UTILITIES BaseUtilities::getInstance()

class BaseUtilities  : public SingletonBase<BaseUtilities> {
public:
    friend class SingletonBase<BaseUtilities>;
    
    std::string get_selfpath(std::string backupPath) {
        char buff[1024];
        ssize_t len = -1;
        #ifdef linux
            len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
        #elif __sun
            len = ::readlink("/proc/self/path/a.out", buff, sizeof(buff)-1);
        #endif
        if (len != -1) {
            buff[len] = '\0';
            return std::string(buff);
        }
        return backupPath;
    }
    
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }

    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }    
    
    bool isNumeric(std::string value) {
        try {
            pthread_mutex_lock(&mutex_);
            boost::lexical_cast<long long>(value);
            pthread_mutex_unlock(&mutex_);
            return true;
        } catch (boost::bad_lexical_cast &) {
            pthread_mutex_unlock(&mutex_);
            return false;
        }
    }    
    
    template<typename T>
    T lexical_cast(std::string value) {
        T resp = T();
        pthread_mutex_lock(&mutex_);
        std::istringstream (value) >> resp;
        pthread_mutex_unlock(&mutex_);
        return resp;
        /*try {
            pthread_mutex_lock(&mutex_);
            resp = boost::lexical_cast<T>(value);
            pthread_mutex_unlock(&mutex_);
            return resp;
        } catch (boost::bad_lexical_cast &) {
            pthread_mutex_unlock(&mutex_);
            return resp;
        }*/
    } 
    
    template<typename T>
    std::string lexical_cast(T value) {
        pthread_mutex_lock(&mutex_);
        std::ostringstream oss;
        oss << value;
        pthread_mutex_unlock(&mutex_);
        return std::string(oss.str());
        
        /*try {
            pthread_mutex_lock(&mutex_);
            resp = boost::lexical_cast<std::string>(value);
            pthread_mutex_unlock(&mutex_);
            return resp;
        } catch (boost::bad_lexical_cast &) {
            pthread_mutex_unlock(&mutex_);
            return resp;
        }*/
    }     
    
private:
    BaseUtilities() {
        pthread_mutex_init(&mutex_, NULL);
    }
    
    ~BaseUtilities() throw() {
        pthread_mutex_destroy(&mutex_);
    }
    
    pthread_mutex_t     mutex_;    
};

#endif  /* BASEUTILITIES_H */

#ifndef SINGLETONBASE_H
#define SINGLETONBASE_H

#include <stddef.h>

template <typename T>
class SingletonBase {
public:
    static T* getInstance() {
        if(!_pInstance) {
            _pInstance = new T();
        }
        return _pInstance;
    }

    static void destroyInstance() {
        if (_pInstance) {
            delete _pInstance;
            _pInstance = NULL;
        }
    }

    static bool exists() {
        return !(_pInstance == NULL);
    }
    
protected:
    SingletonBase() {};
    virtual ~SingletonBase() throw() {};

    static T* _pInstance;

private:
    SingletonBase& operator=(const SingletonBase&) {}
};

template <class T>
T* SingletonBase<T>::_pInstance = NULL;

#endif  /* SINGLETONBASE_H */

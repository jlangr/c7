#pragma once

#include <string>
#include <memory>

#include "Persistence.h"
#include "Serializable.h"

template <class T>
class KeyedFilePersistence: public Persistence<T>
{
public:
    KeyedFilePersistence(const std::string& table) 
        : Persistence<T>(table) 
    {
    }

    virtual ~KeyedFilePersistence(void) 
    {
    }

    unsigned int Size() const 
    { 
        return 0; 
    }

    virtual void Clear()
    {
    }

    void Add(T& object) 
    { 
    }

    std::auto_ptr<T> Get(const std::string& id) const
    {
        return std::auto_ptr<T>(NULL);
    }

    virtual bool Matches(MatcherFunction matches, const std::string& name) const
    {
        return false;
    }

    virtual void FindAllMatching(MatcherFunction matches, const std::string& name, std::vector<Serializable*>& matchesToPopulate) const
    {
    }

private:
    std::map<std::string,Serializable*> mData;
};

#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

#include "Serializable.h"

typedef bool(*MatcherFunction)(Serializable&, const std::string&);

template <class T>
class Persistence
{
public:
    Persistence(const std::string& table) : mTable(table) {}
    virtual ~Persistence(void) {}

    virtual unsigned int Size() const=0;
    virtual void Add(T&)=0;
    virtual void Clear()=0;
    virtual std::auto_ptr<T> Get(const std::string& id) const=0;
    virtual bool Matches(MatcherFunction, const std::string&) const=0;
    virtual void FindAllMatching(MatcherFunction, const std::string&, std::vector<Serializable*>& matchesToPopulate) const=0;

protected:
    std::string mTable;
};

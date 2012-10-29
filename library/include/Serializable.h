#pragma once

#include <string>
#include <iostream>

class Serializable
{
public:
    virtual void Load(std::istream& input)=0;
    virtual void WriteOn(std::ostream& output)=0;
    virtual std::string Id() const=0;
    virtual Serializable* Clone() const=0;
};

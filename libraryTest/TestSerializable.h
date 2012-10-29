#pragma once

#include <string>
#include <fstream>

#include "Serializable.h"
#include "StreamLineReader.h"

class TestSerializable: public Serializable
{
public:
    TestSerializable() 
        : mName("")
        , mId("") 
    {
    }

    TestSerializable(const std::string& name, const std::string& id)
        : mName(name)
        , mId(id) 
    {
    }

    virtual ~TestSerializable() 
    {
    }

    std::string Name() 
    { 
        return mName; 
    }

    virtual std::string Id() const 
    { 
        return mId; 
    }
    
    virtual void Load(std::istream& input)
    {
        StreamLineReader reader(&input);
        reader.Read(mName);
        reader.Read(mId);
    }

    virtual TestSerializable* Clone() const
    {
        return new TestSerializable(mName, mId);
    }

    virtual void WriteOn(std::ostream& output)
    {
        output << mName << std::endl << mId << std::endl;
    }

    bool operator==(const TestSerializable& other) const
    {
        return 
            mId == other.mId && 
            mName == other.mName;
    }

private:
    std::string mName;
    std::string mId;
};

#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <memory>

#include "boost/shared_ptr.hpp"

template <typename T>
class VectorReader
{
public:
    VectorReader(const std::string& name)
        : mName(name)
    {
    }

    virtual ~VectorReader(void) 
    {    
    }
    
    virtual std::vector<T> Load() const
    {
        std::ifstream mStream(mName.c_str());

        std::vector<T> objects;

        while (mStream) 
        {
            T object;
            object.Load(mStream);
            if (object.Id() != "") objects.push_back(object);
        }
        mStream.close();
        return objects;
    }

    std::string Name() const
    { 
        return mName; 
    }

private:
    std::string mName;
    std::ifstream* mStream;
};

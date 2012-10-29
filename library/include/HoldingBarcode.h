#pragma once

#include <string>
#include <sstream>

class HoldingBarcode
{
public:
    HoldingBarcode(const std::string& classification, unsigned int copyNumber) 
        : mClassification(classification)
        , mCopyNumber(copyNumber) 
    {
    }
    
    ~HoldingBarcode(void) 
    {
    }

    std::string AsString() const
    {
        std::stringstream buffer;
        buffer << mClassification << ":" << mCopyNumber;
        return buffer.str();
    }

    std::string Classification() const 
    { 
        return mClassification; 
    }
    
    unsigned int CopyNumber() const 
    { 
        return mCopyNumber; 
    }

private:
    std::string mClassification;
    unsigned int mCopyNumber;
};

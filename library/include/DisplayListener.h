#pragma once

#include <string>

class DisplayListener
{
public:
    virtual void ShowMessage(const std::string& text)=0;
};

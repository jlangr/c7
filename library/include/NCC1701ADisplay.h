#pragma once

#include "DisplayListener.h"

#include <string>

using namespace std;

class NCC1701ADisplay: public DisplayListener
{
public:
    NCC1701ADisplay() {}
    virtual ~NCC1701ADisplay() {}

    virtual void ShowMessage(const std::string& text)
    {
    }
};

#pragma once

#include <string>

class Report
{
public:
    virtual ~Report(void) {}

    virtual std::string Text() const=0;
    virtual std::string Name() const=0;
};

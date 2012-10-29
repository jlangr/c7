#pragma once

#include <exception>

class PatronNotFoundException: public std::exception
{
public:
    PatronNotFoundException(void) {}
    ~PatronNotFoundException() throw() {};
};

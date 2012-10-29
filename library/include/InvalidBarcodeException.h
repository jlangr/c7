#pragma once

#include <exception>

class InvalidBarcodeException: public std::exception {
public:
    InvalidBarcodeException(void) {};
    ~InvalidBarcodeException() throw() {};
};

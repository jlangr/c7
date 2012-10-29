#pragma once

#include <string>
#include <exception>

#include "Book.h"

class ClassificationService
{
public:
    ClassificationService(void);
    ~ClassificationService(void);

    Book RetrieveDetails(const std::string& classification) const;
};

class ClassificationNotFoundException: public std::exception
{
public:
    ClassificationNotFoundException(const std::string& classification) 
        : mClassification(classification) {}

    ~ClassificationNotFoundException() throw() {}

    // TODO include classification in message
    virtual const char* what() const throw()
    {
        return "classification not found";
    }

private:
    std::string mClassification;
};

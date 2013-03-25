#pragma once

#include <string>
#include <stdexcept>

#include "Book.h"

class ClassificationService
{
public:
    ClassificationService(void);
    ~ClassificationService(void);

    Book RetrieveDetails(const std::string& classification) const;
};

class ClassificationNotFoundException: public std::runtime_error {
public:
    ClassificationNotFoundException(const std::string& classification) 
        : std::runtime_error("classification not found")
        , mClassification(classification) {}

    std::string mClassification;
};

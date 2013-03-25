#pragma once

#include <exception>

#include "BranchAccess.h"

class DuplicateBranchNameException: public std::exception {
};

class BranchService
{
public:
    BranchService(void);
    virtual ~BranchService(void);

    static void DeleteAll();

    void Add(Branch& branch);
    std::string Add(const std::string& name, const std::string& address);
    short BranchCount() const;
    bool Find(Branch& branch) const;
    bool ExistsWithName(const std::string& name) const;

private:
    BranchAccess mBranchAccess;
};

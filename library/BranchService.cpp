#include "BranchService.h"

#include "Branch.h"

#include <string>
#include <sstream>

using namespace std;

BranchService::BranchService(void)
{
}

BranchService::~BranchService(void)
{
}

short BranchService::BranchCount() const
{
    return mBranchAccess.Size();
}

void BranchService::Add(Branch& branch)
{
    mBranchAccess.Save(branch);
}

string BranchService::Add(const string& name, const string& address)
{
    if (ExistsWithName(name))
        throw DuplicateBranchNameException();

    stringstream out;
    out << mBranchAccess.Size() + 1;
    string id(out.str());

    Branch branch(id, name);
    branch.SetAddress(address);
    mBranchAccess.Save(branch);
    return id;
}

bool BranchService::Find(Branch& branch) const
{
    return mBranchAccess.Find(branch);
}

bool BranchService::ExistsWithName(const string& name) const
{
    return mBranchAccess.ExistsWithName(name);
}

/* static */ void BranchService::DeleteAll()
{
    BranchAccess access;
    access.DeleteAll();
}

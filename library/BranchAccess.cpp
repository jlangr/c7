#include "BranchAccess.h"
#include "Branch.h"

#include "Persistence.h"
#include "KeyedMemoryPersistence.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>

using namespace std;

BranchAccess::BranchAccess(shared_ptr<Persistence<Branch>> persister)
    : mPersister(persister)
{
}

BranchAccess::BranchAccess()
    : mPersister(new KeyedMemoryPersistence<Branch>(BranchAccess::DataFileName()))
{
}

BranchAccess::~BranchAccess()
{
}

string BranchAccess::DataFileName()
{
    return "BranchAccess.txt";
}

bool MatchBranchByName(Serializable& each, const string& name)
{
    return (dynamic_cast<Branch&>(each)).Name() == name;
}

bool BranchAccess::ExistsWithName(const string& name) const
{
    return mPersister->Matches(MatchBranchByName, name);
}

bool BranchAccess::Find(Branch& branch) const
{
    auto_ptr<Branch> retrieved = mPersister->Get(branch.Id());
    if (retrieved.get() == NULL)
    {
        return false;
    }
    branch = *retrieved;
	return true;
}

int BranchAccess::Size() const
{
    return mPersister->Size();
}

void BranchAccess::Save(Branch& branch)
{
    mPersister->Add(branch);
}

void BranchAccess::DeleteAll()
{
    mPersister->Clear();
}

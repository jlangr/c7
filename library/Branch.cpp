#include "Branch.h"
#include "StreamLineReader.h"

#include <string>
#include <istream>
#include <ostream>
#include <sstream>

using namespace std;

Branch Branch::CHECKED_OUT("99", "-checked out-");

Branch::Branch() 
    : mId("")
    , mName("") 
{
}

Branch::Branch(const std::string& id, const std::string& name)
    : mId(id)
    , mName(name)
{
}

Branch::~Branch()
{
}

Branch* Branch::Clone() const
{
    Branch* branch = new Branch(mId, mName);
    branch->SetAddress(mAddress);
    return branch;
}

void Branch::Load(istream& input)
{
    StreamLineReader reader(&input);
    reader.Read(mId);
    reader.Read(mName);
    reader.Read(mAddress);
}

void Branch::WriteOn(ostream& output)
{
    output 
        << mId << endl
        << mName << endl 
        << mAddress << endl;
}

std::string Branch::Id() const
{
    return mId;
}

std::string Branch::Name() const
{
	return mName;
}

std::string Branch::Address() const
{
	return mAddress;
}

void Branch::SetAddress(const std::string& address)
{
	mAddress = address;
}

bool Branch::operator==(const Branch& other) const 
{
	return mId == other.mId;
}

bool Branch::operator!=(const Branch& other) const 
{
	return !(*this == other);
}

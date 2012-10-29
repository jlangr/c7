#include "PatronAccess.h"
#include "Patron.h"
#include "PatronNotFoundException.h"

#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

vector<Patron>* PatronAccess::mPatrons(NULL);

PatronAccess::PatronAccess()
{
}

PatronAccess::~PatronAccess()
{
}
	
void PatronAccess::Save(const Patron& patron)
{
	PatronAccess::Patrons()->push_back(patron);
}

void PatronAccess::Update(const Patron& patron)
{
	*Locate(patron) = patron;
}

std::vector<Patron>::iterator PatronAccess::Locate(const Patron& patron) const
{
	return find(PatronAccess::Patrons()->begin(),
				PatronAccess::Patrons()->end(),
				patron);
}

bool PatronAccess::Find(Patron& patron) const
{
	vector<Patron>::iterator it = Locate(patron);
	if (it == End())
    {
		return false;
    }
	patron = *it;
	return true;
}

struct PatronAccessFindByName: public binary_function<Patron, std::string, bool> 
{
	bool operator ()(Patron& patron, const string& name) const 
	{
		return (patron.Name() == name);
	}
};

Patron& PatronAccess::FindByName(const string& name) const
{
	vector<Patron>::iterator it =
		find_if(Begin(), End(), bind2nd(PatronAccessFindByName(), name));
	if (it == End())
    {
		throw PatronNotFoundException();
    }
	return *it;
}

vector<Patron>::iterator PatronAccess::Begin() const 
{
	return PatronAccess::Patrons()->begin();
}

vector<Patron>::iterator PatronAccess::End() const 
{
	return PatronAccess::Patrons()->end();
}

int PatronAccess::Size() const
{
	return PatronAccess::Patrons()->size();
}

vector<Patron> PatronAccess::GetAll() const
{
	return *Patrons();
}

/* static */ vector<Patron>* PatronAccess::Patrons()
{
	if (!mPatrons)
    {
		mPatrons = new vector<Patron>();
    }
	return mPatrons;
}
	
/* static */ void PatronAccess::DeleteAll()
{
	if (mPatrons)
	{
		delete mPatrons;
		mPatrons = NULL;
	}
}

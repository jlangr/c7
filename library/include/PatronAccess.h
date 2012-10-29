#pragma once

#include <string>
#include <vector>

class Patron;

class PatronAccess
{
public:
	PatronAccess();
	virtual ~PatronAccess();

	void Save(const Patron&);
	bool Find(Patron&) const;
	Patron& FindByName(const std::string&) const;
	int Size() const;
	void Update(const Patron&);
	std::vector<Patron> GetAll() const;

	static void DeleteAll();

private:
	PatronAccess& operator=(const PatronAccess&);
	PatronAccess(const PatronAccess&);
	
   std::vector<Patron>::iterator Locate(const Patron&) const;
	std::vector<Patron>::iterator Begin() const;
	std::vector<Patron>::iterator End() const;

	static std::vector<Patron>* mPatrons;
	static std::vector<Patron>* Patrons();
};

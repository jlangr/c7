#pragma once

#include <string>
#include <vector>
#include <set>

class Holding;

class Catalog
{
public:
	Catalog();
	virtual ~Catalog();

    static void DeleteAll();

	unsigned int Size() const;
	void Add(Holding& holding);
	void Update(Holding& holding);
	void FindByClassification(const std::string&, std::set<Holding>& holdings) const;
    bool FindByBarCode(Holding&) const;
    static std::vector<Holding>* Holdings();

private:
    Catalog(const Catalog&);

    static std::vector<Holding>* mHoldings;
};

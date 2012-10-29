#pragma once

#include <string>
#include <set>
#include <ostream>

#include "Holding.h"

class Patron {
public:
	Patron(const std::string& name = "", int id=0);
    Patron(const std::string& name, const std::string& cardNumber);

	std::set<Holding> Holdings() const;
	std::string Name() const;
	int Id() const;
    std::string CardNumber() const;
	int FineBalance() const;
	void AddFine(int amount);
	void Remit(int amount);
	void Borrow(const Holding& holding);
	void ReturnHolding(const Holding& holding);

	bool operator==(const Patron& rhs) const;
	bool operator!=(const Patron& rhs) const;

private:
	std::set<Holding> mHoldings;
	std::string mName;
	std::string mCreditCardNumber;
	int mId;
	int mBalance;
};

std::ostream& operator<<(std::ostream&, Patron&);

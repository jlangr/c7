#pragma once

#include "Branch.h"

#include <string>
#include <vector>

#include "boost/date_time/gregorian/gregorian_types.hpp"

class Holding
{
public:
	Holding();
    explicit Holding(const std::string& barcode);
    Holding(const std::string& classification, unsigned short copyNumber);
    virtual ~Holding();

    static std::string ConstructBarcode(
        const std::string& classification, const int& copyNumber);

    bool operator==(const Holding& rhs) const;
	bool operator!=(const Holding& rhs) const;
	bool operator<(const Holding& rhs) const;

    Branch CurrentBranch() const;
    unsigned short CopyNumber() const;
	std::string Barcode() const;
    boost::gregorian::date LastCheckedOutOn() const;
    boost::gregorian::date DueDate() const;
    bool IsAvailable() const;
    std::string Classification() const;

    void Transfer(Branch&);
    void CheckOut(boost::gregorian::date);
	void CheckIn(boost::gregorian::date, const Branch&);

private:
    std::string mClassification;
	unsigned short mCopyNumber;
	Branch mBranch;
	boost::gregorian::date mLastCheckedOutOn;
};

typedef std::vector<Holding>::iterator HoldingIterator;

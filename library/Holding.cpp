#include "Holding.h"
#include "Book.h"
#include "Branch.h"
#include "ClassificationService.h"
#include "InvalidBarcodeException.h"

#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include "boost/date_time/gregorian/gregorian_types.hpp"
#include "boost/algorithm/string.hpp"

using namespace boost::gregorian;
using namespace boost::algorithm;
using namespace std;

Holding::Holding(const string& barcode)
    : mBranch(Branch::CHECKED_OUT)
    , mLastCheckedOutOn()
{
    if (barcode.find(":") == string::npos)
    {
        throw InvalidBarcodeException();
    }
    vector<string> barcodeParts;
    boost::split(barcodeParts, barcode, boost::is_any_of(":"));
    string classification = barcodeParts[0];
    mCopyNumber = atoi(barcodeParts[1].c_str());
    mClassification = classification;
}

Holding::Holding(const string& classification, unsigned short copyNumber)
    : mClassification(classification)
    , mCopyNumber(copyNumber)
    , mBranch(Branch::CHECKED_OUT)
    , mLastCheckedOutOn()
{
}

Holding::Holding(Holding& holding, unsigned short copyNumber)
   : mClassification(holding.Classification())
   , mCopyNumber(copyNumber)
   , mBranch(holding.CurrentBranch())
//   , mLastCheckedOutOn(holding.LastCheckedOutOn())
{
}

Holding::Holding()
	: mClassification("")
	, mCopyNumber(1)
	, mBranch(Branch::CHECKED_OUT)
	, mLastCheckedOutOn()
{
}

Holding::~Holding()
{
}

bool Holding::operator==(const Holding& rhs) const
{
	return Barcode() == rhs.Barcode();
}

bool Holding::operator<(const Holding& rhs) const
{
	return Barcode() < rhs.Barcode();
}

bool Holding::operator!=(const Holding& rhs) const
{
	return !(*this == rhs);
}

string Holding::Classification() const
{
    return mClassification;
}

unsigned short Holding::CopyNumber() const
{
	return mCopyNumber;
}

Branch Holding::CurrentBranch() const
{
	return mBranch;
}

void Holding::Transfer(Branch& branch)
{
	mBranch = branch;
}

string Holding::Barcode() const
{
    return Holding::ConstructBarcode(mClassification, mCopyNumber);
}

/* static */ string Holding::ConstructBarcode(
	const string& classification, const int& copyNumber)
{
	stringstream buffer;
	buffer << classification << ":" << copyNumber;
	return buffer.str();
}

void Holding::CheckOut(date checkOutDate)
{
	mLastCheckedOutOn = checkOutDate;
	mBranch = Branch::CHECKED_OUT;
}

void Holding::CheckIn(date checkInDate, const Branch& branch)
{
	mBranch = branch;
}

date Holding::LastCheckedOutOn() const 
{
	return mLastCheckedOutOn;
}

date Holding::DueDate() const
{
    // should this go into HoldingService?

    // figure out how long the book can be held
   int period = 0;
      
   ClassificationService svc;
   Book book = svc.RetrieveDetails(mClassification);
   switch (book.Type()) {
      case Book::TYPE_BOOK:
         period = Book::BOOK_CHECKOUT_PERIOD;
         break;
      case Book::TYPE_MOVIE:
         period = Book::MOVIE_CHECKOUT_PERIOD;
         break;
      case Book::TYPE_NEW_RELEASE:
         period = Book::NEW_RELEASE_CHECKOUT_PERIOD;
         break;
      default:
         period = Book::BOOK_CHECKOUT_PERIOD;
         break;
      }
	return mLastCheckedOutOn + date_duration(period);
}

bool Holding::IsAvailable() const
{
	return Branch::CHECKED_OUT != mBranch;
}

#include "HoldingService.h"
#include "Branch.h"
#include "Book.h"
#include "Patron.h"
#include "PatronService.h"
#include "BranchService.h"
#include "ClassificationService.h"

#include <set>
#include "boost/date_time/gregorian/gregorian_types.hpp"

using namespace std;
using boost::gregorian::date;
using namespace service;

HoldingService::HoldingService()
{
}

HoldingService::~HoldingService()
{
}

void HoldingService::DeleteAll()
{
	Catalog::DeleteAll();
}

unsigned int HoldingService::InventorySize() const
{
    return mCatalog.Size();
}

void HoldingService::FindByClassification(const string& classification, set<Holding>& holdings) const
{
	mCatalog.FindByClassification(classification, holdings);
}

bool HoldingService::ExistsWithBarcode(const std::string& barCode) const
{
    Holding holding(barCode);
    return FindByBarCode(holding);
}

bool HoldingService::IsAvailable(const std::string& barCode) const
{
    Holding holding(barCode);
    return FindByBarCode(holding) && holding.IsAvailable();
}

bool HoldingService::FindByBarCode(Holding& holding) const
{
	return mCatalog.FindByBarCode(holding);
}

void HoldingService::AddAtBranch(const string& id, const string& barcode) 
{
    Branch branch(id);
    mBranchService.Find(branch);

    Holding holding(barcode);
    holding.Transfer(branch);
    mCatalog.Add(holding);
}

void HoldingService::Transfer(Holding& holding, Branch& branch)
{
	holding.Transfer(branch);
	mCatalog.Update(holding);
}

void HoldingService::Transfer(const string& barCode, const string& branchId)
{
    Branch branch(branchId);
    mBranchService.Find(branch);

    Holding holding(barCode);
    FindByBarCode(holding);
    holding.Transfer(branch);
    mCatalog.Update(holding);
}

void HoldingService::CheckOut(const string& patronCardNumber, const string& barCode, date date)
{
	Holding holding(barCode);
	FindByBarCode(holding);
	holding.CheckOut(date);
	mCatalog.Update(holding);

	Patron patron("", patronCardNumber);
    mPatronService.Find(patron);

    patron.Borrow(holding);
    mPatronService.Update(patron);
}

void HoldingService::CheckIn(const string& barCode, date date, const string& branchId)
{
    Branch branch(branchId);
    mBranchService.Find(branch);

    Holding hld(barCode);
	FindByBarCode(hld);

	vector<Patron> patrons;

	// set the holding to returned status
	set<Holding> holdings;
	hld.CheckIn(date, branch);
	mCatalog.Update(hld);

	patrons = mPatronService.GetAll();

	Holding patHld; 
      
	// locate the patron with the checked out book
	// could introduce a patron reference ID in the holding...
	Patron f;
	Patron p;
	vector<Patron>::const_iterator it;
	for (it = patrons.begin(); 
			it != patrons.end();
			it++) 
    {
		p = *it;
		holdings = p.Holdings();
		set<Holding>::const_iterator it1;
		for (it1 = holdings.begin(); it1 != holdings.end(); it1++) 
        {
			patHld = *it1;
			if (hld.Classification() == patHld.Classification())
	        
            f = p;
		}
	}

  	// remove the book from the patron
	f.ReturnHolding(hld);

	// check for late returns
	bool isLate = false;

	if (date > hld.DueDate()) // is it late?
		isLate = true;

	if (isLate) {
		int daysLate = 1; // calculate # of days past due

        ClassificationService service;
        Book book = service.RetrieveDetails(hld.Classification());

		switch (book.Type()) {
			case Book::TYPE_BOOK:
				f.AddFine(Book::BOOK_DAILY_FINE * daysLate); 
				break;
			case Book::TYPE_MOVIE:
				{
					int fine = 100 + Book::MOVIE_DAILY_FINE * daysLate;
					if (fine > 1000)
						fine = 1000;
					f.AddFine(fine);
				}
				break;
			case Book::TYPE_NEW_RELEASE:
				f.AddFine(Book::NEW_RELEASE_DAILY_FINE * daysLate);
				break;
		}
	}
	mPatronService.Update(f);
}

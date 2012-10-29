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

// START:CheckIn
void HoldingService::CheckIn(
      const string& barCode, date date, const string& branchId)
{
   Branch branch(branchId);
   mBranchService.Find(branch);

   Holding holding(barCode);
   FindByBarCode(holding);

   holding.CheckIn(date, branch);
   mCatalog.Update(holding);

   Patron patronWithBook = FindPatron(holding);

   patronWithBook.ReturnHolding(holding);

   if (IsLate(holding, date)) 
      ApplyFine(patronWithBook, holding);

   mPatronService.Update(patronWithBook);
}
// END:CheckIn

// START:ApplyFine
void HoldingService::ApplyFine(Patron& patronWithHolding, Holding& holding)
{
   int daysLate = CalculateDaysPastDue(holding);

   ClassificationService service;
   Book book = service.RetrieveDetails(holding.Classification());

   switch (book.Type()) {
      case Book::TYPE_BOOK:
         patronWithHolding.AddFine(Book::BOOK_DAILY_FINE * daysLate); 
         break;
      case Book::TYPE_MOVIE:
         {
            int fine = 100 + Book::MOVIE_DAILY_FINE * daysLate;
            if (fine > 1000)
               fine = 1000;
            patronWithHolding.AddFine(fine);
         }
         break;
      case Book::TYPE_NEW_RELEASE:
         patronWithHolding.AddFine(Book::NEW_RELEASE_DAILY_FINE * daysLate);
         break;
   }
}
// END:ApplyFine

unsigned int HoldingService::CalculateDaysPastDue(Holding& holding)
{
   return 1; // TODO
}

bool HoldingService::IsLate(Holding& holding, date& date)
{
   return date > holding.DueDate();
}

Patron HoldingService::FindPatron(Holding& holding)
{
   for (Patron& patron: mPatronService.GetAll())
   {
      set<Holding> holdings = patron.Holdings();
      for (const Holding& patronHolding: holdings)
         if (holding.Classification() == patronHolding.Classification())
            return patron;
   }
   return Patron();
}


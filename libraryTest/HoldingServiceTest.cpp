#include "gmock/gmock.h"

#include "HoldingService.h"
#include "Branch.h"
#include "BranchService.h"
#include "ClassificationData.h"
#include "Patron.h"
#include "PatronService.h"
#include "HoldingBarcode.h"

#include "boost/date_time/gregorian/gregorian_types.hpp"
#include "boost/assign/list_of.hpp"

using namespace std;
using namespace testing;
using namespace ClassificationData;
using namespace boost::assign;
using namespace boost::gregorian;
using namespace service;

class HoldingServiceTest: public Test
{
public:
    Branch* branch1;
    Branch* branch2;
    PatronService patronService;
    BranchService branchService;
    HoldingService holdingService;
    date* arbitraryDate;

	virtual void SetUp() override
	{
		HoldingService::DeleteAll();
        PatronService::DeleteAll();
        BranchService::DeleteAll();

        arbitraryDate = new date(2013, Jan, 1);

		branch1 = new Branch("1", "branch1");
		branch2 = new Branch("2", "branch2");
        branchService.Add(*branch1);
        branchService.Add(*branch2);
	}

	virtual void TearDown() override
	{
		delete branch1;
		delete branch2;

        HoldingService::DeleteAll();
        PatronService::DeleteAll();
        BranchService::DeleteAll();
	}

    void AddPatronWithId(string id)
    {
        Patron patron("", id);
        patronService.Add(patron);
    }

    Patron FindPatronWithId(string id) 
    {
        Patron patron("", id);
        patronService.Find(patron);
        return patron;
    }

    Holding FindHolding(HoldingBarcode& barcode)
    {
        Holding holding(barcode.AsString());
        holdingService.FindByBarCode(holding);
        return holding;
    }

    Holding FindHolding(const string& barcode)
    {
        Holding holding(barcode);
        holdingService.FindByBarCode(holding);
        return holding;
    }

    void CheckOut(HoldingBarcode& barcode, Branch* branch, string patronCardNumber="p1000")
    {
        AddPatronWithId(patronCardNumber);
        holdingService.AddAtBranch(branch->Id(), barcode.AsString());
        holdingService.CheckOut(patronCardNumber, barcode.AsString(), *arbitraryDate);
    }
};

TEST_F(HoldingServiceTest, SizeInitiallyZero)
{
    ASSERT_THAT(holdingService.InventorySize(), Eq(0u));
}

TEST_F(HoldingServiceTest, SizeIncrementedOnAddRegardlessOfBranch)
{
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 1).AsString());
    ASSERT_THAT(holdingService.InventorySize(), Eq(1u));

    holdingService.AddAtBranch(branch2->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 2).AsString());
    ASSERT_THAT(holdingService.InventorySize(), Eq(2u));
}

TEST_F(HoldingServiceTest, DeleteAllSetsSizeToZero)
{
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 1).AsString());
    holdingService.AddAtBranch(branch2->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 2).AsString());

	HoldingService::DeleteAll();

    ASSERT_THAT(holdingService.InventorySize(), Eq(0u));
}

TEST_F(HoldingServiceTest, AddInitializesBranch)
{
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 1).AsString());
    Holding holding(THE_TRIAL_CLASSIFICATION, 1);

    holdingService.FindByBarCode(holding);

    ASSERT_THAT(holding.CurrentBranch(), Eq(*branch1));
}

TEST_F(HoldingServiceTest, AddedHoldingCanBeFound)
{
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 1).AsString());

    Holding theTrial(THE_TRIAL_CLASSIFICATION, 1);
    ASSERT_TRUE(holdingService.FindByBarCode(theTrial));
}

TEST_F(HoldingServiceTest, ExistsReturnsFalseWhenNotFound)
{
    string barcode = Holding::ConstructBarcode(CATCH22_CLASSIFICATION, 1);

    bool found = holdingService.ExistsWithBarcode(barcode);

    ASSERT_FALSE(found);
}

TEST_F(HoldingServiceTest, ExistsReturnsTrueWhenNotFound)
{
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    string barcode = Holding::ConstructBarcode(CATCH22_CLASSIFICATION, 1);

    bool found = holdingService.ExistsWithBarcode(barcode);

    ASSERT_TRUE(found);
}

TEST_F(HoldingServiceTest, IsAvailableReturnsTrueWhenHoldingAvailable)
{
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    string barcode = Holding::ConstructBarcode(CATCH22_CLASSIFICATION, 1);

    bool isAvailable = holdingService.IsAvailable(barcode);

    ASSERT_TRUE(isAvailable);
}

TEST_F(HoldingServiceTest, IsAvailableReturnsFalseWhenHoldingCheckedOut)
{
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    HoldingBarcode barcode(THE_TRIAL_CLASSIFICATION, 1);
    CheckOut(barcode, branch1);

    bool isAvailable = holdingService.IsAvailable(barcode.AsString());

    ASSERT_FALSE(isAvailable);
}

TEST_F(HoldingServiceTest, FindByClassificationReturnsEmptyWhenNoMatch)
{
    set<Holding> holdings;

    holdingService.FindByClassification(THE_TRIAL_CLASSIFICATION, holdings);

    ASSERT_TRUE(holdings.empty());
}

TEST_F(HoldingServiceTest, FindByClassificationReturnsMultipleMatches)
{
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 1).AsString());
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(THE_TRIAL_CLASSIFICATION, 2).AsString());
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    set<Holding> holdings;

    holdingService.FindByClassification(THE_TRIAL_CLASSIFICATION, holdings);

    Holding trialCopy1(THE_TRIAL_CLASSIFICATION, 1);
    Holding trialCopy2(THE_TRIAL_CLASSIFICATION, 2);
    ASSERT_THAT(holdings, Eq(list_of(trialCopy1)(trialCopy2)));
}

TEST_F(HoldingServiceTest, Transfer)
{
	holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    string barcode = Holding::ConstructBarcode(CATCH22_CLASSIFICATION, 1);

    holdingService.Transfer(barcode, branch1->Id());

	ASSERT_THAT(FindHolding(barcode).CurrentBranch(), Eq(*branch1));
}

TEST_F(HoldingServiceTest, CheckedOutHoldingUnavailable)
{
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    AddPatronWithId("p1001");

    holdingService.CheckOut("p1001", HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString(), *arbitraryDate);

	Holding retrieved(HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
	holdingService.FindByBarCode(retrieved);
	ASSERT_FALSE(retrieved.IsAvailable());
}

TEST_F(HoldingServiceTest, CheckedOutBooksAddedToPatron)
{
    holdingService.AddAtBranch(branch1->Id(), HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    string barcode = HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString();
    AddPatronWithId("p1001");

    holdingService.CheckOut("p1001", HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString(), *arbitraryDate);

    Holding holding(HoldingBarcode(CATCH22_CLASSIFICATION, 1).AsString());
    ASSERT_THAT(FindPatronWithId("p1001").Holdings(), Eq(list_of(holding)));
}

TEST_F(HoldingServiceTest, CheckInUpdatesHoldingBranch)
{
    HoldingBarcode barcode(THE_TRIAL_CLASSIFICATION, 1);
    CheckOut(barcode, branch1);

    holdingService.CheckIn(barcode.AsString(), *arbitraryDate + date_duration(1), branch2->Id());

    ASSERT_THAT(FindHolding(barcode).CurrentBranch(), Eq(*branch2));
}

TEST_F(HoldingServiceTest, CheckInUpdatesPatronHoldings)
{
    HoldingBarcode barcode(THE_TRIAL_CLASSIFICATION, 1);
    string patronId("p5");
    CheckOut(barcode, branch1, patronId);

    holdingService.CheckIn(barcode.AsString(), *arbitraryDate + date_duration(1), branch2->Id());

    Patron retrieved = FindPatronWithId(patronId);
    ASSERT_THAT(retrieved.Holdings().size(), Eq(0u));
}

TEST_F(HoldingServiceTest, CheckInEarlyDoesNotUpdatePatronFineBalance)
{
    HoldingBarcode barcode(THE_TRIAL_CLASSIFICATION, 1);
    string patronCardNumber("p5");
    CheckOut(barcode, branch1, patronCardNumber);

    holdingService.CheckIn(barcode.AsString(), *arbitraryDate + date_duration(1), branch2->Id());

    ASSERT_THAT(FindPatronWithId(patronCardNumber).FineBalance(), Eq(0));
}

TEST_F(HoldingServiceTest, CheckInLateUpdatesPatronFineBalance)
{
    HoldingBarcode barcode(THE_TRIAL_CLASSIFICATION, 1);
    string patronCardNumber("p5");
    CheckOut(barcode, branch1, patronCardNumber);
    date_duration oneDayLate(Book::BOOK_CHECKOUT_PERIOD + 1);

    holdingService.CheckIn(barcode.AsString(), *arbitraryDate + oneDayLate, branch2->Id());

    ASSERT_THAT(FindPatronWithId(patronCardNumber).FineBalance(), Eq(Book::BOOK_DAILY_FINE));
}

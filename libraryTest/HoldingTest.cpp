#include "gmock/gmock.h"
#include "Holding.h"
#include "Book.h"
#include "ClassificationData.h"
#include "BranchTestData.h"
#include "Branch.h"
#include "InvalidBarcodeException.h"
#include "boost/date_time/gregorian/gregorian_types.hpp"

using namespace boost::gregorian;
using namespace BranchTestData;
using namespace ClassificationData;
using namespace std;
using namespace testing;

class HoldingTest: public Test
{
public:
   Holding* holding;
   static const date ArbitraryDate;
   Branch* arbitraryBranch;
   virtual void SetUp() override
   {
      holding = new Holding(THE_TRIAL_CLASSIFICATION, 1);
      arbitraryBranch = new Branch(EAST_BRANCH);
   }

   virtual void TearDown() override
   {
      delete holding;
      delete arbitraryBranch;
   }

   void VerifyAvailability(const Branch& branch)
   {
      ASSERT_THAT(holding->CurrentBranch(), Eq(branch));
      ASSERT_THAT(holding->IsAvailable(), Eq(branch != Branch::CHECKED_OUT));
   }

    bool IsAvailableAt(Holding* holding, Branch& branch) 
    {
        return holding->CurrentBranch() == branch && 
               holding->IsAvailable();
    }

    void MakeAvailableAtABranch(Holding* holding)
    {
        holding->Transfer(EAST_BRANCH);
    }
};

class AHolding: public HoldingTest
{
};

const date HoldingTest::ArbitraryDate(2013, Jan, 1);

TEST_F(HoldingTest, BarcodeRequiresColon)
{
    ASSERT_THROW(Holding("A"), InvalidBarcodeException);
}

TEST_F(HoldingTest, CanExtractClassificationWhenCreatedWithBarcode)
{
    Holding holding(Holding::ConstructBarcode("A123", 3));

    ASSERT_THAT(holding.Classification(), Eq("A123"));
}

TEST_F(HoldingTest, CanExtractCopyNumberWhenCreatedWithBarcode)
{
    Holding holding(Holding::ConstructBarcode("A234", 5));

    ASSERT_THAT(holding.CopyNumber(), Eq(5));
}

TEST_F(HoldingTest, IsNotAvailableWhenCreated)
{
    Holding holding(Holding::ConstructBarcode("A", 1));

    ASSERT_FALSE(holding.IsAvailable());
}

TEST_F(HoldingTest, CanSpecifyClassificationSeparatelyWhenCreated)
{
    Holding holding("B", 2);

    ASSERT_THAT(holding.Classification(), Eq("B"));
}

TEST_F(HoldingTest, CanSpecifyCopyNumberSeparatelyWhenCreated)
{
    Holding holding("A", 7);

    ASSERT_THAT(holding.CopyNumber(), Eq(7));
}

TEST_F(HoldingTest, IsNotAvailableWhenCreatedWithSeparateClassificationAndCopy)
{
    Holding holding("A", 2);

    ASSERT_FALSE(holding.IsAvailable());
}

TEST_F(HoldingTest, AssignmentCopiesAllMembers)
{
   holding->Transfer(EAST_BRANCH);

   Holding newHolding = *holding;

   ASSERT_THAT(newHolding.Classification(), Eq(THE_TRIAL_CLASSIFICATION));
   ASSERT_THAT(newHolding.CopyNumber(), Eq(holding->CopyNumber()));
   ASSERT_TRUE(IsAvailableAt(&newHolding, EAST_BRANCH));
}

TEST_F(HoldingTest, TransferMakesHoldingAvailableAtBranch)
{
   holding->Transfer(EAST_BRANCH);

    ASSERT_TRUE(IsAvailableAt(holding, EAST_BRANCH));
}

TEST_F(HoldingTest, BarCodeCombinesClassificationAndCopyNumber)
{
   Holding holding(THE_TRIAL_CLASSIFICATION, 5);

    string barcode = holding.Barcode();

    ASSERT_THAT(barcode, Eq(THE_TRIAL_CLASSIFICATION + ":5"));
}

TEST_F(HoldingTest, AreEqualWhenClassificationAndCopyMatch)
{
   Holding copy(holding->Classification(), holding->CopyNumber());

    ASSERT_TRUE(*holding == copy);
}

TEST_F(HoldingTest, AreUnequalWhenCopyDoesNotMatch)
{
   Holding extraCopy(holding->Classification(), holding->CopyNumber() + 1);

    ASSERT_TRUE(*holding != extraCopy);
}

TEST_F(HoldingTest, AreUnequalWhenClassificationDoesNotMatch)
{
   Holding differentBook(holding->Classification() + "X", 1);

    ASSERT_TRUE(*holding != differentBook);
}

TEST_F(HoldingTest, IsLessThanWhenClassificationsAreLessThan)
{
    Holding a("A:1");
    Holding b("B:1");

   ASSERT_TRUE(a < b);
}

TEST_F(HoldingTest, IsNotLessThanWhenClassificationIsNotLessThan)
{
    Holding a("A:1");
    Holding b("B:1");

    ASSERT_TRUE(!(b < a));
}

TEST_F(HoldingTest, IsNotLessThanWhenBarcodesAreEqual)
{
    Holding a("A:1");
    Holding aCopy("A:1");

    ASSERT_FALSE(a < aCopy);
}

TEST_F(HoldingTest, ck)
{
   holding->Transfer(EAST_BRANCH);

   date checkoutOn(2007, Mar, 1);
   holding->CheckOut(checkoutOn);

   ASSERT_FALSE(holding->IsAvailable());

   ASSERT_THAT(holding->LastCheckedOutOn(), Eq(checkoutOn));

   date_duration daysCheckedOut(Book::BOOK_CHECKOUT_PERIOD);
   date expectedDue = checkoutOn + daysCheckedOut;
   ASSERT_THAT(holding->DueDate(), Eq(expectedDue));
}

//START:Availability
TEST_F(HoldingTest, Availability)
{
   holding->Transfer(EAST_BRANCH);
   holding->CheckOut(ArbitraryDate);
   EXPECT_FALSE(holding->IsAvailable());

   date nextDay = ArbitraryDate + date_duration(1);
   holding->CheckIn(nextDay, EAST_BRANCH);
   EXPECT_TRUE(holding->IsAvailable());
}
//END:Availability

//START:AvailabilityFixed
TEST_F(AHolding, IsNotAvailableAfterCheckout)
{
   holding->Transfer(EAST_BRANCH);

   holding->CheckOut(ArbitraryDate);

   EXPECT_THAT(holding->IsAvailable(), Eq(false));
}

TEST_F(AHolding, IsAvailableAfterCheckin)
{
   holding->Transfer(EAST_BRANCH);
   holding->CheckOut(ArbitraryDate);

   holding->CheckIn(ArbitraryDate + date_duration(1), EAST_BRANCH);

   EXPECT_THAT(holding->IsAvailable(), Eq(true));
}
//END:AvailabilityFixed

TEST_F(HoldingTest, UnavailableOnCheckout)
{
    MakeAvailableAtABranch(holding);

    holding->CheckOut(ArbitraryDate);

    ASSERT_FALSE(holding->IsAvailable());
}

TEST_F(HoldingTest, UpdatesCheckoutDateOnCheckout)
{
    MakeAvailableAtABranch(holding);

    holding->CheckOut(ArbitraryDate);

    ASSERT_THAT(holding->LastCheckedOutOn(), Eq(ArbitraryDate));
}

class ACheckedInHolding: public HoldingTest
{
public:
   void SetUp() override {
      HoldingTest::SetUp();
      MakeAvailableAtABranch(holding);
   }
};

//START:UpdatesDateDueOnCheckout
TEST_F(ACheckedInHolding, UpdatesDateDueOnCheckout)
{
//START_HIGHLIGHT
   ASSERT_TRUE(IsAvailableAt(holding, *arbitraryBranch));
//END_HIGHLIGHT
   holding->CheckOut(ArbitraryDate);

   ASSERT_THAT(holding->DueDate(),
      Eq(ArbitraryDate + date_duration(Book::BOOK_CHECKOUT_PERIOD)));
}
//END:UpdatesDateDueOnCheckout

TEST_F(HoldingTest, UpdatesDateDueOnCheckout)
{
    MakeAvailableAtABranch(holding);

    holding->CheckOut(ArbitraryDate);

    ASSERT_THAT(holding->DueDate(), 
        Eq(ArbitraryDate + date_duration(Book::BOOK_CHECKOUT_PERIOD)));
}

TEST_F(HoldingTest, Ckin)
{
   holding->Transfer(EAST_BRANCH);
   date checkoutOn(2007, Mar, 1);
   holding->CheckOut(checkoutOn);

   date checkinOn(2007, Mar, 2);
   Branch branch2("2", "branch2");
   holding->CheckIn(checkinOn, branch2);

   ASSERT_TRUE(IsAvailableAt(holding, branch2));
}

TEST_F(HoldingTest, CheckinMakesBookAvailableAtAnotherBranch)
{
   holding->Transfer(EAST_BRANCH);
   holding->CheckOut(ArbitraryDate);

   holding->CheckIn(
       ArbitraryDate + date_duration(1), WEST_BRANCH);

   ASSERT_TRUE(IsAvailableAt(holding, WEST_BRANCH));
}

class AMovieHolding: public HoldingTest {
public:
   shared_ptr<Holding> movie;

   virtual void SetUp() {
      HoldingTest::SetUp();
      movie = make_shared<Holding>(SEVEN_CLASSIFICATION, 1);
      movie->Transfer(EAST_BRANCH);
   }
};

//START:ImplicitCorrelation
TEST_F(AMovieHolding, AnswersDateDueWhenCheckedOut)
{
   date checkoutDate(2013, Mar, 1);
   movie->CheckOut(checkoutDate);

   date due = movie->DueDate();

   ASSERT_THAT(due, Eq(checkoutDate + date_duration(Book::MOVIE_CHECKOUT_PERIOD)));
}
//END:ImplicitCorrelation

TEST_F(HoldingTest, MoviesDueCheckoutPeriodDaysAfterCheckout)
{
    Holding movie(SEVEN_CLASSIFICATION, 1);
    MakeAvailableAtABranch(&movie);

    movie.CheckOut(ArbitraryDate);

    ASSERT_THAT(movie.DueDate(), 
        Eq(ArbitraryDate + date_duration(Book::MOVIE_CHECKOUT_PERIOD)));
}

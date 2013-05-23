#include "gmock/gmock.h"

#include "Patron.h"
#include "Holding.h"
#include "ClassificationData.h"

#include "boost/assign/list_of.hpp"

#include <string>
#include <sstream>

using namespace std;
using namespace ClassificationData;
using namespace boost::assign;
using namespace testing;

class PatronTest: public Test
{
public:
    Patron* jane;
    Holding* theTrialHolding;
    Holding* catch22Holding;

    virtual void SetUp() override 
    {
        jane = new Patron("Jane", 1);
        theTrialHolding = new Holding(THE_TRIAL_CLASSIFICATION, 1);
        catch22Holding = new Holding(CATCH22_CLASSIFICATION, 1);
    }

    virtual void TearDown() override 
    {
        delete jane;
        delete theTrialHolding;
        delete catch22Holding;
    }
};

TEST_F(PatronTest, PrintableRepresentation)
{
	Patron joe("Joe", 1);
	stringstream stream;

	stream << joe;

	ASSERT_THAT(stream.str(), Eq("Joe (p1)"));
}

TEST_F(PatronTest, CanConstructWithPatronCardNumber)
{
    Patron zelda("", "p32");

    ASSERT_THAT(zelda.Id(), Eq(32));
}

TEST_F(PatronTest, CardNumberIsIdPreprendedWithPrefix)
{
    Patron mary("", 5);

    ASSERT_THAT(mary.CardNumber(), Eq("p5"));
}

TEST_F(PatronTest, CreateInitializesFields)
{
    Patron mary("Mary", 5);

	ASSERT_THAT(mary.Name(), Eq("Mary"));
	ASSERT_THAT(mary.Id(), Eq(5));
	ASSERT_THAT(mary.FineBalance(), Eq(0));
}

TEST_F(PatronTest, CreateDefaultsCardNumber)
{
    Patron john("john");

	ASSERT_THAT(john.Name(), Eq("john"));
	ASSERT_THAT(john.Id(), Eq(0));
	ASSERT_THAT(john.FineBalance(), Eq(0));
}

TEST_F(PatronTest, CreateDefaultsAllFields)
{
    Patron nobody;

	ASSERT_THAT(nobody.Name(), Eq(""));
	ASSERT_THAT(nobody.Id(), Eq(0));
	ASSERT_THAT(nobody.FineBalance(), Eq(0));
}

TEST_F(PatronTest, AddFineUpdatesFineBalance)
{
	jane->AddFine(10);
	ASSERT_THAT(jane->FineBalance(), Eq(10));
}

TEST_F(PatronTest, AddFineAccumulatesIntoBalance)
{
    jane->AddFine(10);
    jane->AddFine(30);

    ASSERT_THAT(jane->FineBalance(), Eq(40));
}

TEST_F(PatronTest, RemitFineSubtractsFromBalance)
{
    jane->AddFine(40);

	jane->Remit(15);
	
    ASSERT_THAT(jane->FineBalance(), Eq(25));
}

TEST_F(PatronTest, HoldingsContainsBorrowedBooks)
{
	jane->Borrow(*theTrialHolding);
	jane->Borrow(*catch22Holding);

    ASSERT_THAT(jane->Holdings(), Eq(list_of(*theTrialHolding)(*catch22Holding)));
}

TEST_F(PatronTest, HoldingsDoesNotContainReturnedBooks)
{
    jane->Borrow(*theTrialHolding);
    jane->Borrow(*catch22Holding);

    jane->ReturnHolding(*theTrialHolding);

    ASSERT_THAT(jane->Holdings(), Eq(list_of(*catch22Holding)));
}

TEST_F(PatronTest, Equality)
{
	Patron patron1("a", 1);
	Patron patron1copy("a", 1);

    ASSERT_TRUE(patron1 == patron1copy);
}

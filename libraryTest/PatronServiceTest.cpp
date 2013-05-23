#include "PatronService.h"
#include "Patron.h"
#include "Holding.h"
#include "ClassificationData.h"
#include "CreditVerifier.h"

#include "gmock/gmock.h"
#include "boost/assign/list_of.hpp"

using namespace ClassificationData;
using namespace boost::assign;
using namespace testing;
using namespace std;

class StubCreditVerifier: public CreditVerifier
{
public:
    MOCK_METHOD1(Verify, bool(const std::string& cardNumber));
};

class PatronServiceTest: public Test
{
public:
    static const string CARD_NUMBER;
    PatronService service;
    Patron* joe;
    Patron* jane;
    StubCreditVerifier verifier;

    virtual void SetUp() override
    {
        PatronService::DeleteAll();
        joe = new Patron("Joe", 1);
        jane = new Patron("Jane", 2);
    }

    virtual void TearDown() override
    {
        delete jane;
        delete joe;
        PatronService::DeleteAll();
    }
};

const string PatronServiceTest::CARD_NUMBER("99998888777776666");

TEST_F(PatronServiceTest, CountInitiallyZero)
{
    ASSERT_THAT(service.PatronCount(), Eq(0));
}

TEST_F(PatronServiceTest, AddUsingAttributes)
{
    EXPECT_CALL(verifier, Verify(CARD_NUMBER))
        .WillOnce(Return(true));
    PatronService service(&verifier);
    service.Add("Suresh", 20, CARD_NUMBER);

    Patron retrieved("", 20);
    service.Find(retrieved);
    ASSERT_THAT(retrieved.Name(), Eq("Suresh"));
}

TEST_F(PatronServiceTest, AddFailsWhenPatronHasBadCredit)
{
    EXPECT_CALL(verifier, Verify(CARD_NUMBER))
        .WillOnce(Return(false));
    PatronService service(&verifier);

    service.Add("Joe", 10, CARD_NUMBER);

    Patron retrieved("", 10);
    ASSERT_FALSE(service.Find(retrieved));
}

TEST_F(PatronServiceTest, AddIncrementsCount)
{
    service.Add(*joe);
    ASSERT_THAT(service.PatronCount(), Eq(1));

    service.Add(*jane);
    ASSERT_THAT(service.PatronCount(), Eq(2));
}

TEST_F(PatronServiceTest, DeleteAllSetsCountToZero)
{
    service.Add(*joe);
    service.Add(*jane);

    PatronService::DeleteAll();

    ASSERT_THAT(service.PatronCount(), Eq(0));
}

TEST_F(PatronServiceTest, DeleteAllRemovesAnyAddedPatrons)
{
    service.Add(*joe);
    service.Add(*jane);

    PatronService::DeleteAll();

    ASSERT_THAT(service.PatronCount(), Eq(0));
}

TEST_F(PatronServiceTest, FindAnswerFalseForNonexistentPatron)
{
    ASSERT_FALSE(service.Find(*joe));
}

TEST_F(PatronServiceTest, FindAnswersTrueForAddedPatron)
{
    service.Add(*joe);

    bool found = service.Find(*joe);

    ASSERT_TRUE(found);
}

TEST_F(PatronServiceTest, FindRetrievesByCardNumber)
{
    service.Add(*joe);
    Patron retrieved("dummy name", joe->Id());

    service.Find(retrieved);

    ASSERT_THAT(retrieved.Name(), Eq(joe->Name()));
}

TEST_F(PatronServiceTest, MembersFullyPopulatedInFoundPatron)
{
    joe->AddFine(20);
    Holding theTrial(THE_TRIAL_CLASSIFICATION, 1);
    joe->Borrow(theTrial);
    service.Add(*joe);
    Patron retrieved("", joe->Id());

    service.Find(retrieved);
    
    ASSERT_THAT(retrieved.FineBalance(), Eq(20));
    ASSERT_THAT(retrieved.Holdings(), Eq(list_of(theTrial)));
}

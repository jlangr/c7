#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ASSERT_CURRENT_STATE.h"

#include <string>

#include "Scanner.h"
#include "ScannerTestData.h"
#include "ScannerState.h"
#include "ScannerStateWaiting.h"
#include "TestScanner.h"
#include "DisplayListener.h"
#include "HoldingService.h"
#include "TestScanner.h"

using namespace std;
using namespace testing;

class MockState: public ScannerState
{
public:
    MockState() : ScannerState(NULL) {}
    MOCK_METHOD0(PressDone, void());
    MOCK_METHOD1(ScanHolding, void(const std::string& barcode));
    MOCK_METHOD1(ScanBranchCard, void(const std::string& barcode));
    MOCK_METHOD1(ScanInventoryCard, void(const std::string& barcode));
    MOCK_METHOD1(ScanPatronCard, void(const std::string& barcode));
};

class ScannerTest : public Test, public TestScanner
{
public:
    Scanner* scanner;
    MockState* state;
    MockDisplayListener* display;

    virtual void SetUp() override
    {
        display = new MockDisplayListener();
        scanner = new Scanner(display, NULL);
        state = new MockState();
        scanner->SetCurrentState(state);
    }

    virtual void TearDown() override
    {
        delete scanner;
        delete display;
    }
};

TEST_F(ScannerTest, IsInWaitingModeByDefault)
{
    Scanner scanner(NULL, NULL);

    ASSERT_CURRENT_STATE<ScannerStateWaiting>(&scanner);
}

TEST_F(ScannerTest, PressDoneDelegatesToAppropriateState)
{
    EXPECT_CALL(*state, PressDone());

    scanner->PressDone();
}

TEST_F(ScannerTest, ScanHoldingDelegatesToAppropriateState)
{
    string barcode("A123:1");
    EXPECT_CALL(*state, ScanHolding(barcode));

    scanner->Scan(barcode);
}

TEST_F(ScannerTest, ScanInventoryCardDelegatesToAppropriateState)
{
    EXPECT_CALL(*state, ScanInventoryCard(Scanner::INVENTORY_CARD_NUMBER));

    scanner->Scan(Scanner::INVENTORY_CARD_NUMBER);
}

TEST_F(ScannerTest, ScanBranchCardDelegatesToAppropriateState)
{
    EXPECT_CALL(*state, ScanBranchCard(ScannerTestData::BRANCH_SOUTH_CARD));

    scanner->Scan(ScannerTestData::BRANCH_SOUTH_CARD);
}

TEST_F(ScannerTest, ScanPatronCardDelegatesToAppropriateState)
{
    EXPECT_CALL(*state, ScanPatronCard(ScannerTestData::PATRON_JANE_CARD));

    scanner->Scan(ScannerTestData::PATRON_JANE_CARD);
}

TEST_F(ScannerTest, ScanUnrecognizedIdDisplaysMessage)
{
    EXPECT_CALL(*display, ShowMessage(Scanner::MSG_INVALID_BARCODE));

    scanner->Scan("234");
}

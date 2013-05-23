#include "gmock/gmock.h"

#include "TestScanner.h"

#include "ScannerTestData.h"
#include "ScannerStateWaiting.h"
#include "ScannerStateCheckin.h"
#include "ASSERT_CURRENT_STATE.h"

#include "Scanner.h"

#include <string>

using namespace std;
using namespace testing;

class ScannerStateWaitingTest : public Test, public TestScanner 
{
public:
    ScannerState* state;
    
    virtual void SetUp() override {
        state = new ScannerStateWaiting(scanner);
    }

    virtual void TearDown() override {
        delete state;
    }
};

TEST_F(ScannerStateWaitingTest, DisplaysWarningMessageWhenDonePressed) 
{
    EXPECT_CALL(*display, ShowMessage(ScannerStateWaiting::MSG_SCAN_BRANCH_ID_FIRST));

    state->PressDone();
}

TEST_F(ScannerStateWaitingTest, DisplaysWarningMessageWhenPatronCardScanned) 
{
    EXPECT_CALL(*display, ShowMessage(ScannerStateWaiting::MSG_SCAN_BRANCH_ID_FIRST));

    state->ScanPatronCard(ScannerTestData::PATRON_JOE_CARD);
}

TEST_F(ScannerStateWaitingTest, DisplaysWarningMessageWhenInventoryCardScanned) 
{
    EXPECT_CALL(*display, ShowMessage(ScannerStateWaiting::MSG_SCAN_BRANCH_ID_FIRST));

    state->ScanInventoryCard(Scanner::INVENTORY_CARD_NUMBER);
}

TEST_F(ScannerStateWaitingTest, DisplaysWarningMessageWhenHoldingScanned) 
{
    EXPECT_CALL(*display, ShowMessage(ScannerStateWaiting::MSG_SCAN_BRANCH_ID_FIRST));

    state->ScanHolding(ScannerTestData::HOLDING_CATCH22_BARCODE);
}

TEST_F(ScannerStateWaitingTest, SetsStateToCheckinWhenBranchScanned) 
{
    state->ScanBranchCard(ScannerTestData::BRANCH_SOUTH_CARD);

    ASSERT_CURRENT_STATE<ScannerStateCheckin>(scanner);
}

TEST_F(ScannerStateWaitingTest, StoresBranchWhenBranchScanned)
{
    state->ScanBranchCard(ScannerTestData::BRANCH_SOUTH_CARD);

    ASSERT_THAT(scanner->BranchId(), Eq(ScannerTestData::BRANCH_SOUTH_CARD));
}

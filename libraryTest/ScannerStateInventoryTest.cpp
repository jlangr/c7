#include "gmock/gmock.h"
#include "ASSERT_CURRENT_STATE.h"

#include "ScannerTestData.h"

#include "TestScanner.h"
#include "Scanner.h"
#include "ScannerStateInventory.h"
#include "ScannerStateCheckin.h"

#include <string>

using namespace std;
using namespace testing;

class ScannerStateInventoryTest : public Test, public TestScanner
{
public:
    ScannerStateInventory* state;

    virtual void SetUp() override {
        state = new ScannerStateInventory(scanner);
        scanner->SetCurrentState(state);
    }
};

TEST_F(ScannerStateInventoryTest, DisplaysWarningMessageWhenInventoryCardScanned)
{
    EXPECT_CALL(*display, ShowMessage(ScannerStateInventory::MSG_COMPLETE_INVENTORY));

    state->ScanInventoryCard(Scanner::INVENTORY_CARD_NUMBER);
}

TEST_F(ScannerStateInventoryTest, ChangesBranchWhenBranchIdScanned)
{
    stringstream expected;
    expected << ScannerStateInventory::MSG_BRANCH_CHANGED << ScannerTestData::BRANCH_SOUTH_CARD;
    EXPECT_CALL(*display, ShowMessage(expected.str()));
    ASSERT_THAT(scanner->BranchId(), Ne(ScannerTestData::BRANCH_SOUTH_CARD));

    state->ScanBranchCard(ScannerTestData::BRANCH_SOUTH_CARD);

    ASSERT_THAT(scanner->BranchId(), Eq(ScannerTestData::BRANCH_SOUTH_CARD));
}

TEST_F(ScannerStateInventoryTest, AddsNewHoldingWhenHoldingScanned) 
{
    scanner->SetBranchId(ScannerTestData::BRANCH_SOUTH_CARD);
    EXPECT_CALL(*display, ShowMessage(_));
    EXPECT_CALL(*HoldingService(),
        AddAtBranch(ScannerTestData::BRANCH_SOUTH_CARD, ScannerTestData::HOLDING_TRIAL_BARCODE));

    state->ScanHolding(ScannerTestData::HOLDING_TRIAL_BARCODE);
}

TEST_F(ScannerStateInventoryTest, DisplaysHoldingAddedMessageWhenHoldingScanned) 
{
    scanner->SetBranchId(ScannerTestData::BRANCH_SOUTH_CARD);
    
    EXPECT_CALL(*display, 
        ShowMessage(ScannerStateInventory::MSG_HOLDING_ADDED + ScannerTestData::HOLDING_TRIAL_BARCODE));
    EXPECT_CALL(*HoldingService(), AddAtBranch(_, _));

    state->ScanHolding(ScannerTestData::HOLDING_TRIAL_BARCODE);
}

TEST_F(ScannerStateInventoryTest, DisplaysErrorWhenDuplicateHoldingScanned)
{
    // TODO new feature!
    // needs HoldingService to throw if holding already exists
}

TEST_F(ScannerStateInventoryTest, DisplayMessageWhenPatronScanned)
{
    EXPECT_CALL(*display, ShowMessage(ScannerStateInventory::MSG_COMPLETE_INVENTORY));

    state->ScanPatronCard(ScannerTestData::PATRON_JANE_CARD);
}

TEST_F(ScannerStateInventoryTest, ChangesStateToCheckinWhenDonePressed)
{
    state->PressDone();
    
    ASSERT_CURRENT_STATE<ScannerStateCheckin>(scanner);
}

#include "gmock/gmock.h"
#include "ASSERT_CURRENT_STATE.h"

#include "ScannerTestData.h"
#include "TestScanner.h"
#include "Scanner.h"
#include "ScannerStateCheckout.h"
#include "ScannerStateCheckin.h"
#include "TimestampSource.h"

#include "boost/date_time/gregorian/gregorian_types.hpp"
#include <string>

using namespace std;
using namespace testing;
using namespace boost::gregorian;

class ScannerStateCheckoutTest : public Test, public TestScanner
{
public:
    ScannerStateCheckout* state;

    virtual void SetUp() override {
        state = new ScannerStateCheckout(scanner);
        scanner->SetCurrentState(state);
    }

    void Checkout(const string& patronCardNumber, const string& holdingBarcode)
    {
        EXPECT_CALL(*HoldingService(), ExistsWithBarcode(_)).WillOnce(Return(true));
        EXPECT_CALL(*HoldingService(), IsAvailable(_)).WillOnce(Return(true));
        EXPECT_CALL(*HoldingService(), 
            CheckOut(patronCardNumber, holdingBarcode, TimestampSource::Now()));
        EXPECT_CALL(*display, ShowMessage(ScannerStateCheckout::MSG_SCANNED_HOLDING));
        state->ScanHolding(holdingBarcode);
    }

    virtual void TearDown() {
    }    
};

TEST_F(ScannerStateCheckoutTest, DisplaysWarningWhenPatronCardScanned)
{
    EXPECT_CALL(*display, ShowMessage(ScannerStateCheckout::MSG_COMPLETE_CHECKOUT_FIRST));

    state->ScanPatronCard(ScannerTestData::PATRON_JOE_CARD);
}

TEST_F(ScannerStateCheckoutTest, DisplaysWarningWhenInventoryCardScanned)
{
    EXPECT_CALL(*display, ShowMessage(ScannerStateCheckout::MSG_COMPLETE_CHECKOUT_FIRST));

    state->ScanInventoryCard(Scanner::INVENTORY_CARD_NUMBER);
}

TEST_F(ScannerStateCheckoutTest, ChecksOutHoldingWhenHoldingBarcodeScanned)
{
    scanner->SetPatronId(ScannerTestData::PATRON_JANE_CARD);
    EXPECT_CALL(*HoldingService(), ExistsWithBarcode(_)).WillOnce(Return(true));
    EXPECT_CALL(*HoldingService(), IsAvailable(_)).WillOnce(Return(true));
    EXPECT_CALL(*HoldingService(), 
        CheckOut(ScannerTestData::PATRON_JANE_CARD, ScannerTestData::HOLDING_CATCH22_BARCODE, TimestampSource::Now()));
    EXPECT_CALL(*display, ShowMessage(ScannerStateCheckout::MSG_SCANNED_HOLDING));

    state->ScanHolding(ScannerTestData::HOLDING_CATCH22_BARCODE);
}

TEST_F(ScannerStateCheckoutTest, DisplayMessageWhenHoldingDoesNotExist)
{
    scanner->SetPatronId(ScannerTestData::PATRON_JANE_CARD);
    EXPECT_CALL(*HoldingService(), ExistsWithBarcode(_)).WillOnce(Return(false));
    EXPECT_CALL(*display, ShowMessage(ScannerStateCheckout::MSG_INVALID_HOLDING_ID));

    state->ScanHolding(ScannerTestData::HOLDING_CATCH22_BARCODE);
}

TEST_F(ScannerStateCheckoutTest, ChangesStateToCheckinWhenDonePressed)
{
    state->PressDone();
    
    ASSERT_CURRENT_STATE<ScannerStateCheckin>(scanner);
}

TEST_F(ScannerStateCheckoutTest, DisplaysMessageWhenBranchIdScanned)
{
    EXPECT_CALL(*display, ShowMessage(ScannerStateCheckout::MSG_COMPLETE_CHECKOUT_FIRST));

    state->ScanBranchCard(ScannerTestData::BRANCH_SOUTH_CARD);
}

TEST_F(ScannerStateCheckoutTest, DisplaysMessageWhenSameHoldingScannedTwice)
{
    InSequence forcesExpectationOrder;
    scanner->SetPatronId(ScannerTestData::PATRON_JANE_CARD);
    Checkout(ScannerTestData::PATRON_JANE_CARD, ScannerTestData::HOLDING_TRIAL_BARCODE);

    EXPECT_CALL(*HoldingService(), ExistsWithBarcode(_)).WillOnce(Return(true));
    EXPECT_CALL(*HoldingService(), IsAvailable(_)).WillOnce(Return(false));
    EXPECT_CALL(*display, ShowMessage(ScannerStateCheckout::MSG_ALREADY_CHECKED_OUT));

    state->ScanHolding(ScannerTestData::HOLDING_TRIAL_BARCODE);
}

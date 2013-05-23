#include "gmock/gmock.h"
#include "ASSERT_CURRENT_STATE.h"

#include "ScannerTestData.h"

#include "TestScanner.h"
#include "Scanner.h"
#include "ScannerStateCheckin.h"
#include "ScannerStateInventory.h"
#include "ScannerStateCheckout.h"
#include "TimestampSource.h"

#include "boost/date_time/gregorian/gregorian_types.hpp"
#include <string>

using namespace std;
using namespace testing;
using namespace boost::gregorian;

class ScannerStateCheckinTest : public Test, public TestScanner
{
public:
    ScannerStateCheckin* state;

    virtual void SetUp() override {
        state = new ScannerStateCheckin(scanner);
        scanner->SetCurrentState(state);
    }
};

TEST_F(ScannerStateCheckinTest, DisplaysWarningMessageWhenCompletePressed)
{
    EXPECT_CALL(*display, ShowMessage(ScannerStateCheckin::MSG_WAITING_FOR_RETURNS));

    state->PressDone();
}

TEST_F(ScannerStateCheckinTest, ChangesStateToInventoryWhenInventoryCardScanned)
{
    state->ScanInventoryCard("");

    ASSERT_CURRENT_STATE<ScannerStateInventory>(scanner);
}

TEST_F(ScannerStateCheckinTest, ChangesStateToCheckoutWhenPatronCardScanned)
{
    state->ScanPatronCard(ScannerTestData::PATRON_JOE_CARD);

    ASSERT_CURRENT_STATE<ScannerStateCheckout>(scanner);
}

TEST_F(ScannerStateCheckinTest, StoresPatronIdWhenScanned)
{
    state->ScanPatronCard(ScannerTestData::PATRON_JOE_CARD);

    ASSERT_THAT(scanner->PatronId(), Eq(ScannerTestData::PATRON_JOE_CARD));
}

TEST_F(ScannerStateCheckinTest, ChangesBranchWhenBranchIdScanned)
{
    scanner->SetBranchId(ScannerTestData::BRANCH_SOUTH_CARD);

    state->ScanBranchCard(ScannerTestData::BRANCH_WEST_CARD);

    ASSERT_THAT(scanner->BranchId(), Eq(ScannerTestData::BRANCH_WEST_CARD));
}

TEST_F(ScannerStateCheckinTest, ChecksInBookWhenBarcodeScanned)
{
    scanner->SetBranchId(ScannerTestData::BRANCH_SOUTH_CARD);
    EXPECT_CALL(*HoldingService(), 
        CheckIn(Eq(ScannerTestData::HOLDING_CATCH22_BARCODE), TimestampSource::Now(), ScannerTestData::BRANCH_SOUTH_CARD));

    state->ScanHolding(ScannerTestData::HOLDING_CATCH22_BARCODE);
}

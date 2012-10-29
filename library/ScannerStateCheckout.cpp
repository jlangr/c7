#include "ScannerStateCheckout.h"

#include <string>

#include "Scanner.h"
#include "ScannerStateCheckin.h"
#include "TimestampSource.h"

using namespace std;

const string ScannerStateCheckout::MSG_COMPLETE_CHECKOUT_FIRST("Please complete checking out the current patron.");
const string ScannerStateCheckout::MSG_SCANNED_HOLDING("Checked out holding.");
const string ScannerStateCheckout::MSG_INVALID_HOLDING_ID("Holding barcode not recognized.");
const string ScannerStateCheckout::MSG_ALREADY_CHECKED_OUT("Holding already checked out.");

ScannerStateCheckout::ScannerStateCheckout(Scanner* scanner)
    : ScannerState(scanner)
{
}

ScannerStateCheckout::~ScannerStateCheckout(void)
{
}

void ScannerStateCheckout::PressDone()
{
    SetState<ScannerStateCheckin>();
}

void ScannerStateCheckout::ScanHolding(const string& barcode)
{
    if (!HoldingService()->ExistsWithBarcode(barcode))
    {
        ShowMessage(ScannerStateCheckout::MSG_INVALID_HOLDING_ID);
        return;
    }

    if (!HoldingService()->IsAvailable(barcode))
    {
        ShowMessage(ScannerStateCheckout::MSG_ALREADY_CHECKED_OUT);
        return;
    }

    HoldingService()->CheckOut(PatronId(), barcode, TimestampSource::Now());
    ShowMessage(ScannerStateCheckout::MSG_SCANNED_HOLDING);
}

void ScannerStateCheckout::ScanBranchCard(const string& barcode)
{    
    ShowMessage(ScannerStateCheckout::MSG_COMPLETE_CHECKOUT_FIRST);
}

void ScannerStateCheckout::ScanPatronCard(const string& patronId)
{
    ShowMessage(ScannerStateCheckout::MSG_COMPLETE_CHECKOUT_FIRST);
}

void ScannerStateCheckout::ScanInventoryCard(const string&)
{
    ShowMessage(ScannerStateCheckout::MSG_COMPLETE_CHECKOUT_FIRST);
}

#include "ScannerStateCheckin.h"
#include "ScannerStateCheckout.h"
#include "ScannerStateInventory.h"
#include "Scanner.h"
#include "HoldingService.h"
#include "TimestampSource.h"

#include "boost/date_time/gregorian/gregorian_types.hpp"
#include <string>
#include <iostream>

using namespace std;
using namespace boost::gregorian;

const string ScannerStateCheckin::MSG_WAITING_FOR_RETURNS("Waiting for returns");

ScannerStateCheckin::ScannerStateCheckin(Scanner* scanner)
    : ScannerState(scanner)
{
}

ScannerStateCheckin::~ScannerStateCheckin(void)
{
}

void ScannerStateCheckin::PressDone()
{
    ShowMessage(ScannerStateCheckin::MSG_WAITING_FOR_RETURNS);
}

void ScannerStateCheckin::ScanHolding(const string& barcode)
{
    HoldingService()->CheckIn(barcode, TimestampSource::Now(), BranchId());
}

void ScannerStateCheckin::ScanBranchCard(const string& branchId)
{
    SetBranchId(branchId);
}

void ScannerStateCheckin::ScanPatronCard(const string& patronId)
{
    SetPatronId(patronId);
    SetState<ScannerStateCheckout>();
}

void ScannerStateCheckin::ScanInventoryCard(const string&)
{
    SetState<ScannerStateInventory>();
}

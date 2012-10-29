#include "ScannerStateInventory.h"

#include "Scanner.h"
#include "ScannerStateCheckin.h"

#include <string>
#include <sstream>

using namespace std;

const string ScannerStateInventory::MSG_COMPLETE_INVENTORY("Please complete inventory first.");
const string ScannerStateInventory::MSG_BRANCH_CHANGED("Branch changed to ");
const string ScannerStateInventory::MSG_HOLDING_ADDED("Added holding ");

ScannerStateInventory::ScannerStateInventory(Scanner* scanner)
    : ScannerState(scanner)
{
}

ScannerStateInventory::~ScannerStateInventory(void)
{
}

void ScannerStateInventory::ScanHolding(const string& barcode)
{
    HoldingService()->AddAtBranch(BranchId(), barcode);

    ShowMessage(ScannerStateInventory::MSG_HOLDING_ADDED + barcode);
}

void ScannerStateInventory::ScanPatronCard(const string&)
{
    ShowMessage(ScannerStateInventory::MSG_COMPLETE_INVENTORY);
}

void ScannerStateInventory::ScanInventoryCard(const string&)
{
    ShowMessage(ScannerStateInventory::MSG_COMPLETE_INVENTORY);
}

void ScannerStateInventory::ScanBranchCard(const string& branchId)
{
    SetBranchId(branchId);
    ShowMessage(ScannerStateInventory::MSG_BRANCH_CHANGED + branchId);
}

void ScannerStateInventory::PressDone()
{
    SetState<ScannerStateCheckin>();
}

#include "ScannerStateWaiting.h"
#include "ScannerStateCheckin.h"
#include "Scanner.h"

#include <string>

using namespace std;

const string ScannerStateWaiting::MSG_SCAN_BRANCH_ID_FIRST("Please scan a branch ID before continuing.");

ScannerStateWaiting::ScannerStateWaiting(Scanner* scanner)
    : ScannerState(scanner)
{
}

ScannerStateWaiting::~ScannerStateWaiting(void)
{
}

void ScannerStateWaiting::ScanBranchCard(const string& branchId) 
{
    SetState<ScannerStateCheckin>();
    SetBranchId(branchId);
}

void ScannerStateWaiting::ScanHolding(const string&)
{
    ShowInvalidOperationMessage();
}

void ScannerStateWaiting::ScanPatronCard(const string&)
{
    ShowInvalidOperationMessage();
}

void ScannerStateWaiting::ScanInventoryCard(const string&)
{
    ShowInvalidOperationMessage();
}

void ScannerStateWaiting::PressDone() 
{
    ShowInvalidOperationMessage();
}

void ScannerStateWaiting::ShowInvalidOperationMessage()
{
    ShowMessage(ScannerStateWaiting::MSG_SCAN_BRANCH_ID_FIRST);
}
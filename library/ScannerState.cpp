#include "ScannerState.h"

#include "HoldingService.h"

using namespace std;

void ScannerState::ShowMessage(const string& message)
{
    mScanner->ShowMessage(message);
}

string ScannerState::BranchId() const
{
    return mScanner->BranchId();
}

void ScannerState::SetBranchId(const string& id)
{
    mScanner->SetBranchId(id);
}

string ScannerState::PatronId() const
{
    return mScanner->PatronId();
}

void ScannerState::SetPatronId(const string& id)
{
    mScanner->SetPatronId(id);
}

service::HoldingService* ScannerState::HoldingService() const
{
    return mScanner->HoldingService();
}

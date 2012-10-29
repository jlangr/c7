#include "Scanner.h"
#include "DisplayListener.h"
#include "ScannerStateCheckin.h"
#include "ScannerStateWaiting.h"
#include "BarcodeUtil.h"
#include "HoldingService.h"

#include <string>
#include <iostream>

using namespace std;

const string Scanner::INVENTORY_CARD_NUMBER("i999999");
const string Scanner::MSG_INVALID_BARCODE("Barcode not recognized.");

Scanner::Scanner(DisplayListener* display, service::HoldingService* holdingService)
    : mDisplay(display)
    , mBranchId("")
    , mCurrentState(0)
    , mPatronId("")
    , mHoldingService(holdingService)
{
    SetCurrentState(new ScannerStateWaiting(this));
}

Scanner::~Scanner()
{
    if (mCurrentState)
    {
        delete mCurrentState;
    }
}

void Scanner::SetCurrentState(ScannerState* state)
{
    if (mCurrentState)
    {
        delete mCurrentState;
    }
    mCurrentState = state;
}

ScannerState* Scanner::CurrentState()
{
    return mCurrentState;
}

void Scanner::Scan(const std::string& barcode) 
{
    if (barcode == INVENTORY_CARD_NUMBER)
        CurrentState()->ScanInventoryCard(INVENTORY_CARD_NUMBER);
    else if (BarcodeUtil::IsBranchId(barcode))
        CurrentState()->ScanBranchCard(barcode);
    else if (BarcodeUtil::IsPatronId(barcode))
        CurrentState()->ScanPatronCard(barcode);
    else if (BarcodeUtil::IsHoldingBarcode(barcode))
        CurrentState()->ScanHolding(barcode);
    else
        ShowMessage(MSG_INVALID_BARCODE);
}

void Scanner::SetBranchId(const string& branchId)
{
    mBranchId = branchId;
}

string Scanner::BranchId()
{
    return mBranchId;
}

void Scanner::SetPatronId(const string& patronId)
{
    mPatronId = patronId;
}

string Scanner::PatronId()
{
    return mPatronId;
}

void Scanner::ShowMessage(const std::string& message)
{
    mDisplay->ShowMessage(message);
}

void Scanner::PressDone() 
{
    CurrentState()->PressDone();
}

service::HoldingService* Scanner::HoldingService()
{
    return mHoldingService;
}

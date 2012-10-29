#pragma once

#include "ScannerState.h"

class Scanner;

#include <string>

class ScannerStateCheckout : public ScannerState
{
public:
    ScannerStateCheckout(Scanner*);
    ~ScannerStateCheckout(void);

    virtual void ScanHolding(const std::string& barcode);
    virtual void ScanBranchCard(const std::string& barcode);
    virtual void ScanInventoryCard(const std::string& barcode);
    virtual void ScanPatronCard(const std::string& barcode);
    virtual void PressDone();
    
    static const std::string MSG_COMPLETE_CHECKOUT_FIRST;
    static const std::string MSG_SCANNED_HOLDING;
    static const std::string MSG_INVALID_HOLDING_ID;
    static const std::string MSG_ALREADY_CHECKED_OUT;
private:
    Scanner* mScanner;
};

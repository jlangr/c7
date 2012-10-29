#pragma once

#include <string>

#include "ScannerState.h"

class Scanner;

class ScannerStateWaiting: public ScannerState
{
public:
    const static std::string MSG_SCAN_BRANCH_ID_FIRST;

    ScannerStateWaiting(Scanner*);
    ~ScannerStateWaiting(void);

    void ScanHolding(const std::string&);
    void ScanPatronCard(const std::string&);
    void ScanInventoryCard(const std::string&);
    void ScanBranchCard(const std::string&);
    void PressDone();

private:
    Scanner* mScanner;

    void ShowInvalidOperationMessage();
};

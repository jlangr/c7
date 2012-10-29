#pragma once

#include <string>

#include "Scanner.h"
#include "ScannerState.h"

class ScannerStateCheckin: public ScannerState
{
public:
    ScannerStateCheckin(Scanner*);
    virtual ~ScannerStateCheckin(void);

    void PressDone();
    void ScanHolding(const std::string&);
    void ScanPatronCard(const std::string&);
    void ScanInventoryCard(const std::string&);
    void ScanBranchCard(const std::string&);

    static const std::string MSG_WAITING_FOR_RETURNS;

private:
    Scanner* mScanner;
};

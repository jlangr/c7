#pragma once

#include <string>

#include "ScannerState.h"

class Scanner;

class ScannerStateInventory: public ScannerState
{
public:
    ScannerStateInventory(Scanner*);
    ~ScannerStateInventory(void);

    void ScanHolding(const std::string&);
    void ScanPatronCard(const std::string&);
    void ScanInventoryCard(const std::string&);
    void ScanBranchCard(const std::string&);
    void PressDone();

    static const std::string MSG_COMPLETE_INVENTORY;
    static const std::string MSG_BRANCH_CHANGED;
    static const std::string MSG_HOLDING_ADDED;
};

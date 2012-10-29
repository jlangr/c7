#pragma once

#include <string>

#include "HoldingService.h"
// TODO class HoldingService; need namespace wrappers
class DisplayListener;
class ScannerState;

class Scanner
{
public:
    const static std::string INVENTORY_CARD_NUMBER;
    const static std::string MSG_INVALID_BARCODE;

    Scanner(DisplayListener*, service::HoldingService*);
    virtual ~Scanner();

	void Scan(const std::string& barcode);
	void PressDone();

    void ShowMessage(const std::string& message);
    ScannerState* CurrentState();
    void SetCurrentState(ScannerState* state);

    std::string BranchId();
    void SetBranchId(const std::string&);
    std::string PatronId();
    void SetPatronId(const std::string&);

    service::HoldingService* HoldingService();

private:
    DisplayListener* mDisplay;
    ScannerState* mCurrentState;
    service::HoldingService* mHoldingService;
    std::string mBranchId;
    std::string mPatronId;
};

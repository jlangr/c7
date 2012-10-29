#pragma once

#include <string>

#include "HoldingService.h"
#include "Scanner.h"

class ScannerState
{
public:
    ScannerState(Scanner* scanner)
        : mScanner(scanner)
    {
    }

    virtual ~ScannerState()
    {
    }

    virtual void ScanHolding(const std::string& barcode)=0;
    virtual void ScanBranchCard(const std::string& barcode)=0;
    virtual void ScanInventoryCard(const std::string& barcode)=0;
    virtual void ScanPatronCard(const std::string& barcode)=0;
    virtual void PressDone()=0;

protected:
    template <typename T> void SetState();

    void ShowMessage(const std::string& message);
    std::string BranchId() const;
    void SetBranchId(const std::string& id);
    std::string PatronId() const;
    void SetPatronId(const std::string& id);
    service::HoldingService* HoldingService() const;

private:
    Scanner* mScanner;
};

template <typename T>
void ScannerState::SetState()
{
    mScanner->SetCurrentState(new T(mScanner));
}

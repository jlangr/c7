#pragma once

#include "gmock/gmock.h"

#include "Scanner.h"
#include "ScannerState.h"
#include "DisplayListener.h"
#include "Holding.h"

#include "boost/date_time/gregorian/gregorian_types.hpp"
#include <string>

class MockDisplayListener: public DisplayListener {
public:
    MOCK_METHOD1(ShowMessage, void(const std::string& text));
};

class MockHoldingService: public service::HoldingService
{
public:
    MOCK_METHOD3(CheckIn, void(const std::string& barCode, boost::gregorian::date, const std::string& branchId));
    MOCK_METHOD3(CheckOut, void(const std::string& patronCardNumber, const std::string& barCode, boost::gregorian::date));
    MOCK_CONST_METHOD1(ExistsWithBarcode, bool(const std::string& barCode));
    MOCK_CONST_METHOD1(IsAvailable, bool(const std::string& barCode));
    MOCK_METHOD2(AddAtBranch, void(const std::string& branchId, const std::string& barcode));
};

// TODO holding service should be an override mock, not required in ctor

// TODO not sure this is a good idea
class TestScanner
{
public:
    Scanner* scanner;
    MockDisplayListener* display;
    MockHoldingService* holdingService;

    TestScanner() {
        display = new MockDisplayListener;
        holdingService = new MockHoldingService;
        scanner = new Scanner(display, holdingService);
    }

    virtual ~TestScanner() {
        delete scanner;
        delete holdingService;
        delete display;
    }

    MockHoldingService* HoldingService()
    {
         return dynamic_cast<MockHoldingService*>(scanner->HoldingService());
    }
};

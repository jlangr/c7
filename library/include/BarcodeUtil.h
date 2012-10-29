#pragma once

#include <string>
#include <ctype.h>

using std::string;

class BarcodeUtil
{
public:
    BarcodeUtil(void) {}
    ~BarcodeUtil(void) {}

    static bool IsBranchId(const string& barcode)
    {
        return toupper(barcode[0]) == 'B';
    }

    static bool IsPatronId(const string& barcode)
    {
        return toupper(barcode[0]) == 'P';
    }

    static bool IsHoldingBarcode(const string& barcode)
    {
        return barcode.find(":") != string::npos;
    }
};

#include "gmock/gmock.h"

#include "InventoryReport.h"
#include "Catalog.h"
#include "ClassificationData.h"
#include "Holding.h"

using namespace std;
using namespace testing;
using namespace ClassificationData;

class StubLibraryOfCongressAPI: public LibraryOfCongressAPI
{
public:
    string GetISBN(const string& classification) 
    {
        return "1234567890";
    }
};

class InventoryReportTest: public Test
{
public:
    Catalog catalog;
    Holding* trial1;
    Holding* trial2;
    Holding* catch22;
    Holding* cash;
    Branch* east;
    Branch* west;

    LibraryOfCongressAPI* api;
    InventoryReport* report;

    void SetUp()
    {
        api = new StubLibraryOfCongressAPI();
        // should this be an auto_ptr
        report = new InventoryReport(&catalog, api);

        trial1 = new Holding(THE_TRIAL_CLASSIFICATION, 1);
        trial2 = new Holding(THE_TRIAL_CLASSIFICATION, 2);
        catch22 = new Holding(CATCH22_CLASSIFICATION, 1);
        cash = new Holding(CASH_AUTOBIO_CLASSIFICATION, 1);

        east = new Branch("b1", "East");
        west = new Branch("b2", "West");

        trial1->Transfer(*east);
        trial2->Transfer(*west);
        catch22->Transfer(*east);

        catalog.Add(*trial1);
        catalog.Add(*trial2);
        catalog.Add(*catch22);
        catalog.Add(*cash);
    }

    void TearDown()
    {
        delete trial1;
        delete trial2;
        delete catch22;
        delete cash;

        delete east;
        delete west;

        delete report;
    }
};

TEST_F(InventoryReportTest, CanGenerate)
{  
    //string output = report->Generate();

    //cout << output << endl;
}

TEST_F(InventoryReportTest, VerySlowTest)
{
    unsigned long i = 2;
    for (long l = 0; l < 99999999; l++)
        if (l % 2 == 1)
            i = l * 3;
    
}

TEST_F(InventoryReportTest, ReportFooterContainsCopyrightNotice)
{
    stringstream stream;

    report->AppendFooter(stream);

    ASSERT_THAT(stream.str(), Eq("Copyright (C) 2012 Langr Software Solutions"));
}

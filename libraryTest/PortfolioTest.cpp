#include "gmock/gmock.h"
#include "Portfolio.h"
#include "StockService.h"

#include <string>

using namespace testing;
using namespace std;

class PortfolioTest: public Test
{
public:
    Portfolio portfolio;
    static const string Symantec;
    static const unsigned int SymantecCurrentValue;
    static const string Apple;

    void ASSERT_SIZE(unsigned int expected)
    {
        ASSERT_THAT(portfolio.Size(), Eq(expected));
        ASSERT_THAT(portfolio.IsEmpty(), Eq(expected == 0));
    }
};

const string PortfolioTest::Symantec("SYMC");
const unsigned int PortfolioTest::SymantecCurrentValue(19);
const string PortfolioTest::Apple("AAPl");

TEST_F(PortfolioTest, IsEmptyOnCreation)
{
    ASSERT_TRUE(portfolio.IsEmpty());
}

TEST_F(PortfolioTest, SizeIsZeroOnCreation)
{
    ASSERT_THAT(portfolio.Size(), Eq(0));
}

TEST_F(PortfolioTest, SizeIsOneAfterFirstPurchase)
{
    int shares(10);
    portfolio.Purchase(Symantec, shares);

    ASSERT_THAT(portfolio.Size(), Eq(1));
}

TEST_F(PortfolioTest, SizeIncrementsOnAdditionalPurchasDifferentSymbol)
{
    portfolio.Purchase(Symantec, 10);
    portfolio.Purchase(Apple, 10);

    ASSERT_THAT(portfolio.Size(), Eq(2));
}

TEST_F(PortfolioTest, SizeDoesNotIncreaseWhenSameSymbolPurchased)
{
    portfolio.Purchase(Symantec, 10);
    portfolio.Purchase(Symantec, 10);

    ASSERT_SIZE(1);
}

TEST_F(PortfolioTest, AnswersSharesForPurchasedSymbol)
{
    portfolio.Purchase(Symantec, 56);

    ASSERT_THAT(portfolio.Shares(Symantec), Eq(56));
}

TEST_F(PortfolioTest, AnswersZeroForSharesOfUnpurchasedSymbol)
{
    ASSERT_THAT(portfolio.Shares(Symantec), Eq(0));
}

TEST_F(PortfolioTest, SeparatesSharesBySymbol)
{
    portfolio.Purchase(Apple, 3);
    portfolio.Purchase(Symantec, 2);

    ASSERT_THAT(portfolio.Shares(Apple), Eq(3));
}

TEST_F(PortfolioTest, CombinesSharesForSameSymbol)
{
    portfolio.Purchase(Symantec, 70);
    portfolio.Purchase(Symantec, 52);

    ASSERT_THAT(portfolio.Shares(Symantec), Eq(122));
}

TEST_F(PortfolioTest, SellingSharesDeductsFromTotal)
{
    portfolio.Purchase(Symantec, 100);
    portfolio.Sell(Symantec, 32);

    ASSERT_THAT(portfolio.Shares(Symantec), Eq(68));
}

TEST_F(PortfolioTest, SellingAllSharesRemovesSymbol)
{
    portfolio.Purchase(Symantec, 100);
    portfolio.Sell(Symantec, 100);

    ASSERT_THAT(portfolio.Size(), Eq(0));
}

TEST_F(PortfolioTest, SellingTooManyThrowsException)
{
    int purchasedShares(5);
    portfolio.Purchase(Symantec, purchasedShares);

    ASSERT_THROW(portfolio.Sell(Symantec, purchasedShares + 1), std::exception);
}

TEST_F(PortfolioTest, ThrowsWhenSymbolIsEmpty)
{
    ASSERT_THROW(portfolio.Purchase("", 5), InvalidSymbolException);
}

TEST_F(PortfolioTest, ThrowsWhenSharesIsZero)
{
    try 
    {
        portfolio.Purchase(Symantec, 0);
        FAIL();
    }
    catch (ZeroNotAllowedException expected)
    {
        ASSERT_THAT(portfolio.Size(), Eq(0));
    }
}

TEST_F(PortfolioTest, ValueIsNothingOnCreation)
{
    ASSERT_THAT(portfolio.Value(), Eq(0));
}

class StubLookupService: public StockService
{
public:
    virtual unsigned int CurrentPrice(const std::string& symbol)
    {
        //ASSERT_THAT(symbol, Eq(PortfolioTest::Symantec));
        if (symbol != PortfolioTest::Symantec)
            throw exception("symbol unexpected");
        return PortfolioTest::SymantecCurrentValue;
    }
};

TEST_F(PortfolioTest, IsWorthValueForSymbolOnPurchaseSingleShare)
{
    StubLookupService stubService;
    portfolio.SetStockLookupService(&stubService);
    portfolio.Purchase(Symantec, 1);

    ASSERT_THAT(portfolio.Value(), Eq(SymantecCurrentValue));
}

#include "gmock/gmock.h"

#include "BarcodeUtil.h"

using namespace testing;

TEST(BarcodeUtilTest, BranchCodesStartWithB)
{
    ASSERT_TRUE(BarcodeUtil::IsBranchId("b123"));
}

TEST(BarcodeUtilTest, IsBranchIdReturnsFalseWhenStartsWithOtherCharacter)
{
    ASSERT_FALSE(BarcodeUtil::IsBranchId("p123"));
}

TEST(BarcodeUtilTest, IgnoresCaseWhenTestingFirstCharacterForBranch)
{
    ASSERT_TRUE(BarcodeUtil::IsBranchId("B123"));
}

TEST(BarcodeUtilTest, PatronIdsStartWithP)
{
    ASSERT_TRUE(BarcodeUtil::IsPatronId("p123"));
}

TEST(BarcodeUtilTest, IsPatronIdReturnsFalseWhenStartsWithOtherCharacter)
{
    ASSERT_FALSE(BarcodeUtil::IsPatronId("b123"));
}

TEST(BarcodeUtilTest, IgnoresCaseWhenTestingFirstCharacterForPatron)
{
    ASSERT_TRUE(BarcodeUtil::IsPatronId("P123"));
}

TEST(BarcodeUtilTest, HoldingBarcodesContainColon)
{
    ASSERT_TRUE(BarcodeUtil::IsHoldingBarcode("qa123:1"));
}

TEST(BarcodeUtilTest, IsHoldingBarcodeReturnsFalseWhenNoColonContained)
{
    ASSERT_FALSE(BarcodeUtil::IsHoldingBarcode("zz123 2"));
}

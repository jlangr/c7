#include "gmock/gmock.h"

#include "BarcodeUtil.h"

using namespace testing;

TEST(BarcodeUtilTest, BranchCodesStartWithB)
{
    ASSERT_THAT(BarcodeUtil::IsBranchId("b123"), Eq(true));
}

TEST(BarcodeUtilTest, IsBranchIdReturnsFalseWhenStartsWithOtherCharacter)
{
    ASSERT_THAT(BarcodeUtil::IsBranchId("p123"), Eq(false));
}

TEST(BarcodeUtilTest, IgnoresCaseWhenTestingFirstCharacterForBranch)
{
    ASSERT_THAT(BarcodeUtil::IsBranchId("B123"), Eq(true));
}

TEST(BarcodeUtilTest, PatronIdsStartWithP)
{
    ASSERT_THAT(BarcodeUtil::IsPatronId("p123"), Eq(true));
}

TEST(BarcodeUtilTest, IsPatronIdReturnsFalseWhenStartsWithOtherCharacter)
{
    ASSERT_THAT(BarcodeUtil::IsPatronId("b123"), Eq(false));
}

TEST(BarcodeUtilTest, IgnoresCaseWhenTestingFirstCharacterForPatron)
{
    ASSERT_THAT(BarcodeUtil::IsPatronId("P123"), Eq(true));
}

TEST(BarcodeUtilTest, HoldingBarcodesContainColon)
{
    ASSERT_THAT(BarcodeUtil::IsHoldingBarcode("qa123:1"), Eq(true));
}

TEST(BarcodeUtilTest, IsHoldingBarcodeReturnsFalseWhenNoColonContained)
{
    ASSERT_THAT(BarcodeUtil::IsHoldingBarcode("zz123 2"), Eq(false));
}

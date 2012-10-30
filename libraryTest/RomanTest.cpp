#include "gtest/gtest.h"
#include "RomanConverter.h"

//START:Roman
TEST(RomanTest, Convert)
{
   RomanConverter converter;
   ASSERT_EQ("I", converter.convert(1));
   ASSERT_EQ("II", converter.convert(2));
   ASSERT_EQ("III", converter.convert(3));
   ASSERT_EQ("IV", converter.convert(4));
   ASSERT_EQ("V", converter.convert(5));
   ASSERT_EQ("VI", converter.convert(6));
   ASSERT_EQ("VII", converter.convert(7));
   ASSERT_EQ("VIII", converter.convert(8));
   ASSERT_EQ("IX", converter.convert(9));
   ASSERT_EQ("X", converter.convert(10));
   ASSERT_EQ("XIV", converter.convert(14));
   ASSERT_EQ("XIX", converter.convert(19));
//END:Roman
   ASSERT_EQ("XL", converter.convert(40));
   ASSERT_EQ("XC", converter.convert(90));
   ASSERT_EQ("CD", converter.convert(400));
   ASSERT_EQ("CM", converter.convert(900));
   ASSERT_EQ("L", converter.convert(50));
   ASSERT_EQ("C", converter.convert(100));
   ASSERT_EQ("D", converter.convert(500));
   ASSERT_EQ("M", converter.convert(1000));
   ASSERT_EQ("MCMLXXXIX", converter.convert(1989));
//START:Roman
  // ...
}
//END:Roman

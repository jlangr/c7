#include "gmock/gmock.h"

#include <string>

#include "ClassificationData.h"
#include "Book.h"

using namespace std;
using namespace testing;
using namespace ClassificationData;

TEST(BookTest, PopulatesAllFieldsOnConstruction)
{
   Book book("title", "author", 1999, "classification");

   ASSERT_THAT(book.Title(), Eq("title"));
   ASSERT_THAT(book.Author(), Eq("author"));
   ASSERT_THAT(book.Year(), Eq(1999));
   ASSERT_THAT(book.Classification(), Eq("classification"));
   ASSERT_THAT(book.Type(), Eq(Book::TYPE_BOOK));
}

TEST(BookTest, DefaultConstructorInitializesFieldsToDefault)
{
   Book book;

   ASSERT_THAT(book.Title(), Eq(""));
   ASSERT_THAT(book.Author(), Eq(""));
   ASSERT_THAT(book.Year(), Eq(0));
   ASSERT_THAT(book.Classification(), Eq(""));
}

TEST(BookTest, AllowsSpecificationOfTypeOnConstruction)
{
   Book book("", "", 0, "", Book::TYPE_MOVIE);

   ASSERT_THAT(book.Type(), Eq(Book::TYPE_MOVIE));
}

TEST(BookTest, IsEqualToCopyConstructedInstance)
{
   Book copy(THE_TRIAL);

   ASSERT_TRUE(THE_TRIAL == copy);
}

TEST(BookTest, IsNotEqualWhenTitleDiffers)
{
   Book book(
   	THE_TRIAL_TITLE + "x", THE_TRIAL_AUTHOR, 
   	THE_TRIAL_YEAR, THE_TRIAL_CLASSIFICATION);
   ASSERT_TRUE(THE_TRIAL != book);
}
    
TEST(BookTest, IsNotEqualWhenAuthorDiffers)
{
   Book book(
   	THE_TRIAL_TITLE, THE_TRIAL_AUTHOR + "x", 
   	THE_TRIAL_YEAR, THE_TRIAL_CLASSIFICATION);
   ASSERT_TRUE(THE_TRIAL != book);
}

TEST(BookTest, IsNotEqualWhenYearDiffers)
{
   Book book(
   	THE_TRIAL_TITLE, THE_TRIAL_AUTHOR, 
   	THE_TRIAL_YEAR + 1, THE_TRIAL_CLASSIFICATION);
   ASSERT_TRUE(THE_TRIAL != book);
}

TEST(BookTest, IsNotEqualWhenClassificationDiffers)
{
   Book book(
   	THE_TRIAL_TITLE, THE_TRIAL_AUTHOR, 
   	THE_TRIAL_YEAR, THE_TRIAL_CLASSIFICATION + "x");
   ASSERT_TRUE(THE_TRIAL != book);
}

TEST(BookTest, IsNotEqualWhenTypeDiffers)
{
   Book book6(
   	THE_TRIAL_TITLE, THE_TRIAL_AUTHOR, 
   	THE_TRIAL_YEAR, THE_TRIAL_CLASSIFICATION, 
   	Book::TYPE_MOVIE);
   ASSERT_TRUE(THE_TRIAL != book6);
}

TEST(BookTest, LessThanBasedOnClassification)
{
    string classificationA("A123");
    string classificationB("B123");
    Book a("", "", 0, classificationA);
    Book b("", "", 0, classificationB);

    ASSERT_TRUE(a < b);
}

#include "gmock/gmock.h"

#include "PatronAccess.h"
#include "Patron.h"
#include "PatronNotFoundException.h"

#include <vector>
#include "boost/assign/std/vector.hpp"
#include "boost/assign/list_of.hpp"

using std::vector;
using namespace boost::assign;
using namespace testing;

class PatronAccessTest: public Test
{
public:
	PatronAccess access;

	virtual void SetUp() override
	{
		PatronAccess::DeleteAll();
	}

	virtual void TearDown() override
	{
		PatronAccess::DeleteAll();
	}
};

TEST_F(PatronAccessTest, SizeIsZeroWithNoPatrons)
{
	ASSERT_THAT(access.Size(), Eq(0));
}

TEST_F(PatronAccessTest, IncrementsSizeOnAdd)
{
	Patron patron1("patron1");
	access.Save(patron1);

	ASSERT_THAT(access.Size(), Eq(1));
}

TEST_F(PatronAccessTest, FindAnswersFalseForNonexistentPatron) 
{
	Patron patron("nobody");
	ASSERT_FALSE(access.Find(patron));
}

TEST_F(PatronAccessTest, FindAnswerTrueForSavedPatron)
{
	Patron patron1("patron1");
	access.Save(patron1);

	Patron retrieved("patron1");
	ASSERT_TRUE(access.Find(retrieved));
}

TEST_F(PatronAccessTest, SaveIsPersistentAcrossAccessInstances)
{
	Patron patron1("patron1");
	access.Save(patron1);

	PatronAccess newAccess;
	Patron retrieved("patron1");
	ASSERT_TRUE(newAccess.Find(retrieved));
}
	
TEST_F(PatronAccessTest, SavePersistsAllAttributes)
{
	std::shared_ptr<Patron> patron(new Patron("patron1"));
	access.Save(*patron);

	PatronAccess newAccess;
	Patron& retrieved = newAccess.FindByName("patron1");

	ASSERT_THAT(retrieved.Name(), Eq(patron->Name()));
	ASSERT_THAT(retrieved.FineBalance(), Eq(patron->FineBalance()));
}

TEST_F(PatronAccessTest, FindByNameThrowsWhenNoNameMatches)
{
	EXPECT_THROW(access.FindByName("nobody"), PatronNotFoundException);
}

TEST_F(PatronAccessTest, FindByNameReturnsChronologicallyFirstMatch)
{
	Patron match1("Langr", 1);
	access.Save(match1);
	Patron mismatch("Long", 2);
	access.Save(mismatch);
	Patron match2("Langr", 3);
	access.Save(match2);

	Patron found = access.FindByName("Langr");

	ASSERT_THAT(found.Id(), Eq(1));
}

TEST_F(PatronAccessTest, UpdatePersistsChanges)
{
	int id = 10;
	Patron patron1("x", id);
	access.Save(patron1);
	int fine = 100;
	patron1.AddFine(fine);

    access.Update(patron1);

	Patron retrieved("", id);
	access.Find(retrieved);
	ASSERT_THAT(retrieved.FineBalance(), Eq(fine));
}

TEST_F(PatronAccessTest, GetAllAnswersAllSavedPatrons)
{
	Patron patron1("a", 10);
	Patron patron2("b", 20);
	access.Save(patron1);
	access.Save(patron2);

	vector<Patron> expected = list_of(patron1)(patron2);	

	ASSERT_THAT(access.GetAll(), ContainerEq(expected));
}

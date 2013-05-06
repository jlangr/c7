#include "gmock/gmock.h"

#include "Branch.h"

#include <sstream>
#include <memory>

using namespace std;
using namespace testing;

TEST(BranchTest, CreateInitializesMembers)
{
    Branch branch("5", "south");

	ASSERT_THAT(branch.Name(), Eq("south"));
    ASSERT_THAT(branch.Id(), Eq("5"));
}

TEST(BranchTest, CloneCopiesAllMembers)
{
    Branch branch("17", "north");
    branch.SetAddress("123 B Street");

    auto_ptr<Branch>clone(branch.Clone());

    ASSERT_THAT(clone->Id(), Eq(branch.Id()));
    ASSERT_THAT(clone->Name(), Eq(branch.Name()));
    ASSERT_THAT(clone->Address(), Eq(branch.Address()));
}

TEST(BranchTest, CreateDefaultsName)
{
    Branch branch("10");

    ASSERT_THAT(branch.Name(), Eq(""));
}

TEST(BranchTest, EqualityAnswersTrueOnIdMatch)
{
    Branch branch1("1", "a");
    Branch branch1copy("1", "b");

    ASSERT_TRUE(branch1 == branch1copy);
}

TEST(BranchTest, InequalityAnswersFalseOnIdMismatch)
{
    Branch branch1("1", "a");
    Branch branch2("2", "a");

    ASSERT_TRUE(branch1 != branch2);
}

TEST(BranchTest, CanLoadFromSerialized)
{
    Branch branch("20", "xyz");
    branch.SetAddress("101 Main St, Laurel, MD");
    auto_ptr<ostringstream> o(new ostringstream(ios::out | ios::binary));
    branch.WriteOn(*o);
    auto_ptr<istream> i(new istringstream(o->str(), ios::in | ios::binary));

    Branch loaded;
    loaded.Load(*i);

    ASSERT_THAT(loaded.Id(), Eq("20"));
    ASSERT_THAT(loaded.Name(), Eq("xyz"));
    ASSERT_THAT(loaded.Address(), Eq("101 Main St, Laurel, MD"));
}

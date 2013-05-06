#include "gmock/gmock.h"

#include "Branch.h"
#include "BranchService.h"

using std::string;
using namespace testing;

class BranchServiceTest: public Test
{
public:
    Branch* eastBranch;
    Branch* westBranch;
    BranchService service;

    virtual void SetUp()
    {
        BranchService::DeleteAll();
        eastBranch = new Branch("1", "east");
        westBranch = new Branch("2", "west");
    }

    virtual void TearDown()
    {
        delete eastBranch;
        delete westBranch;
        BranchService::DeleteAll();
    }
};

TEST_F(BranchServiceTest, AddReturnsGeneratedId)
{
   string id = service.Add("newname", "address");

   ASSERT_THAT(id, Gt("0"));
}

//START:AddClean
TEST_F(BranchServiceTest, AddGeneratesUniqueId)
{
   string id1 = service.Add("name1", "");
   string id2 = service.Add("name2", "");

   ASSERT_THAT(id1, Ne(id2));
}
//END:AddClean

//START:AddThrows
TEST_F(BranchServiceTest, AddThrowsWhenNameNotUnique)
{
   service.Add("samename", "");

   ASSERT_THROW(service.Add("samename", ""), DuplicateBranchNameException);
}
//END:AddThrows

//START:CountZero
TEST_F(BranchServiceTest, CountInitiallyZero)
{
    ASSERT_THAT(service.BranchCount(), Eq(0));
}
//END:CountZero

TEST_F(BranchServiceTest, DeleteAllRemovesAnyAddedBranches)
{
    service.Add(*eastBranch);
    service.Add(*westBranch);

    BranchService::DeleteAll();

    ASSERT_FALSE(service.Find(*eastBranch));
    ASSERT_FALSE(service.Find(*westBranch));
}

TEST_F(BranchServiceTest, DeleteAllSetsCountToZero)
{
    service.Add(*eastBranch);
    service.Add(*westBranch);

    BranchService::DeleteAll();
    
    ASSERT_THAT(service.BranchCount(), Eq(0));
}

TEST_F(BranchServiceTest, FindAnswersFalseForNonexistentBranch)
{
   ASSERT_FALSE(service.Find(*eastBranch));
}

TEST_F(BranchServiceTest, FindAnswersTrueForAddedBranch)
{
    service.Add(*eastBranch);
    
    ASSERT_TRUE(service.Find(*eastBranch));
}

TEST_F(BranchServiceTest, FindRetrievesById)
{
    service.Add(*eastBranch);

    Branch retrieved(eastBranch->Id(), "");
    service.Find(retrieved);

    ASSERT_THAT(retrieved.Name(), Eq(eastBranch->Name()));
}

// START:AddBranchIncrementsCount
// test that adding a branch increments the count
TEST_F(BranchServiceTest, AddBranchIncrementsCount)
{
   // first branch
   service.Add(*eastBranch); // East
   ASSERT_THAT(service.BranchCount(), Eq(1));

   // second branch
   service.Add(*westBranch); // West
   ASSERT_THAT(service.BranchCount(), Eq(2)); // count now 2
}
// END:AddBranchIncrementsCount

TEST_F(BranchServiceTest, PersistsAcrossServiceInstances)
{
    service.Add(*eastBranch);

    BranchService anotherServiceInstance;
    ASSERT_TRUE(anotherServiceInstance.Find(*eastBranch));
    ASSERT_THAT(anotherServiceInstance.BranchCount(), Eq(1));
}

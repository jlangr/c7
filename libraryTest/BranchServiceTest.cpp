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

TEST_F(BranchServiceTest, AddGeneratesUniqueId)
{
    string id1 = service.Add("newname1", "address");
    string id2 = service.Add("newname2", "address");

    ASSERT_THAT(id1, Ne(id2));
}

TEST_F(BranchServiceTest, AddThrowsWhenNameNotUnique)
{
    service.Add("samename", "address1");

    ASSERT_THROW(service.Add("samename", "address2"), DuplicateBranchNameException);
}

TEST_F(BranchServiceTest, CountInitiallyZero)
{
    ASSERT_THAT(service.BranchCount(), Eq(0));
}

TEST_F(BranchServiceTest, DeleteAllRemovesAnyAddedBranches)
{
    service.Add(*eastBranch);
    service.Add(*westBranch);

    BranchService::DeleteAll();

    ASSERT_THAT(service.Find(*eastBranch), Eq(false));
    ASSERT_THAT(service.Find(*westBranch), Eq(false));
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
    ASSERT_THAT(service.Find(*eastBranch), Eq(false));
}

TEST_F(BranchServiceTest, FindAnswersTrueForAddedBranch)
{
    service.Add(*eastBranch);
    
    ASSERT_THAT(service.Find(*eastBranch), Eq(true));
}

TEST_F(BranchServiceTest, FindRetrievesById)
{
    service.Add(*eastBranch);

    Branch retrieved(eastBranch->Id(), "");
    service.Find(retrieved);

    ASSERT_THAT(retrieved.Name(), Eq(eastBranch->Name()));
}

TEST_F(BranchServiceTest, AddBranchIncrementsCount)
{
    service.Add(*eastBranch);
    ASSERT_THAT(service.BranchCount(), Eq(1));

    service.Add(*westBranch);
    ASSERT_THAT(service.BranchCount(), Eq(2));
}

TEST_F(BranchServiceTest, PersistsAcrossServiceInstances)
{
    service.Add(*eastBranch);

    BranchService anotherServiceInstance;
    ASSERT_THAT(anotherServiceInstance.Find(*eastBranch), Eq(true));
    ASSERT_THAT(anotherServiceInstance.BranchCount(), Eq(1));
}

#include "PersistenceTest.h"

#include "gmock/gmock.h"

#include <memory>
#include "KeyedMemoryPersistence.h"
#include "TestSerializable.h"

using namespace std;
using namespace testing;

TEST_P(PersistenceTest, IsEmptyOnCreation)
{
    ASSERT_THAT(persister->Size(), Eq(0u));
}

TEST_P(PersistenceTest, SizeSetToOneOnFirstAdd)
{
    persister->Add(*objectWithId1);

    ASSERT_THAT(persister->Size(), Eq(1u));
}

TEST_P(PersistenceTest, SizeIncrementsWithEachAdd)
{
    persister->Add(*objectWithId1);
    persister->Add(*objectWithId2);

    ASSERT_THAT(persister->Size(), Eq(2u));
}

TEST_P(PersistenceTest, ReturnsNullPointerWhenItemNotFound)
{
    auto_ptr<TestSerializable> found = persister->Get("1");

    TestSerializable* serializable = found.get();

    ASSERT_THAT(serializable, IsNull());
}

TEST_P(PersistenceTest, AddedItemCanBeRetrievedById)
{
    persister->Add(*objectWithId1);

    ASSERT_THAT(*persister->Get("1"), Eq(*objectWithId1));
}

TEST_P(PersistenceTest, GetAnswersNullWhenNoMatchingEntries)
{
    ASSERT_THAT(persister->Get("1").get(), IsNull());
};

TEST_P(PersistenceTest, RetrievedItemIsNewInstance)
{
    persister->Add(*objectWithId1);

    ASSERT_FALSE(objectWithId1 == persister->Get("1").get());
}

TEST_P(PersistenceTest, CanPersistMultipleObjects)
{
    persister->Add(*objectWithId1);
    persister->Add(*objectWithId2);

    ASSERT_THAT(*(persister->Get("1")), Eq(*objectWithId1));
    ASSERT_THAT(*(persister->Get("2")), Eq(*objectWithId2));
}

bool NameMatcher(Serializable& each, const string& name)
{
    return (dynamic_cast<TestSerializable&>(each)).Name() == name;
}

TEST_P(PersistenceTest, MatchesAnswersTrueWithMatchingEntries)
{
    persister->Add(*objectWithId1);
    persister->Add(*objectWithId2);
    string object1Name = objectWithId1->Name();

    bool exists = persister->Matches(NameMatcher, object1Name);

    ASSERT_TRUE(exists);
};

TEST_P(PersistenceTest, MatchesAnswersFalseWhenNoMatchingEntries)
{
    bool exists = persister->Matches(NameMatcher, "don't match anything");

    ASSERT_FALSE(exists);
};

TEST_P(PersistenceTest, FindAllMatching) 
{
    TestSerializable coolidge("Calvin", "1");
    TestSerializable langr("Jeff", "2");
    TestSerializable lynne("Jeff", "3");
    persister->Add(coolidge);
    persister->Add(langr);
    persister->Add(lynne);

    vector<Serializable*> matches;
    persister->FindAllMatching(NameMatcher, "Jeff", matches);

    ASSERT_THAT(matches.size(), Eq(2u));
};

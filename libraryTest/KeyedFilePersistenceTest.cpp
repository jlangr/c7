#include "gmock/gmock.h"

#include "Persistence.h"
#include "PersistenceTest.h"
#include "KeyedFilePersistence.h"
#include "TestSerializable.h"

using namespace testing;

class KeyedFilePersistenceTest: public Test
{
public:
    static Persistence<TestSerializable>* CreatePersistence()
    {
        return new KeyedFilePersistence<TestSerializable>("table");
    }
};

//INSTANTIATE_TEST_CASE_P(KeyedFile, PersistenceTest, ::testing::Values(&KeyedFilePersistenceTest::CreatePersistence));

TEST_F(KeyedFilePersistenceTest, TestOfMyOwn) // note TEST_F, not TEST_P
{
    ASSERT_TRUE(true);
}

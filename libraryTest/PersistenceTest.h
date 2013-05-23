#pragma once

#include "gtest/gtest.h"

#include "KeyedFilePersistence.h"
#include "KeyedMemoryPersistence.h"
#include "TestSerializable.h"

using namespace std;

typedef Persistence<TestSerializable>* (*PersistenceFactoryFunction)();

class PersistenceTest: public ::testing::TestWithParam<PersistenceFactoryFunction> {
public:
    Persistence<TestSerializable>* persister;

    TestSerializable* objectWithId1;
    TestSerializable* objectWithId2;
    TestSerializable* obj;

    void SetUp() override 
    {
        persister = (*InjectedFactoryFunction())();
        persister->Clear();
        objectWithId1 = new TestSerializable("one", "1");
        obj = new TestSerializable("one", "1");
        objectWithId2 = new TestSerializable("two", "2");
    }

    PersistenceFactoryFunction InjectedFactoryFunction() 
    {
        return GetParam();
    }

    void TearDown() override
    {
        persister->Clear();
        delete objectWithId2;
        delete objectWithId1;
        delete obj;
        delete persister;
    }

};

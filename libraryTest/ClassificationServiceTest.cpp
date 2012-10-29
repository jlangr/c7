#include "ClassificationService.h"

#include "gtest/gtest.h"

TEST(ClassificationServiceTest, ThrowsWhenNotFound)
{
    ClassificationService service;
    ASSERT_THROW(service.RetrieveDetails("nonexistent"), ClassificationNotFoundException);
}
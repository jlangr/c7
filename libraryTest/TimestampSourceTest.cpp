#include "gmock/gmock.h"
#include "boost/date_time/gregorian/gregorian_types.hpp"

#include "TimestampSource.h"

using namespace testing;
using namespace boost::gregorian;

class TimestampSourceTest: public Test
{
public:
    static date NEW_YEARS_DAY;
    static date GROUNDHOG_DAY;

    virtual void SetUp() override
    {
        TimestampSource::ClearQueue();
    }
};

date TimestampSourceTest::NEW_YEARS_DAY(2013, Jan, 1);
date TimestampSourceTest::GROUNDHOG_DAY(2013, Feb, 2);

TEST_F(TimestampSourceTest, RetrievesSinglePushedTime)
{
    TimestampSource::QueueNextTime(NEW_YEARS_DAY);

    ASSERT_THAT(TimestampSource::Now(), Eq(NEW_YEARS_DAY));
}

TEST_F(TimestampSourceTest, RetrievesMultiplePushedTimes)
{
    TimestampSource::QueueNextTime(NEW_YEARS_DAY);
    TimestampSource::QueueNextTime(GROUNDHOG_DAY);

    ASSERT_THAT(TimestampSource::Now(), Eq(NEW_YEARS_DAY));
    ASSERT_THAT(TimestampSource::Now(), Eq(GROUNDHOG_DAY));
}

TEST_F(TimestampSourceTest, IsExhaustedWhenNoTimeQueued)
{
    ASSERT_TRUE(TimestampSource::IsExhausted());
}

TEST_F(TimestampSourceTest, IsNotExhaustedWhenTimeQueued)
{
    TimestampSource::QueueNextTime(NEW_YEARS_DAY);

    ASSERT_FALSE(TimestampSource::IsExhausted());
}

TEST_F(TimestampSourceTest, ClearExhaustsQueue)
{
    TimestampSource::QueueNextTime(NEW_YEARS_DAY);

    TimestampSource::ClearQueue();

    ASSERT_TRUE(TimestampSource::IsExhausted());
}

TEST_F(TimestampSourceTest, ReturnsCurrentTimeWhenQueueExhausted)
{    
    date today(day_clock::local_day());
    TimestampSource::QueueNextTime(GROUNDHOG_DAY);
    TimestampSource::Now();

    date retrieved = TimestampSource::Now();

    ASSERT_THAT(retrieved, Eq(today));
}

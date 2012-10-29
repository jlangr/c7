#include "TimestampSource.h"
#include <list>

using namespace boost::gregorian;
using namespace std;

list<date> TimestampSource::mStoredTimes;

TimestampSource::TimestampSource(void)
{
}

TimestampSource::~TimestampSource(void)
{
}

void TimestampSource::ClearQueue()
{
    mStoredTimes.clear();
}

void TimestampSource::QueueNextTime(date& time)
{
    mStoredTimes.push_back(time);
}

date TimestampSource::Now()
{
    if (IsExhausted())
    {
        return date(day_clock::local_day());
    }
    date firstStored = mStoredTimes.front();
    mStoredTimes.pop_front();
    return firstStored;
}

bool TimestampSource::IsExhausted()
{
    return mStoredTimes.empty();
}

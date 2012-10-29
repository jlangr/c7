#pragma once

#include <list>
#include "boost/date_time/gregorian/gregorian_types.hpp"

class TimestampSource
{
public:
    TimestampSource(void);
    ~TimestampSource(void);

    static void ClearQueue();
    static void QueueNextTime(boost::gregorian::date&);
    static boost::gregorian::date Now();
    static bool IsExhausted();

private:
    static std::list<boost::gregorian::date> mStoredTimes;
};

#include "ClassificationService.h"
#include "Book.h"
#include "ClassificationData.h"

using namespace std;
using namespace ClassificationData;

ClassificationService::ClassificationService(void)
{
}

ClassificationService::~ClassificationService(void)
{
}

Book ClassificationService::RetrieveDetails(const string& classification) const
{
    if (classification == ClassificationData::CATCH22_CLASSIFICATION)
        return ClassificationData::CATCH22;
    if (classification == ClassificationData::THE_TRIAL_CLASSIFICATION)
        return ClassificationData::THE_TRIAL;
    if (classification == ClassificationData::CASH_AUTOBIO_CLASSIFICATION)
        return ClassificationData::CASH_AUTOBIO;
    if (classification == ClassificationData::SEVEN_CLASSIFICATION)
        return ClassificationData::SEVEN;
    throw ClassificationNotFoundException(classification);
}

#include "EndpointValidator.h"
#include "MailDestination.h"

EndpointValidator::EndpointValidator(void)
{
}

EndpointValidator::~EndpointValidator(void)
{
}

bool EndpointValidator::IsValid(MailDestination*) const
{
    return false;
}
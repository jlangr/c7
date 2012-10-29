#pragma once

class MailDestination;

class EndpointValidator
{
public:
    EndpointValidator(void);
    virtual ~EndpointValidator(void);

    virtual bool IsValid(MailDestination*) const;
};

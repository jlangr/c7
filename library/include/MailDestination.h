#pragma once

#include "Endpoint.h"
#include "MailMessage.h"

#include <string>
#include <stdexcept>

class MailDestinationException: public std::runtime_error
{
public:
    MailDestinationException()
       : std::runtime_error("unable to connect to LDAP server") {}
};

class MailDestination
{
public:
    MailDestination(const std::string& address);
    ~MailDestination(void);

    static Endpoint CreateEndpoint(const std::string& address);
    static Endpoint CreateEndpoint(MailDestination& destination);
    static void Send(MailMessage&);

    std::string Address() const;

private:
    std::string mAddress;
};

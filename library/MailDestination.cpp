#include "MailDestination.h"
#include "Transport.h"
#include "Endpoint.h"

using namespace std;

MailDestination::MailDestination(const string& address)
    : mAddress(address)
{
}

MailDestination::~MailDestination(void)
{
}

/* static */ Endpoint MailDestination::CreateEndpoint(const string& address)
{
    throw MailDestinationException();
}

/* static */ Endpoint MailDestination::CreateEndpoint(MailDestination& destination)
{
    return CreateEndpoint(destination.Address());
}

void MailDestination::Send(MailMessage& message)
{
    Transport::Send(message);
}

string MailDestination::Address() const
{
    return mAddress;
}

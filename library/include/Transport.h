#pragma once

#include "MailMessage.h"

class Transport
{
public:
	Transport(void);
	~Transport(void);

	static void Send(const MailMessage&);
};

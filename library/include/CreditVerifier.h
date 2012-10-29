#pragma once

#include <string>

class CreditVerifier
{
public:
	virtual bool Verify(const std::string& cardNumber)=0;
};

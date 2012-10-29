#pragma once

#include "MailMessage.h"
#include "MailDestination.h"
#include "Transport.h"
#include "Report.h"
#include "EndpointValidator.h"

#include <vector>
#include <exception>

class ReportMailerException: public std::exception
{
public:
    ReportMailerException(const std::string& message)
        : mMessage(message) {}
    virtual ~ReportMailerException() throw() {}

    const char* what() const throw()
    {
        return mMessage.c_str();
    }

private:
    std::string mMessage;
};

template <typename ENDPOINT_VALIDATOR>
class ReportMailerImpl
{
public:
    ReportMailerImpl(std::vector<MailDestination> destinations)
        : mDestinations(destinations)
    {
        if (mDestinations.empty())
        {
            throw ReportMailerException("destinations required");
        }
        ENDPOINT_VALIDATOR validator;
        for (std::vector<MailDestination>::iterator it = mDestinations.begin();
            it != mDestinations.end();
            it++)
        {
            MailDestination destination = *it;
            // verify all endpoints
            if (!validator.IsValid(&destination))
            {
                throw ReportMailerException("invalid endpoint");
            }
        }
    }

	virtual ~ReportMailerImpl() 
    {
	}

    void MailReport(Report* report) 
    {
        for (std::vector<MailDestination>::iterator it = mDestinations.begin();
            it != mDestinations.end();
            it++)
        {
            MailDestination destination = *it;
            std::string toAddress = destination.Address();
            MailMessage message = ConstructMailMessage(toAddress, report);
            Send(message);
        }
    }

    virtual void Send(MailMessage& message)
    {
        MailDestination::Send(message);
    }
    
    MailMessage ConstructMailMessage(const std::string& toAddress, Report* report) const
    {
        return ReportMailerImpl::ConstructMailMessageTo(toAddress, report);
    }

    static MailMessage ConstructMailMessageTo(const std::string& toAddress, Report* report)
    {
        std::string content = report->Text();
        std::string subject = report->Name();

        MailMessage message(
            subject, 
            content, 
            "Joe@example.com");
        
        message.AddRecipient(toAddress);

        return message;
    }

private:
    std::vector<MailDestination> mDestinations;
};

typedef ReportMailerImpl<EndpointValidator> ReportMailer;

#include "MailMessage.h"

#include <string>

using namespace std;

MailMessage::MailMessage(const std::string& subject,
		const std::string& content,
        const std::string& from)
		: mSubject(subject)
		, mContent(content)
        , mFrom(from)
{
}

MailMessage::~MailMessage(void)
{
}

void MailMessage::AddRecipient(const std::string& to)
{
    mTo.push_back(to);
}

string MailMessage::Subject() const
{
    return mSubject;
}

string MailMessage::Content() const
{
    return mContent;
}

vector<string> MailMessage::To() const
{
    return mTo;
}

string MailMessage::From() const
{
    return mFrom;
}

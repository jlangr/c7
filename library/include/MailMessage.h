#pragma once

#include <string>
#include <vector>

class MailMessage
{
public:
	MailMessage(const std::string& subject, const std::string& content, const std::string& from);
	~MailMessage(void);

    void AddRecipient(const std::string& toAddress);
    std::string Subject() const;
    std::string Content() const;
    std::vector<std::string> To() const;
    std::string From() const;

private:
	std::string mSubject;
	std::string mContent;
    std::vector<std::string> mTo;
    std::string mFrom;
};

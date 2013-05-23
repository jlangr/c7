#include "gmock/gmock.h"

#include "MailMessage.h"
#include "Report.h"
#include "ReportMailer.h"
#include "MailDestination.h"

#include <vector>

using namespace std;
using namespace testing;

class StubEndpointValidator: public EndpointValidator
{
public:
    virtual bool IsValid(MailDestination*) const
    {
        return true;
    }
};

class ReportMailerTest: public Test
{
public:
    vector<MailDestination> destinations;

    virtual void SetUp() override
    {
        MailDestination destination("address");
        destinations.push_back(destination);
    }
};

// TODO: updated MailMessage.cpp/h

TEST_F(ReportMailerTest, Create)
{
    ASSERT_NO_THROW(ReportMailerImpl<StubEndpointValidator> mailer(destinations));
}

class StubReport: public Report
{
public:
    string Text() const { return "text"; }
    string Name() const { return "name"; }
};

TEST_F(ReportMailerTest, CanConstructMailMessage)
{
    string toAddress = "a@b.com";
    StubReport report;

    MailMessage message = ReportMailer::ConstructMailMessageTo(toAddress, &report);

    ASSERT_THAT(message.To()[0], Eq(toAddress));
    ASSERT_THAT(message.Subject(), Eq(report.Name()));
    ASSERT_THAT(message.Content(), Eq(report.Text()));
    ASSERT_THAT(message.From(), Eq("Joe@example.com"));
}

class SpyReportMailer: public ReportMailerImpl<StubEndpointValidator>
{
public:
    SpyReportMailer(vector<MailDestination> destinations)
        : ReportMailerImpl(destinations)
    {
    }
    MOCK_METHOD1(Send, void(MailMessage&));
};

TEST_F(ReportMailerTest, MailReport)
{
    SpyReportMailer mailer(destinations);
    StubReport report;

    //A<MailMessage>()
    EXPECT_CALL(mailer, Send(_));

    mailer.MailReport(&report);
}

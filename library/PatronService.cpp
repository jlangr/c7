#include "PatronService.h"
#include "PatronAccess.h"
#include "Patron.h"
#include "CreditVerifier.h"

#include <vector>

using namespace std;

PatronService::PatronService(CreditVerifier* verifier)
    : mCreditVerifier(verifier) // TODO: leak!
{
}

PatronService::~PatronService(void)
{
}

/* static */ void PatronService::DeleteAll()
{
    PatronAccess::DeleteAll();
}

void PatronService::SetCreditVerifier(CreditVerifier* verifier)
{
    mCreditVerifier = verifier;
}

vector<Patron> PatronService::GetAll() const
{
    return mPatronAccess.GetAll();
}

void PatronService::Add(const string& name, int id, const string& cardNumber)
{
    if (!mCreditVerifier->Verify(cardNumber))
        return;

    Patron patron(name, id);
    Add(patron);
}

void PatronService::Add(const Patron& patron)
{
    mPatronAccess.Save(patron);
}

void PatronService::Update(const Patron& patron)
{
    mPatronAccess.Update(patron);
}

bool PatronService::Find(Patron& patron) const
{
    return mPatronAccess.Find(patron);
}

short PatronService::PatronCount() const
{
    return mPatronAccess.Size();
}

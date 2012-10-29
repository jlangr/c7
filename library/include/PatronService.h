#pragma once

#include "PatronAccess.h"
#include "CreditVerifier.h"

#include <vector>

class AlwaysTrueCreditVerifier: public CreditVerifier
{
public:
    virtual bool Verify(const std::string& cardNumber)
    {
        return true;
    }
};

class Patron;

class PatronService
{
public:
    PatronService(
        CreditVerifier* verifier=new AlwaysTrueCreditVerifier());
    virtual ~PatronService(void);

    void Add(const std::string& name, int id, const std::string& cardNumber="");
    void Add(const Patron& patron);
    bool Find(Patron& patron) const;
    short PatronCount() const;
    void Update(const Patron& patron);
    std::vector<Patron> GetAll() const; // bad!

    static void DeleteAll();
    void SetCreditVerifier(CreditVerifier* verifier);
private:
    CreditVerifier* mCreditVerifier;
    PatronAccess mPatronAccess;
};

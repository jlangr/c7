#pragma once

#include "Serializable.h"

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <iostream>

class Branch: public Serializable
{
public:
    Branch();
    Branch(const std::string& id, const std::string& name="");
	virtual ~Branch();

    static Branch CHECKED_OUT;

    virtual void WriteOn(std::ostream& output);
    virtual void Load(std::istream& input);
    virtual Branch* Clone() const;
    std::string Id() const;
    std::string Name() const;
	void SetAddress(const std::string&);
	std::string Address() const;
    bool operator==(const Branch& other) const;
	bool operator!=(const Branch& other) const;

private:
    std::string mId;
    std::string mName;
	std::string mAddress;
    friend std::istream& operator>>(std::istream &, Branch&);
    friend std::ostream& operator<<(std::ostream &, Branch&);
};

typedef std::vector<Branch>::iterator BranchIterator;

std::istream& operator >>(std::istream&, Branch&);
std::ostream& operator <<(std::ostream&, Branch&);

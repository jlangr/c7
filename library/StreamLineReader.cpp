#include "StreamLineReader.h"

#include <sstream>
#include <istream>
#include <string>
#include <iostream>

using namespace std;

StreamLineReader::StreamLineReader(istream* input)
    : mInput(input)
{
}

StreamLineReader::~StreamLineReader(void)
{
}

void StreamLineReader::Read(string& stringMember)
{
    getline(*mInput, stringMember);
}

void StreamLineReader::Read(unsigned int& intMember)
{
    string text;
    getline(*mInput, text);
    if ( !(istringstream(text) >> intMember)) 
    {
        intMember = 0;
    }
}

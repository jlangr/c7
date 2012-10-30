#include "RomanConverter.h"
#include <string>

using namespace std;

RomanConverter::RomanConverter(void)
{
}

std::string RomanConverter::convert(int arabic)
{
	int arabics[] = 
      { 1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1 };
	string romans[] = 
      { "M", "CM", "D", "CD", "C", "XC",  "L", 
        "XL","X", "IX", "V", "IV", "I" };
	int numberOfDigits = sizeof(arabics) / sizeof(int);

	string roman("");
	for (int i = 0; i < numberOfDigits; i++) 
		while (arabic >= arabics[i]){
			roman += romans[i];
			arabic = arabic - arabics[i];
		}
	return roman; 
}

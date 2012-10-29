#include "Book.h"

using std::string;

const int Book::BOOK_CHECKOUT_PERIOD(21);
const int Book::MOVIE_CHECKOUT_PERIOD(7);
const int Book::NEW_RELEASE_CHECKOUT_PERIOD(3);

Book::Book()
	: mYear(0)
{
}

Book::Book(
		const string& title, 
		const string& author, 
		const unsigned short year, 
		const string& classification, 
		unsigned short type) 
	: mTitle(title)
	, mAuthor(author)
	, mYear(year)
	, mClassification(classification)
	, mType(type)
{
}

Book::~Book()
{
}

Book& Book::operator=(const Book& rhs)
{
	if (this == &rhs) return *this;

	mTitle = rhs.mTitle;
	mAuthor = rhs.mAuthor;
	mYear = rhs.mYear;
	mClassification = rhs.mClassification;
	mType = rhs.mType;

	return *this;
}

bool Book::operator==(const Book& other) const {
	return mTitle == other.mTitle &&
		mAuthor == other.mAuthor &&
		mYear == other.mYear &&
		mClassification == other.mClassification &&
		mType == other.mType;
}

bool Book::operator!=(const Book& other) const {
	return !(*this == other);
}

bool Book::operator<(const Book& rhs) const {
	return mClassification < rhs.mClassification;
}

unsigned short Book::Year() const {
	return mYear;
}

string Book::Classification() const {
	return mClassification;
}

string Book::Title() const {
	return mTitle;
}

string Book::Author() const {
	return mAuthor;
}

unsigned short Book::Type() const {
	return mType;
}

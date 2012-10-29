#pragma once

#include <string>

class Book
{
public:
	Book();
	Book(
		const std::string& title, 
		const std::string& author, 
		const unsigned short year, 
		const std::string& classification, 
		unsigned short type = Book::TYPE_BOOK);
	virtual ~Book();

	Book& operator=(const Book&);

	bool operator==(const Book& other) const;
	bool operator!=(const Book& other) const;
	bool operator<(const Book& other) const;

	enum BookType { TYPE_BOOK, TYPE_MOVIE, TYPE_NEW_RELEASE };
	static const short BOOK_DAILY_FINE = 10;
	static const short MOVIE_DAILY_FINE = 100;
	static const short NEW_RELEASE_DAILY_FINE = 20;

	std::string Title() const;
	std::string Author() const;
	unsigned short Year() const;
	std::string Classification() const;
	unsigned short Type() const;

	static const int BOOK_CHECKOUT_PERIOD;
	static const int MOVIE_CHECKOUT_PERIOD;
	static const int NEW_RELEASE_CHECKOUT_PERIOD;

private:
	std::string mTitle;
	std::string mAuthor;
	std::string mClassification;
	unsigned short mYear;
	unsigned short mType;
};

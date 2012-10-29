#pragma once

#include "Catalog.h"
#include "Book.h"

#include <string>
#include <stdexcept>
#include <sstream>

class LibraryOfCongressAPI
{
public:
    virtual std::string GetISBN(const std::string& classification) 
    {
        throw std::runtime_error("connection currently unavailable, please try later");
    }
};

struct Record
{
    Record(Book book, const std::string& branchName, LibraryOfCongressAPI* isbnApi)
        : Title(book.Title())
        , Branch(branchName)
        , Author(book.Author())
        , Year(book.Year())
    {
        Isbn = isbnApi->GetISBN(book.Classification());
    }

    bool operator<(const Record& rhs) const
    {
        if (Author != rhs.Author) 
            return Author < rhs.Author;
        if (Year != rhs.Year) 
            return Year < rhs.Year;
        return Title < rhs.Title;
    }

    std::string Title;
    std::string Branch;
    std::string Author;
    unsigned short Year;
    std::string Isbn;
};

class InventoryReport
{
public:
    static const std::string NEWLINE;
    static const unsigned int SPACING;
    static const unsigned int TITLE_LENGTH;
    static const unsigned int BRANCH_LENGTH;
    static const unsigned int AUTHOR_LENGTH;
    static const unsigned int YEAR_LENGTH;
    static const unsigned int ISBN_LENGTH;
    
    InventoryReport(Catalog*,
        LibraryOfCongressAPI* isbnApi = new LibraryOfCongressAPI());

    ~InventoryReport(void);
    std::string Generate();
    void AppendHeader(std::stringstream& buffer);
    void AppendColumnHeaders(std::stringstream& buffer);
    void Append(std::stringstream& buffer, Record& record);
    std::string Underlines(unsigned int count, unsigned int spacing);
    std::string Pad(unsigned int totalLength, const std::string& text="");
    void AppendFooter(std::stringstream& buffer);
private:
    Catalog* mCatalog;
    LibraryOfCongressAPI* mIsbnApi;
};

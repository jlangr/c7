#include "InventoryReport.h"

#include "Catalog.h"
#include "Holding.h"
#include "ClassificationService.h"
#include "Book.h"
#include "Branch.h"

#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

const unsigned int InventoryReport::SPACING(2);
const unsigned int InventoryReport::TITLE_LENGTH(24);
const unsigned int InventoryReport::BRANCH_LENGTH(16);
const unsigned int InventoryReport::AUTHOR_LENGTH(24);
const unsigned int InventoryReport::YEAR_LENGTH(6);
const unsigned int InventoryReport::ISBN_LENGTH(10);

InventoryReport::InventoryReport(Catalog* catalog, LibraryOfCongressAPI* isbnApi)
    : mCatalog(catalog)
    , mIsbnApi(isbnApi)
{
}

InventoryReport::~InventoryReport(void)
{
    delete mIsbnApi;
}

std::string InventoryReport::Generate() {
    ClassificationService classificationService;

    vector<Record> records;

    vector<Holding>* holdings = Catalog::Holdings();
    for (vector<Holding>::iterator it = holdings->begin();
        it != holdings->end();
        it++)
    {
        Holding holding = *it;
        Book book = classificationService.RetrieveDetails(holding.Classification());
        if (book.Type() == Book::TYPE_BOOK)
        {
            Record record(book, holding.CurrentBranch().Name(), mIsbnApi);
            records.push_back(record);
        }
    }


    sort(records.begin(), records.end());

    stringstream buffer;
    AppendHeader(buffer);
    AppendColumnHeaders(buffer);
    for (vector<Record>::iterator it = records.begin(); it != records.end();it++)
         Append(buffer, *it);
    AppendFooter(buffer);
    return buffer.str();
}

void InventoryReport::AppendHeader(stringstream& buffer) {
    buffer << "Inventory" << endl;
    buffer << endl;
}

void InventoryReport::AppendFooter(stringstream& buffer)
{
    buffer << "Copyright (C) 2014 Langr Software Solutions";
}

void InventoryReport::AppendColumnHeaders(stringstream& buffer) {
    buffer << Pad(TITLE_LENGTH + SPACING, "Title")
        << Pad(BRANCH_LENGTH + SPACING, "Branch")
        << Pad(AUTHOR_LENGTH + SPACING, "Author")
        << Pad(YEAR_LENGTH + SPACING, "Year")
        << Pad(ISBN_LENGTH + SPACING, "ISBN");
    buffer << endl;

    buffer << Underlines(TITLE_LENGTH, SPACING);
    buffer << Underlines(BRANCH_LENGTH, SPACING);
    buffer << Underlines(AUTHOR_LENGTH, SPACING);
    buffer << Underlines(YEAR_LENGTH, SPACING);
    buffer << Underlines(ISBN_LENGTH, SPACING);
    buffer << endl;
}

void InventoryReport::Append(stringstream& buffer, Record& record) {
    buffer << Pad(TITLE_LENGTH, record.Title);
    buffer << Pad(SPACING);
    buffer << Pad(BRANCH_LENGTH, record.Branch);
    buffer << Pad(SPACING);
    buffer << Pad(AUTHOR_LENGTH, record.Author);
    buffer << Pad(SPACING);
    stringstream year;
    year << record.Year;
    buffer << Pad(YEAR_LENGTH, year.str());
    buffer << Pad(SPACING);
    buffer << Pad(ISBN_LENGTH, record.Isbn);
    buffer << endl;
}

// TODO: copy over these better implementations to template project!
string InventoryReport::Pad(unsigned int totalLength, const string& text) {
    //stringstream buffer(text, ios::out | ios::ate);
    return text + string(totalLength - text.length(), ' ');
}

string InventoryReport::Underlines(unsigned int count, unsigned int spacing) {
    return Pad(count + spacing, string(count, '-'));
}

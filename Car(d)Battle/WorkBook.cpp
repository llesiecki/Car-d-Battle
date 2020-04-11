#include "stdafx.h"
#include "WorkBook.h"

WorkBook::WorkBook(const wchar_t* filename)
{
    good = false;
    book = xlCreateBook();
    if (book->load(filename))
    {
        sheet = book->getSheet(0);
        if (sheet)
            good = true;
    }
}

WorkBook::~WorkBook()
{
    book->release();
}

std::string WorkBook::cell_to_string(int row, int col)
{
    CellType cellType = sheet->cellType(row, col);
    switch (cellType)
    {
    case CELLTYPE_EMPTY: return std::string();
    case CELLTYPE_NUMBER: return std::to_string(sheet->readNum(row, col));
    case CELLTYPE_STRING:
    {
        std::wstring wstr = std::wstring(sheet->readStr(row, col));
        return std::string(wstr.begin(), wstr.end());
    }
    case CELLTYPE_BOOLEAN:
    {
        bool b = sheet->readBool(row, col);
        return std::string(b ? "true" : "false");
    }
    case CELLTYPE_BLANK: return std::string();
    case CELLTYPE_ERROR: return std::string();
    }
    return std::string();
}

bool WorkBook::to_Cards(Cards& cards)
{
    cards.cards_properties.clear();
    cards.field_names.clear();
    cards.img_paths.clear();
    for (int col = 1; col < sheet->lastCol(); ++col)
        cards.field_names.push_back(cell_to_string(0, col));

    for (int row = 1; row < sheet->lastRow(); ++row)
        cards.img_paths.push_back(cell_to_string(row, 0));

    for (int row = 1; row < sheet->lastRow(); ++row)
    {
        cards.cards_properties.push_back(std::vector< std::string >());
        for (int col = 1; col < sheet->lastCol(); ++col)
            cards.cards_properties.back().push_back(cell_to_string(row, col));
    }
    return true;
}
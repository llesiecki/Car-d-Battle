#include "stdafx.h"
#include "WorkBook.h"
#include <locale>
#include <codecvt>
#include <algorithm>

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
    std::string str;
    switch (cellType)
    {
    case CELLTYPE_NUMBER:
        str = std::to_string(sheet->readNum(row, col));
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        str.erase(str.find_last_not_of('.') + 1, std::string::npos);
        return str;
    case CELLTYPE_STRING: return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(sheet->readStr(row, col));
    case CELLTYPE_BOOLEAN: return std::string(sheet->readBool(row, col) ? "true" : "false");
    default: return std::string();
    }
}

bool WorkBook::to_Cards(Cards& cards)
{
    cards.cards_properties.clear();
    cards.field_names.clear();
    cards.img_paths.clear();
    for (int col = 1; col <= sheet->lastCol(); ++col)
        cards.field_names.push_back(cell_to_string(0, col));

    for (int row = 1; row < sheet->lastRow(); ++row)
        cards.img_paths.push_back(cell_to_string(row, 0));

    for (int row = 1; row <= sheet->lastRow(); ++row)
    {
        cards.cards_properties.push_back(std::vector< std::string >());
        for (int col = 1; col <= sheet->lastCol(); ++col)
            cards.cards_properties.back().push_back(cell_to_string(row, col));
    }

    for (unsigned int pos = 0; pos < cards.field_names.size(); pos++)//add ".0" in acceleration if it's an integer
    {
        if (cards.field_names[pos] == "Acceleration")
        {
            for (auto& properties : cards.cards_properties)
            {
                if (properties[pos].find(".") == std::string::npos)
                    properties[pos].append(".0");
            }
            break;
        }
    }
    return true;
}
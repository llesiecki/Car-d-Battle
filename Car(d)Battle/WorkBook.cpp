#include "WorkBook.h"

WorkBook::WorkBook(const wchar_t* filename)
    :book(nullptr), sheet(nullptr)
{
    good = false;
    book = xlCreateBook();
    if (book->load(filename))
    {
        sheet = book->getSheet(0);
        if (sheet)
            good = true;
    }
    if (!good)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        throw std::runtime_error(
            "Couldn't open file: " +
            converter.to_bytes(filename) +
            " for reading.\n"
        );
    }
}

WorkBook::~WorkBook()
{
    if(good)
        book->release();
}

std::string WorkBook::cell_to_string(int row, int col)
{
    libxl::CellType cellType = sheet->cellType(row, col);
    std::string str;
    switch (cellType)
    {
    case libxl::CELLTYPE_NUMBER:
        str = std::to_string(sheet->readNum(row, col));
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        str.erase(str.find_last_not_of('.') + 1, std::string::npos);
        return str;
    case libxl::CELLTYPE_STRING: return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(sheet->readStr(row, col));
    case libxl::CELLTYPE_BOOLEAN: return std::string(sheet->readBool(row, col) ? "true" : "false");
    default: return std::string();
    }
}

int WorkBook::get_col_num()
{
    return sheet->lastCol();
}

int WorkBook::get_row_num()
{
    return sheet->lastRow();
}

#include "WorkBook.h"

std::string WorkBook::wstring_to_string(const std::wstring& wstring)
{
    std::string str;

    if (!wstring.empty())
    {
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstring.data(), wstring.size(), NULL, 0, NULL, NULL);
        str = std::string(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstring.data(), wstring.size(), str.data(), size_needed, NULL, NULL);
    }
    
    return str;
}

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
        

        throw std::runtime_error(
            "Couldn't open file: " +
            wstring_to_string(filename) +
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
    case libxl::CELLTYPE_STRING: return wstring_to_string(sheet->readStr(row, col));
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

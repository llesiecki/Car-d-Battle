#pragma once
#include "stdafx.h"
#include <codecvt>

class WorkBook
{
	libxl::Book *book;
    libxl::Sheet* sheet;
    bool good;
    std::string wstring_to_string(const std::wstring&);
public:
    WorkBook(const wchar_t* filename);
    ~WorkBook();
    std::string cell_to_string(int row, int col);
    int get_col_num();
    int get_row_num();
};

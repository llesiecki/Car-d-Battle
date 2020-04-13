#pragma once
#include "stdafx.h"

using namespace libxl;

class WorkBook
{
	Book *book;
    Sheet* sheet;
    bool good;
public:
    WorkBook(const wchar_t* filename);
    ~WorkBook();
    std::string cell_to_string(int row, int col);
    bool to_Cards(Cards& cards);
};


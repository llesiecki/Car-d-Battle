#pragma once

#include "stdafx.h"

struct Cards
{
	std::vector< std::string > field_names, img_paths;
	std::vector< std::vector<std::string> > cards_properties;

	void print();
};

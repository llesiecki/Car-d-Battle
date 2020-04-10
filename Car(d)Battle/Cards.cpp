#include "stdafx.h"
#include "Cards.h"

void Cards::print()
{
	for (std::string field : field_names)
	{
		std::cout << field << '\t';
	}
	std::cout << std::endl;
	for (unsigned int i = 0; i < cards_properties.size(); i++)
	{
		//std::cout << "path : " << img_paths[i] << '\t';
		for (unsigned int j = 0; j < cards_properties[i].size(); j++)
			std::cout << cards_properties[i][j] << '\t';
	}
	std::cout << '\n';
}
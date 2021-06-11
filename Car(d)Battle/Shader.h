#pragma once
#include "stdafx.h"

class Shader
{
	int id;
public:
	Shader();
	Shader(std::string);
	void load(std::string);
	void enable();
	void disable();

};


#pragma once
#include "stdafx.h"

class Shader
{
	int id;
public:
	Shader();
	Shader(const std::string &);
	void load(const std::string &);
	void enable();
	void disable();

};


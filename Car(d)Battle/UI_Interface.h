#pragma once
class UI_Interface
{
public:
	virtual void request_category() = 0;
	virtual void button_callback(const std::string&) = 0;
};

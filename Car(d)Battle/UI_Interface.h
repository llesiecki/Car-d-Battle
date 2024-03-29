#pragma once
class UI_Interface
{
public:
	virtual void request_category() = 0;
	virtual void button_callback(const std::string&) = 0;
	virtual int create_battle() = 0;
	virtual int join_battle(int, const std::string&) = 0;
	virtual int start_battle() = 0;
	virtual std::string register_user(const std::string&, const std::string&) = 0;
	virtual std::string login_user(const std::string&, const std::string&) = 0;
	virtual void start_game(int) = 0;
	virtual void exit_game() = 0;
	virtual void set_pause(bool) = 0;
	virtual void leave_battle() = 0;
};

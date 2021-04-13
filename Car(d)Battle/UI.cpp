#include "UI.h"
#include <regex>

UI::UI(Game & game)
	:network_client(80, 512), game(game), battle_id(0)
{
	network_client.start();
}


UI::~UI()
{
}

std::map<std::string, std::string> UI::get_server_response(const std::string& script, const std::map<std::string, std::string>& query = {})
{
	std::map<std::string, std::string> data;

	if (!network_client.is_ready())
	{
		network_client.start();
		if (!network_client.is_ready())
		{
			std::cerr << "Failed to start client\n";
			return data;
		}
	}

	std::string request = "/" + script + ".php?";
	for (auto element : query)
		request.append(element.first + "=" + element.second + "&");
	request.pop_back();//remove last, unnecessary "&" from the request or "?", when no variables provided
	network_client.http_get(std::string("card-battle.cba.pl"), request);
	std::string& response = network_client.get_response();
	std::regex response_capture("([A-Za-z0-9_]+):([A-Za-z0-9_]*)");
	std::sregex_iterator it(response.begin(), response.end(), response_capture);
	std::sregex_iterator end;
	while (it != end)
	{
		if (it->size() != 3)
			continue;
		data[(*it)[1]] = (*it)[2];
		++it;
	}
	if (data["error"].empty())
		return data;
	else
		throw std::runtime_error(data["error"]);
}

int UI::create_battle()
{
	auto response = get_server_response("create_battle");
	return std::stoi(response["battle_id"]);
}

int UI::join_battle(int id, const std::string & passwd = "")
{
	std::map<std::string, std::string> query;
	query["battle_id"] = std::to_string(id);
	query["user_token"] = user_token;
	query["passwd"] = passwd;
	auto response = get_server_response("join_battle", query);
	return std::stoi(response["battle_id"]);
}

int UI::start_battle()
{
	std::map<std::string, std::string> query;
	query["battle_id"] = std::to_string(battle_id);
	query["user_token"] = user_token;
	auto response = get_server_response("start_battle", query);
	return std::stoi(response["battle_id"]);
}

std::string UI::register_user(const std::string & username, const std::string & passwd)
{
	std::map<std::string, std::string> query;
	query["username"] = username;
	query["passwd"] = passwd;
	auto response = get_server_response("register_user", query);
	return response["user_token"];
}

std::string UI::login_user(const std::string & username, const std::string & passwd)
{
	std::map<std::string, std::string> query;
	query["username"] = username;
	query["passwd"] = passwd;
	auto response = get_server_response("login_user", query);
	return response["user_token"];
}

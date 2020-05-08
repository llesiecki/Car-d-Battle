#include "UI.h"

UI::UI()
	:network_client(80, 512)
{
	network_client.start();

}


UI::~UI()
{
}

int UI::create_battle()
{
	if (!network_client.is_ready())
	{
		network_client.start();
		if (!network_client.is_ready())
		{
			std::cerr << "Failed to start client\n";
			return 0;
		}
	}
	network_client.http_get(std::string("card-battle.cba.pl"), std::string("/server.php?create_battle=1"));
	std::string& response = network_client.get_response();
	size_t data_pos = response.find("Car(d) Battle battle_id:");
	if (data_pos == std::string::npos)
	{
		std::cerr << "Failed to recive battle data\n";
		return 0;
	}
	data_pos += strlen("Car(d) Battle battle_id:");
	battle_id = std::stoi(response.substr(data_pos));
	return battle_id;
}

int UI::join_battle(int id)
{
	if (!network_client.is_ready())
	{
		network_client.start();
		if (!network_client.is_ready())
		{
			std::cerr << "Failed to start client\n";
			return 0;
		}
	}
	network_client.http_get(std::string("card-battle.cba.pl"), std::string("/server.php?join_battle=") + std::to_string(id));
	std::string& response = network_client.get_response();
	size_t data_pos = response.find("Car(d) Battle join_battle:");
	if (data_pos == std::string::npos)
	{
		std::cerr << "Failed to recive battle data\n";
		return 0;
	}
	data_pos += strlen("Car(d) Battle join_battle:");
	battle_id = std::stoi(response.substr(data_pos));
	return battle_id;
}

int UI::start_battle()
{
	if (!network_client.is_ready())
	{
		network_client.start();
		if (!network_client.is_ready())
		{
			std::cerr << "Failed to start client\n";
			return 0;
		}
	}
	network_client.http_get(std::string("card-battle.cba.pl"), std::string("/server.php?start_battle=") + std::to_string(battle_id));
	std::string& response = network_client.get_response();
	size_t data_pos = response.find("Car(d) Battle start_battle:");
	if (data_pos == std::string::npos)
	{
		std::cerr << "Failed to recive battle data\n";
		return 0;
	}
	data_pos += strlen("Car(d) Battle start_battle:");
	battle_id = std::stoi(response.substr(data_pos));
	return battle_id;
}

bool UI::register_user(std::string& username, std::string& passwd)
{
	if (!network_client.is_ready())
	{
		network_client.start();
		if (!network_client.is_ready())
		{
			std::cerr << "Failed to start client\n";
			return false;
		}
	}
	network_client.http_get(std::string("card-battle.cba.pl"), std::string("/server.php?register_user=1&username=") + username + "&passwd=" + passwd);
	std::string& response = network_client.get_response();
	size_t data_pos = response.find("Car(d) Battle register_user:");
	if (data_pos == std::string::npos)
	{
		std::cerr << "Failed to recive battle data\n";
		return false;
	}
	data_pos += strlen("Car(d) Battle register_user:");
	return std::stoi(response.substr(data_pos));
}

bool UI::login_user(std::string& username, std::string& passwd)
{
	if (!network_client.is_ready())
	{
		network_client.start();
		if (!network_client.is_ready())
		{
			std::cerr << "Failed to start client\n";
			return false;
		}
	}
	network_client.http_get(std::string("card-battle.cba.pl"), std::string("/server.php?login_user=1&username=") + username + "&passwd=" + passwd);
	std::string& response = network_client.get_response();
	size_t data_pos = response.find("Car(d) Battle login_user:");
	if (data_pos == std::string::npos)
	{
		std::cerr << "Failed to recive battle data\n";
		return false;
	}
	data_pos += strlen("Car(d) Battle login_user:");
	user_id = std::stoi(response.substr(data_pos));
	return user_id;
}

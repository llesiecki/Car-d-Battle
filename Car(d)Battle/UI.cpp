#include "UI.h"
#include <regex>
#include <thread>
#include "Game.h"

UI::UI(Game & game)
	:network_client(80, 512), game(game), battle_id(0), kb(FindWindowW(NULL, LGAME_NAME))
{
	pause = false;
	network_client.start();
	std::string keys_to_observe = "QWERTYUIOPASDFGHJKLZXCVBNM0123456789";
	keys_to_observe.push_back(VK_LBUTTON);
	keys_to_observe.push_back(VK_BACK);
	keys_to_observe.push_back(VK_RETURN);
	keys_to_observe.push_back(VK_SHIFT);
	keys_to_observe.push_back(VK_ESCAPE);
	keys_to_observe.push_back(VK_DELETE);
	for (char vk_code : keys_to_observe)
	{
		std::function<void(BYTE)> fp = std::bind(&UI::on_press_handler, this, std::_Ph<1>());
		int handler_id = kb.observe_key(vk_code, fp, Key_action::on_press);
		handlers.push_back(handler_id);
	}
}

UI::~UI()
{
}

void UI::on_press_handler(BYTE key)
{
	if (key == VK_ESCAPE)
	{
		pause ^= true;
		game.set_pause(pause);
	}
}

std::map<std::string, std::string> UI::get_server_response(const std::string& script, const std::map<std::string, std::string>& query = {})
{
	std::map<std::string, std::string> data;
	std::string request = "/" + script + ".php?";
	for (auto element : query)
		request.append(element.first + "=" + element.second + "&");
	request.pop_back();//remove last, unnecessary "&" from the request or "?", when no variables provided
	unsigned int timeout = GetTickCount() + 10 * 1000;
	while (!network_client.http_get(std::string(SERVER_ADDRESS), request))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (GetTickCount() > timeout)
		{
			throw std::runtime_error("Connection with the serwer was lost.\n");
			break;
		}
	}
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
	std::map<std::string, std::string> query;
	query["user_token"] = user_token;
	auto response = get_server_response("create_battle", query);
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

int UI::get_current_category()
{
	std::map<std::string, std::string> query;
	query["battle_id"] = std::to_string(battle_id);
	query["user_token"] = user_token;
	auto response = get_server_response("get_current_category", query);
	return std::stoi(response["current_category"]);
}

void UI::render()
{
	if (pause)
		render_pause_menu();
}

void UI::render_pause_menu()
{
	//glPushMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(100, 0, 100, 0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glDisable(GL_LIGHTING);
	//glColor3f(0.7f, 1.0f, 0.0f);
	//Text2D menu_title = { 55.0f, 10.0f, "Pause Menu" };
	//menu_title.render();
	//glEnable(GL_LIGHTING);
	//glPopMatrix();



	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//{
	//	glLoadIdentity();
	//	gluOrtho2D(100, 0, 100, 0);
	//	glMatrixMode(GL_MODELVIEW);
	//	glPushMatrix();
	//	{
	//		glLoadIdentity();
	//		glDisable(GL_LIGHTING);
	//		glDisable(GL_CULL_FACE);
	//		glColor3f(0.7f, 1.0f, 0.0f);
	//		Text2D menu_title = { 55.0f, 10.0f, "Pause Menu", GLUT_BITMAP_TIMES_ROMAN_24 };
	//		menu_title.render();
	//		glEnable(GL_LIGHTING);
	//		glEnable(GL_CULL_FACE);

	//		glMatrixMode(GL_PROJECTION);
	//		glPushMatrix();
	//		//glTranslatef(-0.5, -0.5, 0.0f);

	//		glLoadIdentity();
	//		glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
	//		glBegin(GL_QUADS);
	//		glVertex2f(0.0f, 0.0f);
	//		glVertex2f(0.1f, 0.0f);
	//		glVertex2f(0.1f, 0.1f);
	//		glVertex2f(0.0, 0.1f);
	//		glEnd();
	//		glPopMatrix();
	//	}
	//	glPopMatrix();
	//	glMatrixMode(GL_MODELVIEW);
	//}
	//glPopMatrix();


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{



		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -0.2f);
		glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.1f, 0.0f);
		glVertex2f(0.1f, 0.1f);
		glVertex2f(0.0, 0.1f);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		gluOrtho2D(100, 0, 100, 0);
		glDisable(GL_CULL_FACE);
		glColor3f(0.7f, 1.0f, 0.0f);
		Text2D menu_title = { 55.0f, 9.0f, "Pause Menu", GLUT_BITMAP_TIMES_ROMAN_24 };
		menu_title.render();
		glEnable(GL_CULL_FACE);
		glPopMatrix();

	}
	glPopMatrix();
}

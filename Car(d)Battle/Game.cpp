#include "Game.h"

Game::Game()
	:cards(L"carlist.xls"), ui(*this)
{
	srand(static_cast<unsigned int>(time(NULL)));
	state = Game_state::no_action;
	for (int i = 0; i < 24; i++)
		random_translation_vec.push_back({
			(static_cast<float>(rand()) / RAND_MAX - 0.5f) / 9999.0f,
			(static_cast<float>(rand()) / RAND_MAX - 0.5f) / 9999.0f });
	players_num = -1;
	choosen_category = -1;
	winner = nullptr;
	loser = nullptr;
	network_client = nullptr;
	pause = false;
}

void Game::clean()
{
	for (Card& card : central_stack)
	{
		card.reset_coords();
		card.highlight_row(-1);
	}

	central_stack.clear();
	player_card.clear();
	player_stack.clear();
	kill_threads = true;
	std::this_thread::sleep_for(400ms);
	for (auto& th : threads)
	{
		if (th.joinable())
			th.join();
	}
	threads.clear();
	kill_threads = false;
	if (winner)
		delete[] winner;
	if (loser)
		delete[] loser;
	if (network_client)
		delete network_client;

}

Game::~Game()
{
	clean();
}

bool Game::thread_sleep_ms(unsigned int delay)
{
	while (delay >= 200)
	{
		if (kill_threads)
			return true;
		std::this_thread::sleep_for(200ms);
		delay -= 200;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	return false;
}

void Game::set_cursor_pos(int x, int y)
{
	cursor_pos = { x, y };
}

void Game::set_screen_size(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	screen_size = { width, height };
	gluPerspective(60.0f, static_cast<float>(width) / height, 0.01f, 10.0f);
}

void Game::load()
{
	cards.create_lists();
	cards.load_textures();
	scene.load();
}

void Game::draw_cards_stack(std::vector<Card>& cards_vec)
{
	glPushMatrix();
	lock.lock();//other threads may attempt to modify cards_vec during execution of this member function
	for (unsigned int i = 0; i < cards_vec.size(); i++)
	{
		glTranslatef(random_translation_vec[i].first, 0.005f, random_translation_vec[i].second);
		cards_vec[i].draw();
	}
	lock.unlock();
	glPopMatrix();
}

void Game::move_cards(const Card_translation transltion_type[])
{
	float* height_diff = new float[players_num];

	for (int player_num = 0; player_num < players_num; player_num++)
	{
		if (transltion_type[player_num] == Card_translation::no_translation)
			continue;

		float initial_height = player_stack[player_num].size() * 0.005f;
		float dest_height = player_card[player_num].size() * 0.005f + 0.005f;
		height_diff[player_num] = dest_height - initial_height;

		if (transltion_type[player_num] == Card_translation::without_flip)
			continue;

		player_stack[player_num].back().rot = Vec3(0.0f, 0.0f, 1.0f);
	}

	const int iterations_max = 60;
	for (int i = 0; i < iterations_max; i++)
	{
		if(thread_sleep_ms(17))
			return;
		for (int player_num = 0; player_num < players_num; player_num++)
		{
			if (transltion_type[player_num] == Card_translation::no_translation)
				continue;

			Card& card = player_stack[player_num].back();

			card.pos.x -= 1.0f / iterations_max;
			card.pos.z += -0.5f / iterations_max;

			if (height_diff[player_num] > 0 && i < iterations_max / 4)//can raise here
				card.pos.y += height_diff[player_num] / 0.25f / iterations_max;

			if (height_diff[player_num] < 0 && i > iterations_max * 3 / 4)//can descend here
				card.pos.y += height_diff[player_num] / 0.25f / iterations_max;

			if (transltion_type[player_num] == Card_translation::without_flip)
				continue;

			card.angle += 180.0f / iterations_max;
			card.pos.y = sqrtf(-static_cast<float>(i * 2) / iterations_max * (static_cast<float>(i * 2) / iterations_max - 2)) * CARD_WIDTH / 2;//rotate around left edge of the card
		}
	}

	lock.lock();
	for (int player_num = 0; player_num < players_num; player_num++)
	{
		if (transltion_type[player_num] == Card_translation::no_translation)
			continue;
		player_stack[player_num].back().reset_coords();
		player_card[player_num].push_back(player_stack[player_num].back());
		player_stack[player_num].pop_back();
		if (transltion_type[player_num] == Card_translation::without_flip)
			continue;
		player_card[player_num].back().invert = !player_card[player_num].back().invert;
	}
	lock.unlock();
	delete[] height_diff;
}

void Game::flip_cards(const bool flip[])
{
	for (int player_num = 0; player_num < players_num; player_num++)
		if (flip[player_num])
			player_card[player_num].back().rot = Vec3(0.0f, 0.0f, 1.0f);

	const int iterations_max = 60;

	for (int i = 0; i < iterations_max; i++)
	{
		if (thread_sleep_ms(17))
			return;
		for (int player_num = 0; player_num < players_num; player_num++)
		{
			if (!flip[player_num])
				continue;
			Card& card = player_card[player_num].back();
			card.angle += 180.0f / iterations_max;
			card.pos.y = sqrtf(-static_cast<float>(i * 2) / iterations_max * (static_cast<float>(i * 2) / iterations_max - 2)) * CARD_WIDTH / 2;//rotate around left edge of the card
		}
	}

	for (int player_num = 0; player_num < players_num; player_num++)
		if (flip[player_num])
		{
			Card& card = player_card[player_num].back();
			card.invert = !card.invert;
			card.reset_coords();
		}
}

void Game::distribute_cards()
{
	int card_num = 0;
	while (!central_stack.empty())
	{
		Card& card = central_stack.back();
		card.reset_coords();
		const int iterations_max = 10;
		float current_height = central_stack.size() * 0.005f;
		float destination_height = player_stack[card_num % players_num].size() * 0.005f + 0.005f;
		float height_difference = destination_height - current_height;

		for (int i = 0; i < iterations_max; i++)
		{
			if (thread_sleep_ms(17))
				return;
			switch (card_num%players_num)
			{
			case 0:
				card.pos.x += 1.0f / iterations_max;
				card.pos.z += 1.5f / iterations_max;
				break;
			case 1:
				card.rot = Vec3(0.0f, 1.0f, 0.0f);
				card.pos.x += -1.9f / iterations_max;
				card.pos.z += 1.3f / iterations_max;
				card.angle += 90.0f / iterations_max;
				break;
			case 2:
				card.pos.x += -1.0f / iterations_max;
				card.pos.z += -1.2f / iterations_max;
				break;
			case 3:
				card.rot = Vec3(0.0f, 1.0f, 0.0f);
				card.pos.x += 1.9f / iterations_max;
				card.pos.z += -0.7f / iterations_max;
				card.angle += 90.0f / iterations_max;
				break;
			}

			if (height_difference > 0 && i < iterations_max/4)//can raise here
				card.pos.y += height_difference / 0.25f / iterations_max;

			if (height_difference < 0 && i > iterations_max * 3 / 4)//can descend here
				card.pos.y += height_difference / 0.25f / iterations_max;
		}
		lock.lock();
		card.reset_coords();
		player_stack[card_num % players_num].push_back(card);
		central_stack.pop_back();
		lock.unlock();
		card_num++;
	}
	current_player = rand() % players_num;
	current_player = 0;//TEST!
	state = Game_state::cards_to_players;
}

void Game::cards_to_players()
{
	Card_translation* translation = new Card_translation[players_num];
	translation[0] = Card_translation::with_flip;

	for (int player_num = 1; player_num < players_num; player_num++)
		translation[player_num] = Card_translation::without_flip;

	for (int player_num = 0; player_num < players_num; player_num++)
	{
		if (player_stack[player_num].empty())
		{
			loser[player_num] = true;
			winner[player_num] = false;
			translation[player_num] = Card_translation::no_translation;
		}
	}

	move_cards(translation);
	delete[] translation;
	state = Game_state::choose_category;
}

void Game::choose_category()
{
	Text3D text;
	text.pos = Vec3(-0.6f, 0.01f, 0.2f);
	text.angle = -60.0f;
	text.rot = Vec3(1.0f, 0.0f, 0.0f);
	text.scale = 0.001f;
	text.text = current_player == 0 ? "Choose a category..." : "Waiting for opponent...";
	text.line_width = 7.5f;
	lock.lock();
	unsigned int text_id = texts.size();
	texts.push_back(text);
	lock.unlock();
	choosen_category = -1;
	if (current_player == 0)
	{
		std::pair<std::pair<float, float>, std::pair<float, float>> categories[6] =//pair of upper left and lower right corners of each category
		{
			{{-0.1377f, 0.7395f}, {0.1387f, 0.7734f}},//Cylinders
			{{-0.1397f, 0.77345f}, {0.1407f, 0.8084f}},//Capacity
			{{-0.1407f, 0.8084f}, {0.1427f, 0.8453f}},//Power
			{{-0.1427f, 0.8453f}, {0.1437f, 0.8832f}},//Torque
			{{-0.1447f, 0.8832f}, {0.1457f, 0.9212f}},//Top speed
			{{-0.1457f, 0.9212f}, {0.1467f, 0.9591f}}//Acceleration
		};

		while (choosen_category == -1)
		{
			if (thread_sleep_ms(17))
				return;

			for (int i = 0; i < 6; i++)//6 categories
			{
				if (
					(cursor_pos.x * 1.0f - screen_size.x / 2.0f) / screen_size.y > categories[i].first.first
					&& (cursor_pos.x * 1.0f - screen_size.x / 2.0f) / screen_size.y < categories[i].second.first
					&& cursor_pos.y * 1.0f / screen_size.y > categories[i].first.second
					&& cursor_pos.y * 1.0f / screen_size.y < categories[i].second.second)
				{
					player_card[0].back().highlight_row(i);
					if (GetAsyncKeyState(1) & (1 << 15))//1 is virtual key-code of LMB, 15 is shift to the highest bit of short
						choosen_category = i;
					break;
				}
				player_card[0].back().highlight_row(-1);//no highlit
			}
		}
		player_card[0].back().highlight_row(-1);//no highlit
	}
	else//wait for server response
	{
		while (choosen_category == -1)
		{
			choosen_category = ui.get_current_category();
			if (choosen_category != -1)
			{
				if (thread_sleep_ms(500))
					return;
			}
		}
	}

	lock.lock();
	texts.erase(texts.begin() + text_id);
	lock.unlock();
	state = Game_state::show_players_cards;
}

void Game::show_players_cards()
{
	bool* flip = new bool[players_num];
	flip[0] = false;

	for (int player_num = 1; player_num < players_num; player_num++)
		flip[player_num] = true;

	for (int player_num = 0; player_num < players_num; player_num++)
		if (loser[player_num])
			flip[player_num] = false;

	flip_cards(flip);
	delete[] flip;
	state = Game_state::compare_by_choosen_category;
}

void Game::compare_by_choosen_category()
{
	for (int player_num = 0; player_num < players_num; player_num++)
	{
		if (loser[player_num])
			continue;
		player_card[player_num].back().highlight_row(choosen_category);
	}

	if (thread_sleep_ms(3000))
		return;

	bool* flip = new bool[players_num]();
	for (int player_num = 0; player_num < players_num; player_num++)
	{
		if (loser[player_num])
			continue;
		player_card[player_num].back().highlight_row(-1);//no highlit
		flip[player_num] = true;
	}

	flip_cards(flip);
	delete[] flip;
	std::fill_n(winner, players_num, true);
	int winners_num = 0;

	for (int player_num = 0; player_num < players_num; player_num++)
	{
		if (loser[player_num])
		{
			winner[player_num] = false;
			continue;
		}

		for (int i = 0; i < players_num; i++)
		{
			if (player_num == i)//we will not compare a player with himself. But we still compare player 2 to 3, then 3 to 2...
				continue;
			if (loser[i])
				continue;
			float left = std::stof(player_card[player_num].back().get_category_value(choosen_category));
			float right = std::stof(player_card[i].back().get_category_value(choosen_category));
			if (cards.get_category_name(choosen_category) == "Acceleration")//time to 60mph is the only parameter when the lowest value wins
				winner[player_num] &= left <= right;
			else
				winner[player_num] &= left >= right;
		}
		winners_num += winner[player_num];
	}
	assert(winners_num > 0);
	if(winners_num != 1)
		state = Game_state::tie_break;
	else
		state = Game_state::transfer_cards_to_winner;
}

void Game::tiebreak()
{
	int winners_num = 0;
	for (int player_num = 0; player_num < players_num; player_num++)
		winners_num += winner[player_num];

	while (winners_num != 1)//tiebreak
	{
		assert(winners_num > 1);

		for (int tiebreak_cards_num = 0; tiebreak_cards_num < 2; tiebreak_cards_num++)
		{
			int empty_stacks_num = 0;
			Card_translation* translation = new Card_translation[players_num];
			for (int player_num = 0; player_num < players_num; player_num++)
			{
				if (winner[player_num])
				{
					translation[player_num] = Card_translation::without_flip;
					if (player_stack[player_num].empty())
					{
						empty_stacks_num++;
						translation[player_num] = Card_translation::no_translation;
					}
				}
				else
					translation[player_num] = Card_translation::no_translation;
			}

			if (winners_num - empty_stacks_num >= 2)//there are players left for tiebreak
			{
				for (int player_num = 0; player_num < players_num; player_num++)
				{
					if (player_stack[player_num].empty())
					{
						loser[player_num] = true;
						winner[player_num] = false;
					}
				}
				move_cards(translation);
			}
			else if (winners_num == empty_stacks_num)//winners have no cards left - move back cards form hand to stack (and shuffle them)
			{
				const int iterations_max = 60;
				for (int i = 0; i < iterations_max; i++)
				{
					if (thread_sleep_ms(17))
						return;
					for (int player_num = 0; player_num < players_num; player_num++)
					{
						if (!winner[player_num])
							continue;
						for (Card& card : player_card[player_num])
						{
							card.pos.x += 1.0f / iterations_max;
							card.pos.z += 0.5f / iterations_max;
						}
					}
				}

				bool cards_equal_in_selected_category = true;
				std::string category_value;
				for (int player_num = 0; player_num < players_num; player_num++)
				{
					if (!winner[player_num])
						continue;
					for (Card& card : player_card[player_num])
					{
						card.reset_coords();
						if(category_value.empty())
							category_value = card.values[choosen_category];
						else if(cards_equal_in_selected_category)
						{
							if (category_value != card.values[choosen_category])
								cards_equal_in_selected_category = false;
						}
					}
						
					lock.lock();
					player_stack[player_num] = player_card[player_num];
					player_card[player_num].clear();
					std::random_shuffle(player_stack[player_num].begin(), player_stack[player_num].end(), [](int a) -> int {return rand() % a; });
					lock.unlock();

					//it is possible, that all the winners left have the same value in the selected category.
					//in this case the category should be randomized
					if (cards_equal_in_selected_category)
					{
						choosen_category = rand() % 6;
						std::cout << "The category has been randomized because all players taking part in the tie break have the same value in the previous category." << std::endl;
					}

				}

				for (int player_num = 0; player_num < players_num; player_num++)
				{
					if (winner[player_num])
						translation[player_num] = Card_translation::without_flip;
					else
						translation[player_num] = Card_translation::no_translation;
				}
				move_cards(translation);
			}
			else//olny 1 player with non-empty stack left - the final winner
			{
				for (int player_num = 0; player_num < players_num; player_num++)
				{
					if (player_stack[player_num].empty())
					{
						loser[player_num] = true;
						winner[player_num] = false;
					}
				}
			}
			delete[] translation;
		}

		winners_num = 0;
		for (int player_num = 0; player_num < players_num; player_num++)
			winners_num += winner[player_num];
		assert(winners_num > 0);

		if (winners_num != 1)
		{
			flip_cards(winner);

			for (int player_num = 0; player_num < players_num; player_num++)
				if (winner[player_num])
					player_card[player_num].back().highlight_row(choosen_category);

			if (thread_sleep_ms(3000))
				return;

			winners_num = 0;
			bool* flip = new bool[players_num]();
			for (int player_num = 0; player_num < players_num; player_num++)
			{
				if (loser[player_num])
					continue;
				if (winner[player_num])
					player_card[player_num].back().highlight_row(-1);//no highlight
				for (int i = 0; i < players_num; i++)
				{
					if (player_num == i)//we will not compare a player with himself
						continue;
					if (!winner[player_num])
						continue;
					if (!winner[i])
						continue;
					if (loser[i])
						continue;
					float left = std::stof(player_card[player_num].back().get_category_value(choosen_category));
					float right = std::stof(player_card[i].back().get_category_value(choosen_category));
					if (cards.get_category_name(choosen_category) == "Acceleration")//time to 60mph is the only parameter when the lowest value wins
						winner[player_num] &= left <= right;
					else
						winner[player_num] &= left >= right;
				}
				winners_num += winner[player_num];
				flip[player_num] = !player_card[player_num].back().invert;
			}
			flip_cards(flip);
			delete[] flip;
		}
		assert(winners_num > 0);
	}
	assert(winners_num == 1);
	state = Game_state::transfer_cards_to_winner;
}

void Game::cards_to_winner()
{
	for (int player_num = 0; player_num < players_num; player_num++)
		if (winner[player_num])
			current_player = player_num;

	for (int shift : {0, 3, 2, 1})
	{
		int player_num = (shift + current_player) % 4;
		if (player_num >= players_num)
			continue;
		if (player_card[player_num].empty())
			continue;
		const int iterations_max = 60;
		for (int i = 0; i < iterations_max; i++)
		{
			if (thread_sleep_ms(17))
				return;

			Vec3 move;
			float angle = 0.0f;
			if (current_player == player_num)
				move = Vec3(1.0f, 0.0f, 0.5f);
			else if (current_player == 0 && player_num == 3)
			{
				move = Vec3(-1.2f, 0.0f, -0.4f);
				angle = -90.0f;
			}
			else if (current_player == 0 && player_num == 2)
			{
				move = Vec3(-1.0f, 0.0f, -2.2f);
				angle = 180.0f;
			}
			else if (current_player == 0 && player_num == 1)
			{
				move = Vec3(1.2f, 0.0f, -2.4f);
				angle = 90.0f;
			}
			else if (current_player == 1 && player_num == 3)
			{
				move = Vec3(-1.0f, 0.0f, -3.3f);
				angle = 180.0f;
			}
			else if (current_player == 1 && player_num == 2)
			{
				move = Vec3(1.9f, 0.0f, -2.0f);
				angle = 90.0f;
			}
			else if (current_player == 1 && player_num == 0)
			{
				move = Vec3(-1.9f, 0.0f, 0.3f);
				angle = -90.0f;
			}
			else if (current_player == 2 && player_num == 3)
			{
				move = Vec3(1.5, 0.0f, -2.4f);
				angle = 90.0f;
			}
			else if (current_player == 2 && player_num == 1)
			{
				move = Vec3(-1.5f, 0.0f, -0.4f);
				angle = -90.0f;
			}
			else if (current_player == 2 && player_num == 0)
			{
				move = Vec3(-1.0f, 0.0f, -2.2f);
				angle = 180.0f;
			}
			else if (current_player == 3 && player_num == 2)
			{
				move = Vec3(-1.9f, 0.0f, 0.0f);
				angle = -90.0f;
			}
			else if (current_player == 3 && player_num == 1)
			{
				move = Vec3(-1.0f, 0.0f, -3.3f);
				angle = 180.0f;
			}
			else if (current_player == 3 && player_num == 0)
			{
				move = Vec3(1.9f, 0.0f, -1.7f);
				angle = 90.0f;
			}

			if (i < iterations_max / 4)
			{
				for (Card& card : player_stack[current_player])
					card.pos.y += (player_card[player_num].size() * 0.005f) / (iterations_max / 4);
			}
			else
			{
				for (Card& card : player_card[player_num])
				{
					card.pos.x += move.x / (iterations_max * 3 / 4);
					card.pos.z += move.z / (iterations_max * 3 / 4);
					if (angle != 0.0f)
					{
						card.rot = Vec3(0.0f, 1.0f, 0.0f);
						card.angle += angle / (iterations_max * 3 / 4);
					}
				}
			}
		}
		lock.lock();
		for (Card& card : player_card[player_num])
			card.reset_coords();
		player_stack[current_player].insert(player_stack[current_player].begin(), player_card[player_num].begin(), player_card[player_num].end());
		player_card[player_num].clear();
		lock.unlock();
	}
	if(player_stack[current_player].size() == 24)
		state = Game_state::finish;
	else
		state = Game_state::next_round;
}

void Game::start(int players_num)
{
	if (players_num < 2 || players_num > 4)
	{
		std::cerr << "The game is designed for 2-4 players.\n";
		return;
	}
	central_stack = cards.get_cards_vec();
	for (Card& card : central_stack)
	{
		card.reset_coords();
		card.highlight_row(-1);
	}

	player_card.clear();
	player_stack.clear();

	std::random_shuffle(central_stack.begin(), central_stack.end(), [](int a) -> int {return rand() % a; });
	if (central_stack.size() != 24)
		std::cerr << "Number of cards is != 24 (" + std::to_string(central_stack.size()) + ")\n";
	winner = new bool[players_num];
	loser = new bool[players_num]();
	std::fill_n(loser, players_num, false);
	this->players_num = players_num;
	state = Game_state::cards_distribution;
}

void Game::draw_players_stacks()
{
	//stacks:
	if (players_num < 2)
		return;
	//player 0:
	glPushMatrix();
	glTranslatef(1.0f, 0.0f, 1.5f);
	draw_cards_stack(player_stack[0]);
	glPopMatrix();
	//player 1:
	glPushMatrix();
	glTranslatef(-1.9f, 0.0f, 1.3f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	draw_cards_stack(player_stack[1]);
	glPopMatrix();
	if (players_num < 3)
		return;
	//player 2:
	glPushMatrix();
	glTranslatef(-1.0f, 0.0f, -1.2f);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	draw_cards_stack(player_stack[2]);
	glPopMatrix();
	if (players_num < 4)
		return;
	//player 3:
	glPushMatrix();
	glTranslatef(1.9f, 0.0f, -0.7f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	draw_cards_stack(player_stack[3]);
	glPopMatrix();
}

void Game::draw_players_cards()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f);
	draw_cards_stack(player_card[0]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.4f, 0.0f, 0.3f);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	draw_cards_stack(player_card[1]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.7f);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	draw_cards_stack(player_card[2]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.4f, 0.0f, 0.3f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	draw_cards_stack(player_card[3]);
	glPopMatrix();
}

void Game::set_pause(bool pause)
{
	this->pause = pause;
}
void Game::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0.0f, 2.3f, 1.15f,//camera position
		0.0f, 0.0f, 0.44f,// camera look-at point
		0.0f, 1.0f, 0.0f //vertical vector
	);

	ui.render();

	if (!pause)
	{
		scene.draw();
		draw_cards_stack(central_stack);
		draw_players_stacks();
		draw_players_cards();
		lock.lock();
		for (Text3D& text : texts)
			text.render();
		lock.unlock();
	}

	switch (state)
	{
	case Game_state::no_action:
		break;
	case Game_state::cards_distribution:
		state = Game_state::no_action;
		threads.push_back(std::thread(&Game::distribute_cards, this));
		break;
	case Game_state::cards_to_players:
		state = Game_state::no_action;
		threads.push_back(std::thread(&Game::cards_to_players, this));
		break;
	case Game_state::choose_category:
		state = Game_state::no_action;
		threads.push_back(std::thread(&Game::choose_category, this));
		break;
	case Game_state::show_players_cards:
		state = Game_state::no_action;
		threads.push_back(std::thread(&Game::show_players_cards, this));
		break;
	case Game_state::compare_by_choosen_category:
		state = Game_state::no_action;
		threads.push_back(std::thread(&Game::compare_by_choosen_category, this));
		break;
	case Game_state::tie_break:
		state = Game_state::no_action;
		threads.push_back(std::thread(&Game::tiebreak, this));
		break;
	case Game_state::next_round:
		state = Game_state::cards_to_players;
		break;
	case Game_state::transfer_cards_to_winner:
		state = Game_state::no_action;
		threads.push_back(std::thread(&Game::cards_to_winner, this));
		break;
	case Game_state::finish:
		state = Game_state::no_action;
		for (int winner_num = 0; winner_num < players_num; winner_num++)
			if (winner[winner_num])
			{
				std::cout << "The Winner is player number " << winner_num << std::endl;
				break;
			}
		clean();
		unsigned int opp_num;
		do
		{
			std::cout << "Number of opponents for the next round: ";
			std::cin >> opp_num;
		}
		while (opp_num < 1 && opp_num > 3);
		start(opp_num + 1);
		break;
	default:
		break;
	}

	glFlush();
	glutSwapBuffers();
}

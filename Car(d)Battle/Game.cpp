#include "Game.h"
#include <algorithm>
#include <time.h>

Game::Game()
	:cards(L"carlist.xls")
{
srand(static_cast<unsigned int>(time(NULL)));
	state = Game_state::No_action;
	for (int i = 0; i < 24; i++)
		random_translation_vec.push_back({ rand() / 1000000.0f - RAND_MAX / 2000000.0f, rand() / 1000000.0f - RAND_MAX / 2000000.0f });
	players_num = -1;
    central_stack = cards.get_cards_vec();
    std::random_shuffle(central_stack.begin(), central_stack.end(), [](int a) -> int {return rand() % a; });
    if (central_stack.size() != 24)
        std::cerr << "Number of cards is != 24 (" + std::to_string(central_stack.size()) + ")\n";
}

Game::~Game()
{
}

Game& Game::get_instance()
{
    return instance;
}

void Game::load()
{
    cards.create_lists();
    cards.load_textures();
    scene.load();
}

void Game::draw_cards_stack(std::vector<Card>& cards_vec, bool invert = true)
{
    glPushMatrix();
    lock.lock();// &cards_vec may be modified by other threads during execution of this method
    for (unsigned int i = 0; i < cards_vec.size(); i++)
    {
        glPushMatrix();
        if(invert)
            glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        cards_vec[i].draw();
        glPopMatrix();
        glTranslatef(random_translation_vec[i].first, 0.005f, random_translation_vec[i].second);
    }
    lock.unlock();
    glPopMatrix();
}

void Game::distribute_cards()
{
    int card_num = 0;
    while (!central_stack.empty())
    {
        Card& card = central_stack.back();
        card.rot = Vec3(0.0f, 1.0f, 0.0f);
        card.pos = Vec3();
        card.angle = 0.0f;
        const int iterations_max = 10;
        float current_height = central_stack.size() * 0.005f;
        float destination_height = player_stack[card_num % players_num].size() * 0.005f + 0.005f;
        float height_difference = destination_height - current_height;
        for (int i = 0; i < iterations_max; i++)
        {
            std::this_thread::sleep_for(17ms);
            switch (card_num%players_num)
            {
            case 0:
                card.pos.x += -1.0f / iterations_max;
                card.pos.z += 1.5f / iterations_max;
                break;
            case 1:
                card.pos.x += 1.9f / iterations_max;
                card.pos.z += 1.3f / iterations_max;
                card.angle += 90.0f / iterations_max;
                break;
            case 2:
                card.pos.x += 1.0f / iterations_max;
                card.pos.z += -1.2f / iterations_max;
                break;
            case 3:
                card.pos.x += -1.9f / iterations_max;
                card.pos.z += -0.7f / iterations_max;
                card.angle += 90.0f / iterations_max;
                break;
            }

            if (height_difference > 0 && i < iterations_max/4)//can raise here
                card.pos.y -= height_difference / 0.25f / iterations_max;

            if (height_difference < 0 && i > iterations_max * 3 / 4)//can descend here
                card.pos.y -= height_difference / 0.25f / iterations_max;
        }
        lock.lock();
        central_stack.pop_back();
        lock.unlock();
        card.rot = Vec3();
        card.pos = Vec3();
        card.angle = 0.0f;
        lock.lock();
        player_stack[card_num % players_num].push_back(card);
        lock.unlock();
        card_num++;
    }
    current_player = rand() % players_num;
    current_player = 0;//TEST!
    state = Game_state::Choose_category;
}

void Game::card_to_player()
{
    for (int player_num = 0; player_num < players_num; player_num++)
    {
        Card& card = player_stack[player_num].back();
        card.rot = Vec3(0.0f, 0.0f, 0.0f);
        card.pos = Vec3();
        card.angle = 0.0f;
    }
    float initial_height = player_stack[0].size() * 0.005f;
    float dest_height = player_card[0].size() * 0.005f;
    float height_diff = dest_height - initial_height;
    const int iterations_max = 60;
    for (int i = 0; i < iterations_max; i++)
    {
        std::this_thread::sleep_for(17ms);
        for (int player_num = 0; player_num < players_num; player_num++)
        {
            Card& card = player_stack[player_num].back();
            card.pos.x += 1.0f / iterations_max;
            card.pos.z += -0.5f / iterations_max;
            if (player_num == 0)
            {
                card.angle += 180.0f / iterations_max;
                card.rot.z = 1.0f;
                card.pos.y = -sqrtf(-static_cast<float>(i*2)/iterations_max*(static_cast<float>(i*2) / iterations_max -2)) * CARD_WIDTH / 2;//rotate around left edge of the card
                card.pos.y -= height_diff * i / iterations_max;
            }
        }
    }
    for (int player_num = 0; player_num < players_num; player_num++)
    {
        player_stack[player_num].back().reset_coords();
        player_card[player_num].push_back(player_stack[player_num].back());
        player_stack[player_num].pop_back();
    }
    if (current_player == 0)
    {
        Text3D text;
        text.pos = Vec3(-0.6f, 0.01f, 0.2f);
        text.angle = -60.0f;
        text.rot = Vec3(1.0f, 0.0f, 0.0f);
        text.scale = 0.001;
        text.text = "Choose a category...";
        text.line_width = 7.5f;
        lock.lock();
        unsigned int text_id = texts.size();
        texts.push_back(text);
        lock.unlock();


    }
    else//wait for server response
    {
        while (true)
        {
            std::this_thread::sleep_for(200ms);
        }
    }
}

void Game::start(int players_num)
{
    if (players_num < 2 || players_num > 4)
    {
        std::cerr << "The game is designed for 2-4 players.\n";
        return;
    }
    this->players_num = players_num;
    state = Game_state::Cards_distribution;
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
    draw_cards_stack(player_card[0], false);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.7f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    draw_cards_stack(player_card[1]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.4f, 0.0f, 0.3f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    draw_cards_stack(player_card[2]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.4f, 0.0f, 0.3f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    draw_cards_stack(player_card[3]);
    glPopMatrix();
}

void Game::draw()
{
    scene.draw();
    std::thread th;
    switch (state)
    {
    case Game_state::No_action:
        draw_cards_stack(central_stack);
        draw_players_stacks();
        draw_players_cards();
        lock.lock();
        for (Text3D& text : texts)
            text.render();
        lock.unlock();
        break;
    case Game_state::Cards_distribution:
        state = Game_state::No_action;
        th = std::thread(&Game::distribute_cards, this);
        th.detach();
        break;
    case Game_state::Choose_category:
        state = Game_state::No_action;
        th = std::thread(&Game::card_to_player, this);
        th.detach();
        break;
    case Game_state::Show_players_cards:
        break;
    case Game_state::Compare_by_choosen_category:
        break;
    case Game_state::Tie_break:
        break;
    case Game_state::Next_round:
        break;
    case Game_state::Transfer_cards_to_winner:
        break;
    case Game_state::Finish:
        break;
    default:
        break;
    }
}
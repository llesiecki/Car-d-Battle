#include "Game.h"

Game::Game()
	:cards(L"carlist.xls")
{
	state = Game_state::No_action;
	for (int i = 0; i < 24; i++)
		random_translation_vec.push_back({ rand() / 1000000.0f - RAND_MAX / 2000000.0f, rand() / 1000000.0f - RAND_MAX / 2000000.0f });
	players_num = -1;
    central_stack = cards.get_cards_vec();
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
    for (unsigned int i = 0; i < cards_vec.size(); i++)
    {
        glPushMatrix();
        if(invert)
            glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        cards_vec[i].draw();
        glPopMatrix();
        glTranslatef(random_translation_vec[i].first, 0.005f, random_translation_vec[i].second);
    }
    glPopMatrix();
}

void Game::distribute_cards()
{
    state = Game_state::No_action;
    int card_num = 0;
    while (!central_stack.empty())
    {
        lock.lock();
        Card& card = central_stack.back();
        lock.unlock();
        for (int i = 0; i < 20; i++)
        {
            std::this_thread::sleep_for(17ms);
            switch (card_num%players_num)
            {
            case 0:
                card.pos.z -= 5.0f / 100;
                break;
            case 1:
                card.pos.z += 5.0f / 100;
                break;
            case 2:
                card.pos.x -= 5.0f / 100;
                break;
            case 3:
                card.pos.x += 5.0f / 100;
                break;
            }
        }
        lock.lock();
        central_stack.pop_back();
        lock.unlock();
        card.pos = Vec3();
        card.rot = Vec3();
        card.angle = 0.0f;
        player_stack[card_num % players_num].push_back(card);
        card_num++;
    }
}

void Game::start(int players_num)
{
    if (players_num < 2 || players_num > 4)
    {
        std::cerr << "Game is designed for 2-4 players.\n";
        return;
    }
    this->players_num = players_num;

    for (int i = 0; i < players_num; i++)
    {
        player_stack[i] = std::vector<Card>();
        player_stack[i].push_back(central_stack[i]);
    }
    state = Game_state::Cards_distribution;
}

void Game::draw_players_stacks()
{
    //stacks:
    if (players_num < 2)
        return;
    //player 1:
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 1.5f);
    draw_cards_stack(player_stack[0]);
    glPopMatrix();
    //player 2:
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, -1.2f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    draw_cards_stack(player_stack[1]);
    glPopMatrix();
    if (players_num < 3)
        return;
    //player 3:
    glPushMatrix();
    glTranslatef(-1.9f, 0.0f, 1.3f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    draw_cards_stack(player_stack[2]);
    glPopMatrix();
    if (players_num < 4)
        return;
    //player 4:
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
    draw_cards_stack(player_card[1], false);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.4f, 0.0f, 0.3f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    draw_cards_stack(player_card[2], false);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.4f, 0.0f, 0.3f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    draw_cards_stack(player_card[3], false);
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
        break;
    case Game_state::Cards_distribution:
        th = std::thread(&Game::distribute_cards, this);
        th.detach();
        break;
    case Game_state::Choose_category:
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
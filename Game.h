#ifndef GAME_H
#define GAME_H

#include <vector>

#include "player.h"

class Game
{

public:
    Game();

    int n_players;
    std::vector<Player> players;

};

#endif // GAME_H

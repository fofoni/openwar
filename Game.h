#ifndef GAME_H
#define GAME_H

#include <cstdlib>
#include <ctime>

#include <vector>

#include <QDebug>

#define TAU ( double(6.283185307179586477) /* tau is 2*pi */ )
#include "player.h"
#include "terr.h"

class Game
{

public:
    Game(std::vector<Terr>& g, int qtd,
         const QList<QString> &player_names,
         const QList<QColor> &player_colors);

    int num_players;
    std::vector<Player> players;
    std::vector<Terr>& graph;

};

#endif // GAME_H

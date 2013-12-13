#include <cmath>

#include <iostream>

#include "Game.h"


// random in [ 0 ... N-1 ]
#define RAND_RANGE(N) ( \
    long(floor( double(N) * double(rand()) / (double(RAND_MAX)+1) )) \
)

Game::Game(std::vector<Terr>&g, int qtd,
           const QList<QString>& player_names,
           const QList<QColor>& player_colors)
    : num_players(qtd), graph(g)
{

    srand((unsigned)time(0));

    std::vector<bool> players_ordered(num_players, false);
    for (int i = 0; i < num_players; i++) {
        int pid;
        do pid = RAND_RANGE(num_players); while (players_ordered[pid]);
        players.push_back(Player(player_names[pid].toUtf8().constData(),
                                 player_colors[pid]));
        players_ordered[pid] = true;
    }

    for (std::vector<Player>::iterator it = players.begin();
         it != players.end(); ++it)
        std::cout << it->name << " ; " << it->c.redF() << " " << it->c.greenF() <<
                     " " << it->c.blueF() << " ; " << it->terrs << " " << it->html() << std::endl;

    // assign a different territory to each player
    int quot = graph.size() / num_players, rem = graph.size() % num_players;
    for (int i = 0; i < quot; i++)
        for (int j = num_players-1; j >= 0; j--) {
            int tid;
            do tid = RAND_RANGE(graph.size()); while (graph[tid].armies != 0);
            graph[tid].p = &(players[j]);
            graph[tid].armies = 1;
        }
    for (int j = num_players-1; j >= num_players-rem; j--) {
        int tid;
        do tid = RAND_RANGE(graph.size()); while (graph[tid].armies != 0);
        graph[tid].p = &(players[j]);
        graph[tid].armies = 1;
    }

}

#ifndef PLAYER_H
#define PLAYER_H

#include <QColor>

class Player {
public:

    std::string name;
    QColor c;

    Player(const std::string& name, const QColor& c) : name(name), c(c) {}
    ~Player() {}

};

#endif // PLAYER_H

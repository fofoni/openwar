#ifndef PLAYER_H
#define PLAYER_H

#include <QColor>
#include <QString>
#include <sstream>

class Player {
public:

    std::string name;
    QColor c;
    int terrs;

    Player(const std::string& name, const QColor& cc)
        : name(name), c(cc), terrs(0) {}
    ~Player() {}

    std::string html() const {
        std::stringstream ss;
        ss << "<b><font color='" << c.name().toUtf8().constData() << "'>";
        ss << name << "</font></b>";
        return ss.str();
    }

};

#endif // PLAYER_H

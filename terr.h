#ifndef TERR_H
#define TERR_H

#include <cmath>

// territory
class Terr {
public:

    // longitude and latitude in degrees. East and North are positive
    float x, y;
    // three-vector from the center of the earth
    double x0, y0, z0;
    std::vector<int> frontiers;
    QString name;

    int armies;
    Player *p;

    Terr(const float x, const float y, const std::string& n) :
        x(x), y(y), name(n.c_str()), armies(0), p(NULL)
    {
        y0 = sin(y*TAU/360);
        z0 = cos(x*TAU/360) * cos(y*TAU/360);
        x0 = sin(x*TAU/360) * cos(y*TAU/360);
    }
    ~Terr() {}

};

#endif // TERR_H

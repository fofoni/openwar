/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

#ifndef MAIN_H
#define MAIN_H

#include <QColor>

#include <cmath>

#include "player.h"

#define TAU ( double(6.283185307179586477) /* tau is 2*pi */ )

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
        x(x), y(y), name(n.c_str()), armies(1), p(NULL)
    {
        y0 = sin(y*TAU/360);
        z0 = cos(x*TAU/360) * cos(y*TAU/360);
        x0 = sin(x*TAU/360) * cos(y*TAU/360);
    }
    ~Terr() {}

};

int main(int argc, char** argv);

#endif

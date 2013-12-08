/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

#ifndef MAIN_H_
#define MAIN_H_

namespace Color {
    typedef float color[3];
}

class Player {
public:

    std::string name;
    const float *c;

    Player(std::string name, const float *c) : name(name), c(c) {}
    ~Player() {}

};

// territory
class Terr {
public:

    // longitude and latitude in degrees. East and North are positive
    float x, y;
    std::vector<int> frontiers;

    int armies;
    Player *p;

    Terr(float x, float y) : x(x), y(y), armies(1) {}
    ~Terr() {}

};

void init_render();

void handle_keypress(unsigned char key, int x, int y);
void handle_mouse(int button, int state, int x, int y);
void handle_resize(int w, int h);

void draw_scene();
void draw_single_army(const Color::color c, float x, float y,
                      float h=0, bool big=false,
                      float dx=0, float dy=0);
void draw_armies(const Terr& terr, int qtd);

int main(int argc, char** argv);

#endif

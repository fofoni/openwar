/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

// #include <exception>

#ifndef MAIN_H_
#define MAIN_H_

// #include <exception>

namespace Color {
    typedef float color[3];
}

const double TAU = 6.283185307179586477; // tau is 2*pi

class clickoutside_except : public std::exception {
    virtual const char* what() const throw() {
        return "Clicked outside the window";
    }
} clicked_outside;

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
    // three-vector from the center of the earth
    double x0, y0, z0;
    std::vector<int> frontiers;
    std::string name;

    int armies;
    Player *p;

    Terr(const float x, const float y, const std::string& n) :
        x(x), y(y), name(n), armies(1), p(NULL)
    {
        y0 = sin(y*TAU/360);
        z0 = cos(x*TAU/360) * cos(y*TAU/360);
        x0 = sin(x*TAU/360) * cos(y*TAU/360);
    }
    ~Terr() {}

};

void init_render();

void handle_keypress(unsigned char key, int x, int y);
void handle_mouse(int button, int state, int x, int y);
void handle_resize(int w, int h);

void get_click_vec(int alpha, int beta, double &x, double &y, double &z);
std::vector<Terr>::iterator get_click_terr(int alpha, int beta);

void draw_scene();
void draw_single_army(const Color::color c, float x, float y,
                      float h=0, bool big=false,
                      float dx=0, float dy=0);
void draw_armies(const Terr& terr, int qtd);

int main(int argc, char** argv);

#endif

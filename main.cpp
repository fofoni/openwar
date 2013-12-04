/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

/*****************************************
************** STL includes **************
*****************************************/

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>

#include <vector>
#include <map>

/******************************************
************** math includes **************
******************************************/

#include <cmath>

/********************************************
************** OpenGL includes **************
********************************************/

#include <GL/glew.h>
#ifdef __APPLE__
# include <OpenGL/OpenGL.h>
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#include "loadpng.h"

/****************************************
************** definitions **************
****************************************/

// version string
#define TOSTR_(x...) #x // needs to be variadic because x might contain commas
#define STRINGIFY(x) TOSTR_(x)
#ifdef OpenWAR_VERSION
# define OpenWAR_VERSION_STR STRINGIFY(OpenWAR_VERSION)
#else
# define OpenWAR_VERSION_STR "v???"
#endif

class Terr {
public:
    float x, y;
    Terr(float x, float y) : x(x), y(y) {}
    ~Terr() {}
    std::vector<int> frontiers;
};

namespace Key {
    static const unsigned char ESC = 27;
}

namespace Color {
    static const float black[] = {0, 0, 0};
    static const float dark_green[] = {0, 0, .5};
    static const float green[] = {0, 0, 1};
    static const float dark_blue[] = {0, .5, 0};
    static const float dark_cyan[] = {0, .5, .5};
    // static const float unknown_color[] = {0, .5, 1};
}

const double TAU = 6.283185307179586477; // tau is 2*pi
const int WORLD_LAT_QTD = 50; // TODO: make these configurable at runtime
const int WORLD_LONG_QTD = 100;

const int STONE_LONG_QTD = 3;
const float STONE_HEIGHT = .1;
const float STONE_RAD = .2;
const float STONE_IN_PERC = 0.80;
const float STONE_HOLE_PERC = 0.80;

const double WORLD_LAT_EPS = TAU/double(2*WORLD_LAT_QTD);
const double WORLD_LONG_EPS = TAU/double(WORLD_LONG_QTD);

/****************************************
************** global vars **************
****************************************/

int latitude = 0, longitude = 0;

GLuint world_tex_map, world_tex_graph, world_curr_tex;
int wtm_width = 1024, wtm_height = 1024,
    wtg_width = 1024, wtg_height = 1024;

float sph_vertices[WORLD_LONG_QTD+1][WORLD_LAT_QTD+1][3];

std::vector<Terr> graph;

/******************************************
************** GLUT routines **************
******************************************/

using namespace std;

void handle_keypress(unsigned char key, int x, int y) {
    switch (key) {
        case Key::ESC:
            /*throw escape*/; break;
        case 'a':
            longitude -= -10; // TODO: make this number depend on the zoom level
            break;
        case 'd':
            longitude += -10;
            break;
        case 's':
            latitude -= 10;
            break;
        case 'w':
            latitude += 10;
            break;
        case 'v':
            if (world_curr_tex == world_tex_map)
                world_curr_tex = world_tex_graph;
            else
                world_curr_tex = world_tex_map;
            break;
    }
    glutPostRedisplay();
}

void init_render() {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(.14, .18, .2, 1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    world_tex_map = loadpng("imgs/earth_tex.png", wtm_width, wtm_height);
    world_tex_graph = loadpng("imgs/earth_graph.png", wtg_width, wtg_height);
    world_curr_tex = world_tex_map;

}

void handle_resize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        45,                    // camera angle
        double(w) / double(h), // width-to-height ratio
        .5, 200                // near and far z clipping coordinates
    );
}

void draw_scene() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ambient light
    GLfloat ambient_color[] = {.5, .5, .5, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_color);

    // positioned light
    GLfloat light_color0[] = {5, 5, 5, 1};
    GLfloat light_pos0[] = {-2, 3, 0, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

    glTranslatef(0, 0, -3);
    glRotatef(latitude, 1, 0, 0);
    glRotatef(longitude, 0, 1, 0);

    // draw sphere representing earth
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, world_curr_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    for (int j = 1; j <= WORLD_LAT_QTD; j++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= WORLD_LONG_QTD; i++) {

            glNormal3f(sph_vertices[i][j-1][0], sph_vertices[i][j-1][1],
                       sph_vertices[i][j-1][2]);
            glTexCoord2f(float(double(i)/double(WORLD_LONG_QTD)), 1-float(double(j-1)/double(WORLD_LAT_QTD)));
            glVertex3f(sph_vertices[i][j-1][0], sph_vertices[i][j-1][1],
                       sph_vertices[i][j-1][2]);

            glNormal3f(sph_vertices[i][j][0], sph_vertices[i][j][1],
                       sph_vertices[i][j][2]);
            glTexCoord2f(float(double(i)/double(WORLD_LONG_QTD)), 1-float(double(j)/double(WORLD_LAT_QTD)));
            glVertex3f(sph_vertices[i][j][0], sph_vertices[i][j][1],
                       sph_vertices[i][j][2]);

        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();

}

/**************************************
************** main prog **************
**************************************/

int main(int argc, char** argv) {

    cout << "OpenWAR" << endl << OpenWAR_VERSION_STR << endl;
    cout << endl;

    // sphere vertices
    cout << "Calculating sphere vertices...";
    // we should be able to calculate these at compile-time, actually
    for (int j = 0; j <= WORLD_LAT_QTD; j++) {
        for (int i = 0; i <= WORLD_LONG_QTD; i++) {
            sph_vertices[i][j][0] = -sin(j*WORLD_LAT_EPS)*sin(i*WORLD_LONG_EPS);
            sph_vertices[i][j][1] = cos(j*WORLD_LAT_EPS);
            sph_vertices[i][j][2] = -sin(j*WORLD_LAT_EPS)*cos(i*WORLD_LONG_EPS);
        }
    }
    cout << endl;

    // load territories data
    cout << "Loading territories and frontiers data...";
    {
        string line;
        char filename[] = "terr_db"; // TODO: make configurable at runtime
        ifstream file (filename);
        if (!file.is_open()) {
            cerr << "Cannot open database file `" << filename << "'" << endl;
            return 1;
        }
        map<string, int> names;
        for (int count=0; getline(file, line); count++) {
            if (line.length() == 0) { count--; continue; }
            if (line[0] == '#') { count--; continue; }
            istringstream ss(line);
            string word;
            float x, y;
            ss >> word;
            names[word] = count;
            ss >> x >> y;
            graph.push_back(Terr(x/1024, 1-y/512));
        }
        file.clear();
        file.seekg(0, file.beg);
        for (int count=0; getline(file, line); count++) {
            if (line.length() == 0) { count --; continue; }
            if (line[0] == '#') { count--; continue; }
            istringstream ss(line);
            string word;
            // discard first three words (name, x coord, y coord)
            ss >> word; ss >> word; ss >> word;
            while (ss >> word)
                graph[count].frontiers.push_back(names[word]);
        }
        file.close();
    }
    cout << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);

    glutCreateWindow("OpenWAR");
    init_render();

    glutDisplayFunc(draw_scene);
    glutKeyboardFunc(handle_keypress);
    glutReshapeFunc(handle_resize);

    // try {
    glutMainLoop();
    // }
    // catch (exception& e) {
    //     cout << "Bye." << endl;
    // }

    return 0;

}

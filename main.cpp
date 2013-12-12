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

#include <exception>
#include <stdexcept>

/******************************************
************** math includes **************
******************************************/

#include <cmath>

/****************************************
************** Qt includes **************
****************************************/

#include <QtGui>

/*******************************************
************** local includes **************
*******************************************/

#include "main.h"
#include "loadpng.h"
#include "OpenWAR.h"

/*************************************************
************** GLUT and GL routines **************
*************************************************/

using namespace std;

/*void handle_keypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            longitude -= -zoom/5;
            break;
        case 'd':
            longitude += -zoom/5;
            break;
        case 's':
            latitude -= zoom/5;
            break;
        case 'w':
            latitude += zoom/5;
            break;
        case 'v':
            if (world_curr_tex == world_tex_map)
                world_curr_tex = world_tex_graph;
            else
                world_curr_tex = world_tex_map;
            break;
        case 'z':
            if (zoom >= 10) zoom -= 5;
            handle_resize(window_w, window_h);
            break;
        case 'x':
            if (zoom <= 170) zoom += 5;
            handle_resize(window_w, window_h);
            break;
        case 'k':
            if (darkening >= 1) darkening -= 1;
            break;
        case 'l':
            if (darkening <= 9) darkening += 1;
            break;
        case 'h':
            longitude = latitude = 0;
            darkening = 10;
            zoom = 50;
            world_curr_tex = world_tex_map;
            handle_resize(window_w, window_h);
            break;
    }
    glutPostRedisplay();
}*/

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

void get_click_vec(const int alpha, const int beta,
                   double &x0, double &y0, double &z0) {

    double a = double(alpha - .5*window_w)/double(window_h);
    double b = .5 - double(beta)/double(window_h);
    // norm squared of (a,b)
    double norm2_ab = a*a + b*b;
    // tangent and cosine of phi, the angle between the clicking direction
    // and the eye direction.
    // zoom is theta in degrees; tan(phi) = 2*tan(theta/2)*norm((a,b))
    double tan2p = 4 * tan(zoom*TAU/720)*tan(zoom*TAU/720) * norm2_ab;
    double cos2p = 1/(1+tan2p);
    double discriminant = 1/cos2p - tan2p*CAMERA_DISTANCE*CAMERA_DISTANCE;

    if (discriminant < 0) throw clicked_outside;

    // point of intersection (with the origin at center of the earth)
    z0 = cos2p*(CAMERA_DISTANCE*tan2p + sqrt(discriminant));
    double lambda = (norm2_ab != 0) ? sqrt((1-z0*z0)/norm2_ab) : 0;
    x0 = lambda * a;
    y0 = lambda * b;

    double x0_, y0_, z0_;
    double cla=cos(latitude*TAU/360),  sla=sin(latitude*TAU/360),
           clo=cos(longitude*TAU/360), slo=sin(longitude*TAU/360);

    x0_ =   x0;
    y0_ =   cla*y0  + sla*z0;
    z0_ = - sla*y0  + cla*z0;

    y0  =   y0_;
    z0  =   clo*z0_ + slo*x0_;
    x0  = - slo*z0_ + clo*x0_;

}

vector<Terr>::iterator get_click_terr(int alpha, int beta) {
    double x0, y0, z0;
    get_click_vec(alpha, beta, x0, y0, z0);
    double closer_prod = -1; vector<Terr>::iterator pointer;
    for(vector<Terr>::iterator it = graph.begin(); it != graph.end(); ++it) {
        double curr_prod = x0*it->x0 + y0*it->y0 + z0*it->z0;
        if (curr_prod > closer_prod) {
            closer_prod = curr_prod;
            pointer = it;
        }
    }
    return pointer;
}

void handle_mouse(int button, int state, int alpha, int beta) {

    // state = GLUT_UP (release) ou GLUT_DOWN (press)
    // button = GLUT_LEFT_BUTTON ou GLUT_RIGHT_BUTTON

    if ((state == GLUT_DOWN) || (button != GLUT_LEFT_BUTTON)) return;

    cout << get_click_terr(alpha, beta)->name << endl;

}

inline void make_perspective(GLdouble fovy, GLdouble aspect,
                    GLdouble zNear, GLdouble zFar) {
    GLdouble ymax = zNear * tan(fovy * TAU / 720.0);
    GLdouble ymin = -ymax;
    GLdouble xmin = ymin * aspect;
    GLdouble xmax = ymax * aspect;
    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

void handle_resize(int w, int h) {
    window_w = w; window_h = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    make_perspective(
        zoom,                  // camera angle (vertical direction)
        double(w) / double(h), // width-to-height ratio
        .5, 200                // near and far z clipping coordinates
    );
    // TODO: Qt doc says to put GL_MODELVIEW here. what now?
}

// TODO: material
void draw_single_army(const Color::color c, float x, float y,
                      float h, bool big, float dx, float dy) {

    glPushMatrix();
    glRotatef(x, 0, 1, 0);
    glRotatef(-y, 1, 0, 0);
    glRotatef(dx, 0, 1, 0);
    glRotatef(-dy, 1, 0, 0);
    glTranslatef(0, 0, 1);
    if (big) glScalef(ARMY_BIG_PERC, ARMY_BIG_PERC, ARMY_BIG_PERC);
    glTranslatef(0, 0, h*ARMY_HEIGHT*ARMY_GHOST_PERC);

    glColor3f(c[0], c[1], c[2]);

    // draw lower cap
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 0, -1);
    glVertex3f(0, 0, ARMY_HS[0]);
    for (int i = ARMY_LONG_QTD; i >= 0; i--) {
        glNormal3f(0, 0, -1);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], ARMY_HS[0]);
    }
    glEnd();

    // draw lower wall
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= ARMY_LONG_QTD; i++) {
        glNormal3f(-army_vertices[i][1][0], -army_vertices[i][1][1], 0);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], 0);
        glNormal3f(-army_vertices[i][1][0], -army_vertices[i][1][1], 0);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], ARMY_HS[0]);
    }
    glEnd();

    // draw bottom cap
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= ARMY_LONG_QTD; i++) {
        glNormal3f(0, 0, -1);
        glVertex3f(army_vertices[i][0][0], army_vertices[i][0][1], 0);
        glNormal3f(0, 0, -1);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], 0);
    }
    glEnd();

    // draw outer wall
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= ARMY_LONG_QTD; i++) {
        glNormal3f(army_vertices[i][0][0], army_vertices[i][0][1], 0);
        glVertex3f(army_vertices[i][0][0], army_vertices[i][0][1], ARMY_HEIGHT);
        glNormal3f(army_vertices[i][0][0], army_vertices[i][0][1], 0);
        glVertex3f(army_vertices[i][0][0], army_vertices[i][0][1], 0);
    }
    glEnd();

    // draw top cap
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= ARMY_LONG_QTD; i++) {
        glNormal3f(0, 0, 1);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], ARMY_HEIGHT);
        glNormal3f(0, 0, 1);
        glVertex3f(army_vertices[i][0][0], army_vertices[i][0][1], ARMY_HEIGHT);
    }
    glEnd();

    // draw upper wall
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= ARMY_LONG_QTD; i++) {
        glNormal3f(-army_vertices[i][1][0], -army_vertices[i][1][1], 0);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], ARMY_HS[1]);
        glNormal3f(-army_vertices[i][1][0], -army_vertices[i][1][1], 0);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], ARMY_HEIGHT);
    }
    glEnd();

    // draw upper cap
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 0, 1);
    glVertex3f(0, 0, ARMY_HS[1]);
    for (int i = 0; i <= ARMY_LONG_QTD; i++) {
        glNormal3f(0, 0, 1);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], ARMY_HS[1]);
    }
    glEnd();

    glColor3f(1, 1, 1); // reset color

    glPopMatrix();

}

void draw_armies(const Terr& terr, int qtd) {
    int q = qtd/5, r = qtd % 5;
    const float *c = terr.p->c;
    const float& x = terr.x;
    const float& y = terr.y;
    if (q == 0)
        switch (r) {
            case 1:
                draw_single_army(c,x,y);
                break;
            case 2:
                draw_single_army(c,x,y, 0, false, -1.5, 0);
                draw_single_army(c,x,y, 0, false,  1.5, 0);
                break;
            case 4:
                draw_single_army(c,x,y, 1, false);
                // fall through
            case 3:
                draw_single_army(c,x,y, 0, false, -1.5, -1.27);
                draw_single_army(c,x,y, 0, false,  1.5, -1.27);
                draw_single_army(c,x,y, 0, false,    0,  1.27);
        }
    else
        switch (r) {
            case 0:
                for (int i = 0; i < q; i++)
                    draw_single_army(c,x,y, i, true);
                break;
            case 1:
                for (int i = 0; i < q; i++)
                    draw_single_army(c,x,y, i, true, -1.7, 0);
                draw_single_army(c,x,y, 0, false, 1.7, 0);
                break;
            case 2:
                for (int i = 0; i < q; i++)
                    draw_single_army(c,x,y, i, true, -1.55, 0);
                draw_single_army(c,x,y, 0, false, 1.55, -1.5);
                draw_single_army(c,x,y, 0, false, 1.55,  1.5);
                break;
            case 4:
                draw_single_army(c,x,y, 1, false,  .85, 0);
                /* fall through */
            case 3:
                for (int i = 0; i < q; i++)
                    draw_single_army(c,x,y, i, true,  -3.1, 0);
                draw_single_army(c,x,y, 0, false,    0, -1.5);
                draw_single_army(c,x,y, 0, false,    0,  1.5);
                draw_single_army(c,x,y, 0, false, 2.54, 0);
                break;
        }
}

void draw_scene() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ambient light
    static GLfloat ambient_color[] = {.5, .5, .5, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_color);

    // positioned light
    static GLfloat light_color0[] = {5, 5, 5, 1};
    static GLfloat light_pos0[] = {-2, 3, 0, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

    glTranslatef(0, 0, -CAMERA_DISTANCE);
    glRotatef(latitude, 1, 0, 0);
    glRotatef(longitude, 0, 1, 0);

    // draw sphere representing earth
    glPushMatrix();
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, world_curr_tex);
    glColor3f(darkening/10., darkening/10., darkening/10.);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    for (int j = 1; j <= WORLD_LAT_QTD; j++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= WORLD_LONG_QTD; i++) {

            glNormal3f(sph_vertices[i][j-1][0], sph_vertices[i][j-1][1],
                       sph_vertices[i][j-1][2]);
            glTexCoord2f(double(i)/double(WORLD_LONG_QTD),
                         1-double(j-1)/double(WORLD_LAT_QTD));
            glVertex3f(sph_vertices[i][j-1][0], sph_vertices[i][j-1][1],
                       sph_vertices[i][j-1][2]);

            glNormal3f(sph_vertices[i][j][0], sph_vertices[i][j][1],
                       sph_vertices[i][j][2]);
            glTexCoord2f(double(i)/double(WORLD_LONG_QTD),
                         1-double(j)/double(WORLD_LAT_QTD));
            glVertex3f(sph_vertices[i][j][0], sph_vertices[i][j][1],
                       sph_vertices[i][j][2]);

        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // draw armies
    for(vector<Terr>::iterator it = graph.begin(); it != graph.end(); ++it) {
        draw_armies(*it, 49);
    }

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

    // army vertices
    cout << "Calculating army vertices..." << endl;
    for (int i = 0; i <= ARMY_LONG_QTD; i++) {
        army_vertices[i][0][0] = ARMY_RAD * cos(i*ARMY_LONG_EPS);
        army_vertices[i][0][1] = ARMY_RAD * sin(i*ARMY_LONG_EPS);
        army_vertices[i][1][0] = ARMY_RAD * cos(i*ARMY_LONG_EPS) * ARMY_IN_PERC;
        army_vertices[i][1][1] = ARMY_RAD * sin(i*ARMY_LONG_EPS) * ARMY_IN_PERC;
    }

    // TODO: Qt GUI
    players.push_back(Player(string("p0"), Color::dark_cyan));
    players.push_back(Player(string("p1"), Color::dark_magenta));
    players.push_back(Player(string("p2"), Color::dark_yellow));
    players.push_back(Player(string("p3"), Color::dark_red));
    players.push_back(Player(string("p4"), Color::dark_green));
    players.push_back(Player(string("p5"), Color::dark_blue));

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
            graph.push_back(Terr(360*(x/1024-.5), 180*(.5-y/512), word));
            graph[count].p = &(players[0]); // TODO: Qt GUI
        }
        file.clear(); file.seekg(0, file.beg); // for reading it again
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
    glutMouseFunc(handle_mouse);
    glutReshapeFunc(handle_resize);

    // try {
    glutMainLoop();
    // }
    // catch (exception& e) {
    //     cout << "Bye." << endl;
    // }

    return 0;

}

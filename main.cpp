#include <iostream>
#include <cmath>

#include "loadpng.h"

#ifdef __APPLE__
# include <OpenGL/OpenGL.h>
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

const double TAU = 6.283185307179586477; // tau is 2*pi
const int WORLD_LAT_QTD = 5;
const int WORLD_LONG_QTD = 10;

const double WORLD_LAT_EPS = TAU/double(2*WORLD_LAT_QTD);
const double WORLD_LONG_EPS = TAU/double(WORLD_LONG_QTD);

float sph_vertices[WORLD_LONG_QTD][WORLD_LAT_QTD+1][3];

namespace Key {
    const int ESC = 27;
}

int latitude = 0, longitude = 0;

GLuint world_texture;
int wt_width, wt_height;

using namespace std;

class esc_except : public exception {
    virtual const char* what() const throw() {
        return "ESC key pressed";
    }
} escape;

void handle_keypress(unsigned char key, int x, int y) {
    switch (key) {
        case Key::ESC:
            throw escape;
        case 'a':
            longitude -= 10; // TODO: make this number depend on the zoom level
            break;
        case 'd':
            longitude += 10;
            break;
        case 's':
            latitude -= 10;
            break;
        case 'w':
            latitude += 10;
            break;
    }
//     cout  << int(key) << "; long = " << longitude << endl;
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

//     world_texture = loadpng("earth.png", wt_width, wt_height);

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
    GLfloat light_color0[] = {.5, .5, .5, 1};
    GLfloat light_pos0[] = {-1, 1, 0, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

    glTranslatef(0, 0, -4);
    glRotatef(latitude, 1, 0, 0);
    glRotatef(longitude, 0, 1, 0);

    // draw sphere representing earth
    /* glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */
    glColor3f(0, 0, 1);
    for (int j = 1; j <= WORLD_LAT_QTD; j++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= WORLD_LONG_QTD; i++) {
            glNormal3f(sph_vertices[i][j-1][0], sph_vertices[i][j-1][1],
                       sph_vertices[i][j-1][2]);
            glVertex3f(sph_vertices[i][j-1][0], sph_vertices[i][j-1][1],
                       sph_vertices[i][j-1][2]);
            glNormal3f(sph_vertices[i][j][0], sph_vertices[i][j][1],
                       sph_vertices[i][j][2]);
            glVertex3f(sph_vertices[i][j][0], sph_vertices[i][j][1],
                       sph_vertices[i][j][2]);
        }
        glEnd();
    }
    /* glDisable(GL_TEXTURE_2D); */

    glutSwapBuffers();

}

int main(int argc, char** argv) {

    // we should be able to calculate these at compile-time, actually
    for (int j = 0; j <= WORLD_LAT_QTD; j++) {
        for (int i = 0; i <= WORLD_LONG_QTD; i++) {
            sph_vertices[i][j][0] = -sin(j*WORLD_LAT_EPS)*sin(i*WORLD_LONG_EPS);
            sph_vertices[i][j][1] = cos(j*WORLD_LAT_EPS);
            sph_vertices[i][j][2] = -sin(j*WORLD_LAT_EPS)*cos(i*WORLD_LONG_EPS);
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);

    glutCreateWindow("OpenWAR");
    init_render();

    glutDisplayFunc(draw_scene);
    glutKeyboardFunc(handle_keypress);
    glutReshapeFunc(handle_resize);
//     glutTimerFunc(25, update, 0);

    try {
        glutMainLoop();
    }
    catch (exception& e) {
        cout << "Bye." << endl;
    }

    return 0;

}

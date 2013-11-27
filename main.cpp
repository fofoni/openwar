#include <iostream>
#include <cmath>

#include "loadpng.h"

#ifdef __APPLE__
# include <OpenGL/OpenGL.h>
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

const int TAU = 6.283185307179586477; // tau is 2*pi
const int WORLD_LAT_QTD = 3;
const int WORLD_LONG_QTD = 6;

const int WORLD_LAT_EPS = TAU/(2*WORLD_LAT_QTD);
const int WORLD_LONG_EPS = TAU/WORLD_LONG_QTD;

namespace Key {
    const int ESC = 27;
}

float angle = 0;

GLuint world_texture;
int wt_width, wt_height;

using namespace std;

// class texture {
// public:
//     GLuint id;
//     int w, h;
//     texture(int id=-1, int w=-1, int h=-1) : id(id), w(w), h(h) {}
// };

// float blue = .65;
// texture tex;

// class esc_except : public exception {
//     virtual const char* what() const throw() {
//         return "ESC key pressed";
//     }
// } escape;

void handle_keypress(unsigned char key, int x, int y) {
    cout  << int(key) <<endl;
    switch (key) {
        case Key::ESC:
//             throw escape;
            ;
    }
}

void init_render() {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(.14, .18, .2, 1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
//     glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    world_texture = loadpng("earth.png", wt_width, wt_height);

}

void handle_resize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        45,                    // camera angle
        double(w) / double(h), // width-to-height ratio
        .5, 200                 // near and far z clipping coordinates
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

    // make sphere representing earth
    /* glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */
    for (int j = 0; j < WORLD_LAT_QTD; j++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= WORLD_LONG_QTD; i++) {
            glVertex3f(
                sin(j*WORLD_LAT_EPS)*sin(i*WORLD_LONG_EPS),
                cos(j*WORLD_LAT_EPS),
                sin(j*WORLD_LAT_EPS)*cos(i*WORLD_LONG_EPS)
            );
            glVertex3f(
                sin((j+1)*WORLD_LAT_EPS)*sin(i*WORLD_LONG_EPS),
                cos((j+1)*WORLD_LAT_EPS),
                sin((j+1)*WORLD_LAT_EPS)*cos(i*WORLD_LONG_EPS)
            );
        }
        glEnd();
    }
    /* glDisable(GL_TEXTURE_2D); */

//     // Trapezoid
//     glPushMatrix();
//     glTranslatef(0, -1, 0);
//     glRotatef(angle, 0, 0, 1);
//     glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, tex.id);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glBegin(GL_QUADS);
//
//         glColor3f(0.5f, 1, 1-blue);
//         glTexCoord2f(0, 0);
//         glVertex3f(-0.7f, -0.5f, 0.0f);
// //         glColor3f(0.0f, 0.9f, 0.0f);
//         glTexCoord2f(1, 0);
//         glVertex3f(0.7f, -0.5f, 0.0f);
// //         glColor3f(1.0f, 0.0f, 0.0f);
//         glTexCoord2f(1, 1);
//         glVertex3f(0.4f, 0.5f, 0.0f);
//         glColor3f(0.0f, 0.65, blue);
//         glTexCoord2f(0, 1);
//         glVertex3f(-0.4f, 0.5f, 0.0f);
//
//     glEnd();
//     glDisable(GL_TEXTURE_2D);
//     glPopMatrix();
//
//     // Pentagon
//     glPushMatrix();
//     glTranslatef(1, 1, 0);
//     glRotatef(angle, 1, 2, 3);
//     glScalef(.5, .5, .5);
//     glBegin(GL_TRIANGLES);
//
//         glVertex3f(-0.5f, -0.5f, 0.0f);
//         glVertex3f(0.5f, -0.5f, 0.0f);
//         glVertex3f(-0.5f, 0.0f, 0.0f);
//
//         glVertex3f(-0.5f, 0.0f, 0.0f);
//         glVertex3f(0.5f, -0.5f, 0.0f);
//         glVertex3f(0.5f, 0.0f, 0.0f);
//
//         glVertex3f(-0.5f, 0.0f, 0.0f);
//         glVertex3f(0.5f, 0.0f, 0.0f);
//         glVertex3f(0.0f, 0.5f, 0.0f);
//
//     glEnd();
//     glPopMatrix();
//
//     // Triangle
//     glPushMatrix();
//     glTranslatef(-1, 1, 0);
//     glRotatef(angle, 0, 1, 0);
//     glBegin(GL_TRIANGLES);
//
//         glNormal3f(1.f, 0.f, 1.f);
//         glVertex3f(0.5f, -0.5f, 0.0f);
//         glNormal3f(0.f, 1.f, 1.f);
//         glVertex3f(0.0f, 0.5f, 0.0f);
//         glNormal3f(-1.0f, 0.0f, 1.0f);
//         glVertex3f(-0.5f, -0.5f, 0.0f);
//
//     glEnd();
//     glPopMatrix();

    glutSwapBuffers();

}

// void update(int value) {
//
//     angle += 2;
//     if (angle >= 180) angle -= 360;
//
//     blue += .0005;
//     if (blue >= 1) blue -= 1;
//
//     glutPostRedisplay();
//     glutTimerFunc(25, update, 0);
//
// }

int main(int argc, char** argv) {

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

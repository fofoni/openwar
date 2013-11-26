#include <iostream>
#include <exception>

#ifdef __APPLE__
# include <OpenGL/OpenGL.h>
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

using namespace std;

float angle = 0;
float blue = .65;

class esc_except : public exception {
    virtual const char* what() const throw() {
        return "ESC key pressed";
    }
} escape;

void handle_keypress(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            throw escape;
    }
}

void init_render() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(.14, .18, .2, 1.0);
    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0); //Enable light #0
    glEnable(GL_LIGHT1); //Enable light #1
    glEnable(GL_NORMALIZE); //Automatically normalize normals
    glShadeModel(GL_SMOOTH); //Enable smooth shading
}

void handle_resize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        45,                    // camera angle
        double(w) / double(h), // width-to-height ratio
        1, 200                 // near and far z clipping coordinates
    );
}

void draw_scene() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -5);

    // ambient light
    GLfloat ambient_color[] = {.2, .2, .2, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_color);

    // positioned light
    GLfloat light_color0[] = {1.f, .5f, .5f, 1.f};
    GLfloat light_pos0[] = {0.f, 1.f, 0.f, 1.f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

    // Trapezoid
    glPushMatrix();
    glTranslatef(0, -1, 0);
    glRotatef(angle, 0, 0, 1);
    glBegin(GL_QUADS);

        glColor3f(0.5f, 0.0f, 1-blue);
        glVertex3f(-0.7f, -0.5f, 0.0f);
        glColor3f(0.0f, 0.9f, 0.0f);
        glVertex3f(0.7f, -0.5f, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.4f, 0.5f, 0.0f);
        glColor3f(0.0f, 0.65f, blue);
        glVertex3f(-0.4f, 0.5f, 0.0f);

    glEnd();
    glPopMatrix();

    // Pentagon
    glPushMatrix();
    glTranslatef(1, 1, 0);
    glRotatef(angle, 0, 1, 0);
    glScalef(.5, .5, .5);
    glBegin(GL_TRIANGLES);

        glVertex3f(-0.5f, -0.5f, 0.0f);
        glVertex3f(0.5f, -0.5f, 0.0f);
        glVertex3f(-0.5f, 0.0f, 0.0f);

        glVertex3f(-0.5f, 0.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);

        glVertex3f(-0.5f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.5f, 0.0f);

    glEnd();
    glPopMatrix();

    // Triangle
    glPushMatrix();
    glTranslatef(-1, 1, 0);
    glRotatef(angle, 1, 2, 3);
    glBegin(GL_TRIANGLES);

        glNormal3f(1.f, 0.f, 1.f);
        glVertex3f(0.5f, -0.5f, 0.0f);
        glNormal3f(0.f, 1.f, 1.f);
        glVertex3f(0.0f, 0.5f, 0.0f);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, 0.0f);

    glEnd();
    glPopMatrix();

    glutSwapBuffers();

}

void update(int value) {

    angle += 2;
    if (angle >= 180) angle -= 360;

    blue += .005;
    if (blue >= 1) blue -= 1;

    glutPostRedisplay();
    glutTimerFunc(25, update, 0);

}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);

    glutCreateWindow("OpenWAR");
    init_render();

    glutDisplayFunc(draw_scene);
    glutKeyboardFunc(handle_keypress);
    glutReshapeFunc(handle_resize);
    glutTimerFunc(25, update, 0);

    try {
        glutMainLoop();
    }
    catch (esc_except& e) {
        cout << "Bye." << endl;
    }

    return 0;

}

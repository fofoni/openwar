#include <iostream>
#include <exception>

#ifdef __APPLE__
# include <OpenGL/OpenGL.h>
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

using namespace std;

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

    glBegin(GL_QUADS);

        //Trapezoid
        glVertex3f(-0.7f, -1.5f, -5.0f);
        glVertex3f(0.7f, -1.5f, -5.0f);
        glVertex3f(0.4f, -0.5f, -5.0f);
        glVertex3f(-0.4f, -0.5f, -5.0f);

    glEnd();

    glBegin(GL_TRIANGLES);

        //Pentagon
        glVertex3f(0.5f, 0.5f, -5.0f);
        glVertex3f(1.5f, 0.5f, -5.0f);
        glVertex3f(0.5f, 1.0f, -5.0f);

        glVertex3f(0.5f, 1.0f, -5.0f);
        glVertex3f(1.5f, 0.5f, -5.0f);
        glVertex3f(1.5f, 1.0f, -5.0f);

        glVertex3f(0.5f, 1.0f, -5.0f);
        glVertex3f(1.5f, 1.0f, -5.0f);
        glVertex3f(1.0f, 1.5f, -5.0f);

        //Triangle
        glVertex3f(-0.5f, 0.5f, -5.0f);
        glVertex3f(-1.0f, 1.5f, -5.0f);
        glVertex3f(-1.5f, 0.5f, -5.0f);

    glEnd();

    glutSwapBuffers();

}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);

    //Create the window
    glutCreateWindow("OpenWAR");
    init_render();

    //Set handler functions for drawing, keypresses, and window resizes
    glutDisplayFunc(draw_scene);
    glutKeyboardFunc(handle_keypress);
    glutReshapeFunc(handle_resize);

    try {
        glutMainLoop();
    }
    catch (esc_except& e) {
        cout << "Bye." << endl;
    }

    return 0;

}

/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include <QtGui>
#include <QtOpenGL>

#include <cmath>

#include "Screen.h"
#include "loadpng.h"

Screen::Screen(QWidget *parent) :
    QGLWidget(parent), latitude(0), longitude(0), zoom(50), darkening(10),
    ARMY_HS_0(ARMY_HEIGHT * ARMY_HOLE_PERC),
    ARMY_HS_1(ARMY_HEIGHT * (1 - ARMY_HOLE_PERC)),
    wtm_width(1024), wtm_height(1024), wtg_width(1024), wtg_height(1024)
{

    WORLD_LAT_EPS = TAU/double(2*WORLD_LAT_QTD);
    WORLD_LONG_EPS = TAU/double(WORLD_LONG_QTD);
    ARMY_LONG_EPS = TAU/double(ARMY_LONG_QTD);

    color_red    = QColor::fromRgbF(.5 , 0  , 0  );
    color_green  = QColor::fromRgbF(0  , .5 , 0  );
    color_blue   = QColor::fromRgbF(0  , 0  , .5 );
    color_yellow = QColor::fromRgbF(.35, .35, 0  );
    color_white  = QColor::fromRgbF(.3 , .3 , .3 );
    color_black  = QColor::fromRgbF(.05, .05, .05);
    color_backgr = QColor::fromRgbF(.14, .18, .2 );

    // sphere vertices
    for (int j = 0; j <= WORLD_LAT_QTD; j++) {
        for (int i = 0; i <= WORLD_LONG_QTD; i++) {
            sph_vertices[i][j][0] = -sin(j*WORLD_LAT_EPS)*sin(i*WORLD_LONG_EPS);
            sph_vertices[i][j][1] = cos(j*WORLD_LAT_EPS);
            sph_vertices[i][j][2] = -sin(j*WORLD_LAT_EPS)*cos(i*WORLD_LONG_EPS);
        }
    }

    // army vertices
    for (int i = 0; i <= ARMY_LONG_QTD; i++) {
        army_vertices[i][0][0] = ARMY_RAD * cos(i*ARMY_LONG_EPS);
        army_vertices[i][0][1] = ARMY_RAD * sin(i*ARMY_LONG_EPS);
        army_vertices[i][1][0] = ARMY_RAD * cos(i*ARMY_LONG_EPS) * ARMY_IN_PERC;
        army_vertices[i][1][1] = ARMY_RAD * sin(i*ARMY_LONG_EPS) * ARMY_IN_PERC;
    }

    // load territories data
    {
        std::string line;
        char filename[] = "terr_db"; // TODO: make configurable at runtime
        std::ifstream file (filename);
        if (!file.is_open()) {
            throw cant_open_terr_db(std::string("Cannot open database file `") +
                                    filename + "'");
        }
        std::map<std::string, int> names;
        for (int count=0; getline(file, line); count++) {
            if (line.length() == 0) { count--; continue; }
            if (line[0] == '#') { count--; continue; }
            std::istringstream ss(line);
            std::string word;
            float x, y;
            ss >> word;
            names[word] = count;
            ss >> x >> y;
            graph.push_back(Terr(360*(x/1024-.5), 180*(.5-y/512), word));
            //graph[count].p = &(players[0]); // TODO: Qt GUI
        }
        file.clear(); file.seekg(0, file.beg); // for reading it again
        for (int count=0; getline(file, line); count++) {
            if (line.length() == 0) { count --; continue; }
            if (line[0] == '#') { count--; continue; }
            std::istringstream ss(line);
            std::string word;
            // discard first three words (name, x coord, y coord)
            ss >> word; ss >> word; ss >> word;
            while (ss >> word)
                graph[count].frontiers.push_back(names[word]);
        }
        file.close();
    }

}

Screen::~Screen()
{
}

QSize Screen::minimumSizeHint() const {
    return QSize(100, 100);
}

QSize Screen::sizeHint() const {
    return QSize(600, 600);
}

void Screen::initializeGL() {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(color_backgr.redF(), color_backgr.greenF(),
                 color_backgr.blueF(), 1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    world_tex_map = loadpng("imgs/earth_tex.png", wtm_width, wtm_height);
    world_tex_graph = loadpng("imgs/earth_graph.png", wtg_width, wtg_height);
    world_curr_tex = world_tex_map;

}

void Screen::resizeGL(int w, int h) {
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

void Screen::paintGL() {

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
    for (std::vector<Terr>::iterator it = graph.begin();
         it != graph.end(); ++it) {
        draw_armies(*it, 29);
    }

}

void Screen::mouseReleaseEvent(QMouseEvent *event) {
    QMessageBox msg_box;
    QString text;
    try { text = get_click_terr(event->x(), event->y())->name; }
    catch (clickoutside_except& e) { return; }
    msg_box.setText(text);
    msg_box.setWindowTitle("OpenWAR [clicked]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();
}

void Screen::keyReleaseEvent(QKeyEvent *event) {

    switch (event->key()) {
        case Qt::Key_A: case Qt::Key_Left:
            longitude -= -zoom/5;
            break;
        case Qt::Key_D: case Qt::Key_Right:
            longitude += -zoom/5;
            break;
        case Qt::Key_S: case Qt::Key_Down:
            latitude -= zoom/5;
            break;
        case Qt::Key_W: case Qt::Key_Up:
            latitude += zoom/5;
            break;
        case Qt::Key_V: case Qt::Key_Tab:
            if (world_curr_tex == world_tex_map)
                world_curr_tex = world_tex_graph;
            else
                world_curr_tex = world_tex_map;
            break;
        case Qt::Key_Z: // TODO: make possible to Ctrl+Plus
            if (zoom >= 10) zoom -= 5;
//            handle_resize(window_w, window_h);
            break;
        case Qt::Key_X: //                    and Ctrl+Minus
            if (zoom <= 170) zoom += 5;
//            handle_resize(window_w, window_h);
            break;
        case Qt::Key_K:
            if (darkening >= 1) darkening -= 1;
            break;
        case Qt::Key_L:
            if (darkening <= 9) darkening += 1;
            break;
        case Qt::Key_H: case Qt::Key_Home:
            longitude = latitude = 0;
            darkening = 10;
            zoom = 50;
            world_curr_tex = world_tex_map;
//            handle_resize(window_w, window_h);
            break;
    }

    updateGL();

}

/**********************************
************** SLOTS **************
**********************************/

void Screen::rot_left()  { longitude -= -zoom/5; updateGL(); }
void Screen::rot_right() { longitude += -zoom/5; updateGL(); }
void Screen::rot_down()  { latitude  -= +zoom/5; updateGL(); }
void Screen::rot_up()    { latitude  += +zoom/5; updateGL(); }

/**********************************************
************** Graphics routines **************
**********************************************/

void Screen::get_click_vec(const int alpha, const int beta,
                           double &x0, double &y0, double &z0) {

    double a = double(alpha - .5*width())/double(height());
    double b = .5 - double(beta)/double(height());
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

std::vector<Terr>::iterator Screen::get_click_terr(const int alpha,
                                                   const int beta) {
    double x0, y0, z0;
    get_click_vec(alpha, beta, x0, y0, z0);
    double closer_prod = -1; std::vector<Terr>::iterator pointer;
    for(std::vector<Terr>::iterator it = graph.begin();
        it != graph.end(); ++it) {
        double curr_prod = x0*it->x0 + y0*it->y0 + z0*it->z0;
        if (curr_prod > closer_prod) {
            closer_prod = curr_prod;
            pointer = it;
        }
    }
    return pointer;
}

// TODO: material
void Screen::draw_single_army(const QColor& c, float x, float y,
                              float h, bool big, float dx, float dy) {

    glPushMatrix();
    glRotatef(x, 0, 1, 0);
    glRotatef(-y, 1, 0, 0);
    glRotatef(dx, 0, 1, 0);
    glRotatef(-dy, 1, 0, 0);
    glTranslatef(0, 0, 1);
    if (big) glScalef(ARMY_BIG_PERC, ARMY_BIG_PERC, ARMY_BIG_PERC);
    glTranslatef(0, 0, h*ARMY_HEIGHT*ARMY_GHOST_PERC);

    glColor3f(c.redF(), c.greenF(), c.blueF());

    // draw lower cap
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 0, -1);
    glVertex3f(0, 0, ARMY_HS_0);
    for (int i = ARMY_LONG_QTD; i >= 0; i--) {
        glNormal3f(0, 0, -1);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], ARMY_HS_0);
    }
    glEnd();

    // draw lower wall
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= ARMY_LONG_QTD; i++) {
        glNormal3f(-army_vertices[i][1][0], -army_vertices[i][1][1], 0);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], 0);
        glNormal3f(-army_vertices[i][1][0], -army_vertices[i][1][1], 0);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], ARMY_HS_0);
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
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], ARMY_HS_1);
        glNormal3f(-army_vertices[i][1][0], -army_vertices[i][1][1], 0);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], ARMY_HEIGHT);
    }
    glEnd();

    // draw upper cap
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 0, 1);
    glVertex3f(0, 0, ARMY_HS_1);
    for (int i = 0; i <= ARMY_LONG_QTD; i++) {
        glNormal3f(0, 0, 1);
        glVertex3f(army_vertices[i][1][0], army_vertices[i][1][1], ARMY_HS_1);
    }
    glEnd();

    glColor3f(1, 1, 1); // reset color

    glPopMatrix();

}

void Screen::draw_armies(const Terr& terr, int qtd) {
    int quotient = qtd/5, remainder = qtd % 5;
    const QColor& c = terr.p->c.darker();
//    std::cout << r << " " << g << " " << b << std::endl;
    const float& x = terr.x;
    const float& y = terr.y;
    if (quotient == 0)
        switch (remainder) {
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
        switch (remainder) {
            case 0:
                for (int i = 0; i < quotient; i++)
                    draw_single_army(c,x,y, i, true);
                break;
            case 1:
                for (int i = 0; i < quotient; i++)
                    draw_single_army(c,x,y, i, true, -1.7, 0);
                draw_single_army(c,x,y, 0, false, 1.7, 0);
                break;
            case 2:
                for (int i = 0; i < quotient; i++)
                    draw_single_army(c,x,y, i, true, -1.55, 0);
                draw_single_army(c,x,y, 0, false, 1.55, -1.5);
                draw_single_army(c,x,y, 0, false, 1.55,  1.5);
                break;
            case 4:
                draw_single_army(c,x,y, 1, false,  .85, 0);
                // fall through
            case 3:
                for (int i = 0; i < quotient; i++)
                    draw_single_army(c,x,y, i, true,  -3.1, 0);
                draw_single_army(c,x,y, 0, false,    0, -1.5);
                draw_single_army(c,x,y, 0, false,    0,  1.5);
                draw_single_army(c,x,y, 0, false, 2.54, 0);
                break;
        }
}

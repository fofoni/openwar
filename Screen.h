/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <QGLWidget>

class Screen : public QGLWidget {

    Q_OBJECT

public:
    explicit Screen(QWidget *parent = 0);
    ~Screen();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    void init_render();
    void draw_scene();
    void handle_resize(int w, int h);
    void mousePressEvent(QMouseEvent *event);

private:
    /********************** colors **********************/

    QColor color_red;
    QColor color_green;
    QColor color_blue;
    QColor color_yellow;
    QColor color_white;
    QColor color_black;
    QColor color_background;

    /********************** definitions **********************/

    // distance from cam to center of the earth
    static const int CAMERA_DISTANCE = 3;

    // TODO: make these configurable at runtime (when we get Qt)
    static const int WORLD_LAT_QTD = 50;
    static const int WORLD_LONG_QTD = 100;

    static const int ARMY_LONG_QTD = 50; // num of sides of quantizing polygon
    static const float ARMY_HEIGHT = .0135; // height of outer cilinder
    static const float ARMY_RAD = .025; // radius of outer colinder
    static const float ARMY_IN_PERC = 0.70; // ratio inner/outer radii
    static const float ARMY_HOLE_PERC = 0.30; // ratio of heights hole/cilinder
    static const float ARMY_BIG_PERC = 1.25; // ratio big/common armies
    static const float ARMY_GHOST_PERC = 1.1;
    static const float ARMY_HS[] = { // heights
        ARMY_HEIGHT * ARMY_HOLE_PERC,
        ARMY_HEIGHT * (1-ARMY_HOLE_PERC)
    };
    static const float ARMY_HS_BIG[] = { // big heights // TODO: is this used?
        ARMY_HEIGHT * ARMY_HOLE_PERC * ARMY_BIG_PERC,
        ARMY_HEIGHT * (1-ARMY_HOLE_PERC) * ARMY_BIG_PERC
    };

    static const double WORLD_LAT_EPS = TAU/double(2*WORLD_LAT_QTD);
    static const double WORLD_LONG_EPS = TAU/double(WORLD_LONG_QTD);
    static const double ARMY_LONG_EPS = TAU/double(ARMY_LONG_QTD);

    /********************** variables **********************/

    // viewing
    int latitude = 0, longitude = 0; // coordinates of center of vision
    int zoom = 50; // camera angle in degrees
    int darkening = 10; // 0 is completely dark, 10 is normal.

    GLuint world_tex_map, world_tex_graph, world_curr_tex;
    int wtm_width = 1024, wtm_height = 1024,
        wtg_width = 1024, wtg_height = 1024;

    float sph_vertices[WORLD_LONG_QTD+1][WORLD_LAT_QTD+1][3];
    float army_vertices[ARMY_LONG_QTD+1][2][2];

};

#endif // SCREEN_H

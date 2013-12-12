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
#include <stdexcept>

#include "main.h"

class OpenWAR;

class clickoutside_except : public std::exception {
    virtual const char* what() const throw() {
        return "Clicked outside the window";
    }
};

extern clickoutside_except clicked_outside;

class cant_open_terr_db : public std::runtime_error {
public:
    cant_open_terr_db(std::string const& error) : std::runtime_error(error) {}
};

inline void make_perspective(GLdouble y_angle, GLdouble aspect_ratio,
                             GLdouble z_near, GLdouble z_far) {
    GLdouble ymax = z_near * tan(y_angle * TAU / 720.0);
    GLdouble ymin = -ymax;
    GLdouble xmin = ymin * aspect_ratio;
    GLdouble xmax = ymax * aspect_ratio;
    glFrustum(xmin, xmax, ymin, ymax, z_near, z_far);
}

class Screen : public QGLWidget {

    Q_OBJECT

public:
    explicit Screen(QWidget *parent = 0);
    ~Screen();

    std::vector<Terr> graph; // "map"

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void get_click_vec(const int alpha, const int beta,
                       double &x0, double &y0, double &z0);
    std::vector<Terr>::iterator get_click_terr(const int alpha, const int beta);

    void draw_single_army(const QColor &c, float x, float y,
                          float h=0, bool big=0,
                          float dx=0, float dy=false);
    void draw_armies(const Terr& terr, int qtd);

    QColor color_red;
    QColor color_green;
    QColor color_blue;
    QColor color_yellow;
    QColor color_white;
    QColor color_black;
    QColor color_backgr;

    // viewing
    int latitude, longitude; // coordinates of center of vision
    int zoom; // camera angle in degrees
    int darkening; // 0 is completely dark, 10 is normal.

    GLuint world_tex_map, world_tex_graph, world_curr_tex;

public slots:
    // screen control
    void rot_left();
    void rot_right();
    void rot_down();
    void rot_up();
    void zoom_in();
    void zoom_out();
    void toggle_view();
    void darken();
    void lighten();
    void reset_coords();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int h);
    void mouseReleaseEvent(QMouseEvent *event);

private:
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
    static const float ARMY_BIG_PERC = 1.4; // ratio big/common armies
    static const float ARMY_GHOST_PERC = 1.1;
    float ARMY_HS_0, ARMY_HS_1; // heights

    double WORLD_LAT_EPS;
    double WORLD_LONG_EPS;
    double ARMY_LONG_EPS;

    /********************** variables **********************/

    int wtm_width, wtm_height,
        wtg_width, wtg_height;

    float sph_vertices[WORLD_LONG_QTD+1][WORLD_LAT_QTD+1][3];
    float army_vertices[ARMY_LONG_QTD+1][2][2];

};

#endif // SCREEN_H

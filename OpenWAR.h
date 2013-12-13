/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

#ifndef OPENWAR_H
#define OPENWAR_H

#include <QtGui>
#include <QWidget>

#include "player.h"

#include "Game.h"
#include "Screen.h"

class OpenWAR : public QMainWindow {

    Q_OBJECT

public:
    explicit OpenWAR(QWidget *parent = 0);

    Game *game;

private slots:
    // file
    void newgame();
    void open();
    void save();
    void save_as();
    void quit();

    // edit
    void show_prefs_window();

    // help
    void show_help();
    void about_openwar();
    void about_qt();

private:
    QAction *newgame_act;
    QAction *open_act;
    QAction *save_act;
    QAction *save_as_act;
    QAction *quit_act;
    QAction *show_prefs_window_act;
    QAction *zoom_in_act;
    QAction *zoom_out_act;
    QAction *reset_coords_act;
    QAction *toggle_view_act;
    QAction *show_help_act;
    QAction *about_openwar_act;
    QAction *about_qt_act;

    QAction *rot_left_act;
    QAction *rot_right_act;
    QAction *rot_down_act;
    QAction *rot_up_act;
    QAction *darken_act;
    QAction *lighten_act;

    QMenu *file_menu;
    QMenu *edit_menu;
    QMenu *view_menu;
    QMenu *help_menu;

    QToolBar *toolbar;

    QDockWidget *ctrl_dock;

    QListWidget *func_listwidget;
    Screen *screen;

};

#endif // OPENWAR_H

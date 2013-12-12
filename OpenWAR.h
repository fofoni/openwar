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

class Screen;

class OpenWAR : public QMainWindow {

    Q_OBJECT

public:
    explicit OpenWAR(QWidget *parent = 0);

    std::vector<Terr> graph; // "map"
    std::vector<Player> players;

private slots:
    // file
    void open();
    void save();
    void save_as();
    void quit();

    // edit
    void show_prefs_window();

    // view
    void zoom_in();
    void zoom_out();
    void reset_coords();

    // help
    void show_help();
    void about_openwar();
    void about_qt();

private:
    QAction *open_act;
    QAction *save_act;
    QAction *save_as_act;
    QAction *quit_act;
    QAction *show_prefs_window_act;
    QAction *zoom_in_act;
    QAction *zoom_out_act;
    QAction *reset_coords_act;
    QAction *show_help_act;
    QAction *about_openwar_act;
    QAction *about_qt_act;

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

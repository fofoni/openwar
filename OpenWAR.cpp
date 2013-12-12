/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

#include <QtGui>

#include "Screen.h"
#include "OpenWAR.h"
#include <iostream>

OpenWAR::OpenWAR(QWidget *parent) :
    QMainWindow(parent)
{

    /*
     * ACTIONS
     *
     */

    // open
    open_act = new QAction(QIcon::fromTheme("document-open"),
                           "&Open", this);
    open_act->setShortcuts(QKeySequence::Open);
    open_act->setStatusTip("Open existing savegame.");
    connect(open_act, SIGNAL(triggered()), this, SLOT(open()));

    // save
    save_act = new QAction(QIcon::fromTheme("document-save"),
                           "&Save", this);
    save_act->setShortcuts(QKeySequence::Save);
    save_act->setStatusTip("Save game state");
    connect(save_act, SIGNAL(triggered()), this, SLOT(save()));

    // save as
    save_as_act = new QAction(QIcon::fromTheme("document-save-as"),
                              "Save &As", this);
    save_as_act->setShortcuts(QKeySequence::SaveAs);
    save_as_act->setStatusTip(
        "Save game state in a new file, without overwriting the existing one");
    connect(save_as_act, SIGNAL(triggered()), this, SLOT(save_as()));

    // quit
    quit_act = new QAction(QIcon::fromTheme("application-exit"),
                           "Quit", this);
    quit_act->setShortcuts(QKeySequence::Quit);
    connect(quit_act, SIGNAL(triggered()), this, SLOT(quit()));

    // preferences
    show_prefs_window_act = new QAction("Preferences", this);
    show_prefs_window_act->setShortcuts(QKeySequence::Preferences);
    connect(show_prefs_window_act, SIGNAL(triggered()),
            this, SLOT(show_prefs_window()));

    // zoom in
    zoom_in_act = new QAction(QIcon::fromTheme("zoom-in"),
                              "&Zoom in", this);
    QList<QKeySequence> zoom_in_shortcut;
    zoom_in_shortcut << QKeySequence::ZoomIn << QKeySequence(Qt::Key_Z);
    zoom_in_act->setShortcuts(zoom_in_shortcut);

    // zoom out
    zoom_out_act = new QAction(QIcon::fromTheme("zoom-out"),
                               "Zoom &out", this);
    QList<QKeySequence> zoom_out_shortcut;
    zoom_out_shortcut << QKeySequence::ZoomOut << QKeySequence(Qt::Key_X);
    zoom_out_act->setShortcuts(zoom_out_shortcut);

    // home
    reset_coords_act = new QAction(QIcon::fromTheme("zoom-original"),
                                   "&Reset coordinates", this);
    reset_coords_act->setStatusTip(
        "Get viewport back at default coordinates and world color");
    QList<QKeySequence> reset_coords_shortcut;
    reset_coords_shortcut << QKeySequence(Qt::Key_Home)
                          << QKeySequence(Qt::Key_H);
    reset_coords_act->setShortcuts(reset_coords_shortcut);

    // toggle graph/frontiers
    toggle_view_act = new QAction("&Toggle view", this);
    QList<QKeySequence> toggle_shortcut;
    toggle_shortcut << QKeySequence(Qt::Key_Tab) << QKeySequence(Qt::Key_V);
    toggle_view_act->setShortcuts(toggle_shortcut);

    // help
    show_help_act = new QAction(QIcon::fromTheme("help-contents"),
                                "&Manual", this);
    show_help_act->setShortcuts(QKeySequence::HelpContents);
    connect(show_help_act, SIGNAL(triggered()), this, SLOT(show_help()));

    // about
    about_openwar_act = new QAction(QIcon::fromTheme("help-about"),
                                    "&About OpenWAR", this);
    connect(about_openwar_act, SIGNAL(triggered()),
            this, SLOT(about_openwar()));

    // about qt
    about_qt_act = new QAction("About &Qt", this);
    connect(about_qt_act, SIGNAL(triggered()), this, SLOT(about_qt()));

    /*
     * MENU BAR
     *
     */

    file_menu = menuBar()->addMenu("&File");
    file_menu->addAction(open_act);
    file_menu->addAction(save_act);
    file_menu->addAction(save_as_act);
    file_menu->addSeparator();
    file_menu->addAction(quit_act);

    edit_menu = menuBar()->addMenu("&Edit");
    edit_menu->addAction(show_prefs_window_act);

    view_menu = menuBar()->addMenu("&View");
    view_menu->addAction(zoom_in_act);
    view_menu->addAction(zoom_out_act);
    view_menu->addSeparator();
    view_menu->addAction(reset_coords_act);
    view_menu->addSeparator();
    view_menu->addAction(toggle_view_act);

    help_menu = menuBar()->addMenu("&Help");
    help_menu->addAction(show_help_act);
    help_menu->addSeparator();
    help_menu->addAction(about_openwar_act);
    help_menu->addAction(about_qt_act);

    /*
     * TOOL BAR
     *
     */

    toolbar = addToolBar("Toolbar");
    toolbar->addAction(open_act);
    toolbar->addAction(save_act);
    toolbar->addSeparator();
    toolbar->addAction(zoom_in_act);
    toolbar->addAction(zoom_out_act);
    toolbar->addAction(reset_coords_act);
    toolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea |
                             Qt::RightToolBarArea);

    /*
     * STATUS BAR
     *
     */

    statusBar()->showMessage(QString("OpenWAR"));

    /*
     * CONTROL DOCK
     *
     */

    ctrl_dock = new QDockWidget("Control", this);
    ctrl_dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    ctrl_dock->setFeatures(QDockWidget::DockWidgetMovable |
                           QDockWidget::DockWidgetFloatable);
    func_listwidget = new QListWidget(ctrl_dock);
    // example function
    /// FAZER SLOT ADD_FUNC
    ctrl_dock->setWidget(func_listwidget);
    addDockWidget(Qt::LeftDockWidgetArea, ctrl_dock);

    /*
     * SCREEN
     *
     */

    screen = new Screen;
    this->setCentralWidget(screen);

    setWindowTitle("OpenWAR");

    rot_left_act = new QAction("Rotate left", this);
    QList<QKeySequence> left_shortcut;
    left_shortcut << QKeySequence(Qt::Key_Left) << QKeySequence(Qt::Key_A);
    rot_left_act->setShortcuts(left_shortcut);
    connect(rot_left_act, SIGNAL(triggered()), screen, SLOT(rot_left()));
    this->addAction(rot_left_act);

    rot_right_act = new QAction("Rotate right", this);
    QList<QKeySequence> right_shortcut;
    right_shortcut << QKeySequence(Qt::Key_Right) << QKeySequence(Qt::Key_D);
    rot_right_act->setShortcuts(right_shortcut);
    connect(rot_right_act, SIGNAL(triggered()), screen, SLOT(rot_right()));
    this->addAction(rot_right_act);

    rot_down_act = new QAction("Rotate down", this);
    QList<QKeySequence> down_shortcut;
    down_shortcut << QKeySequence(Qt::Key_Down) << QKeySequence(Qt::Key_S);
    rot_down_act->setShortcuts(down_shortcut);
    connect(rot_down_act, SIGNAL(triggered()), screen, SLOT(rot_down()));
    this->addAction(rot_down_act);

    rot_up_act = new QAction("Rotate up", this);
    QList<QKeySequence> up_shortcut;
    up_shortcut << QKeySequence(Qt::Key_Up) << QKeySequence(Qt::Key_W);
    rot_up_act->setShortcuts(up_shortcut);
    connect(rot_up_act, SIGNAL(triggered()), screen, SLOT(rot_up()));
    this->addAction(rot_up_act);

    connect(zoom_in_act, SIGNAL(triggered()), screen, SLOT(zoom_in()));
    connect(zoom_out_act, SIGNAL(triggered()), screen, SLOT(zoom_out()));
    connect(toggle_view_act, SIGNAL(triggered()), screen, SLOT(toggle_view()));

    darken_act = new QAction("Darker Earth", this);
    QList<QKeySequence> darken_shortcut;
    darken_shortcut << QKeySequence(Qt::Key_K);
    darken_act->setShortcuts(darken_shortcut);
    connect(darken_act, SIGNAL(triggered()), screen, SLOT(darken()));
    this->addAction(darken_act);

    lighten_act = new QAction("lighter Earth", this);
    QList<QKeySequence> lighten_shortcut;
    lighten_shortcut << QKeySequence(Qt::Key_L);
    lighten_act->setShortcuts(lighten_shortcut);
    connect(lighten_act, SIGNAL(triggered()), screen, SLOT(lighten()));
    this->addAction(lighten_act);

    connect(reset_coords_act, SIGNAL(triggered()),
            screen, SLOT(reset_coords()));

    players.push_back(Player(std::string("p0"), screen->color_red));
    players.push_back(Player(std::string("p1"), screen->color_green));
    players.push_back(Player(std::string("p2"), screen->color_blue));
    players.push_back(Player(std::string("p3"), screen->color_yellow));
    players.push_back(Player(std::string("p4"), screen->color_white));
    players.push_back(Player(std::string("p5"), screen->color_black));

    for (std::vector<Terr>::iterator it = screen->graph.begin();
         it != screen->graph.end(); ++it)
        it->p = &(players[0]);

}

void OpenWAR::open() {
    QMessageBox msg_box;
    msg_box.setText("Not implemented yet");
    msg_box.setWindowTitle("OpenWAR [info]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();
}

void OpenWAR::save() {
    QMessageBox msg_box;
    msg_box.setText("Not implemented yet");
    msg_box.setWindowTitle("OpenWAR [info]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();
}

void OpenWAR::save_as() {
    QMessageBox msg_box;
    msg_box.setText("Not implemented yet");
    msg_box.setWindowTitle("OpenWAR [info]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();
}

void OpenWAR::quit() {
    qApp->quit();
}

void OpenWAR::show_prefs_window() {
    QMessageBox msg_box;
    msg_box.setText("Not implemented yet");
    msg_box.setWindowTitle("OpenWAR [info]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();
}

void OpenWAR::zoom_in() {
    QMessageBox msg_box;
    msg_box.setText("Not implemented yet");
    msg_box.setWindowTitle("OpenWAR [info]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();
}

void OpenWAR::zoom_out() {
    QMessageBox msg_box;
    msg_box.setText("Not implemented yet");
    msg_box.setWindowTitle("OpenWAR [info]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();
}

void OpenWAR::reset_coords() {
    QMessageBox msg_box;
    msg_box.setText("Not implemented yet");
    msg_box.setWindowTitle("OpenWAR [info]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();
}

void OpenWAR::show_help() {
    QMessageBox msg_box;
    msg_box.setText("Not implemented yettt");
    msg_box.setWindowTitle("OpenWAR [info]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();
}

void OpenWAR::about_openwar() {
    QMessageBox msg_box;
    msg_box.setText("Not implemented yet");
    msg_box.setWindowTitle("OpenWAR [info]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();
}

void OpenWAR::about_qt() {
    QMessageBox msg_box;
    msg_box.setText("Not implemented yet");
    msg_box.setWindowTitle("OpenWAR [info]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();
}

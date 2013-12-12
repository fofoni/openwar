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

OpenWAR::OpenWAR(QWidget *parent) :
    QMainWindow(parent)
{

    /*
     * ACTIONS
     *
     */

    open_act = new QAction(QIcon::fromTheme("document-open"),
                           "&Open", this);
    open_act->setShortcuts(QKeySequence::Open);
    open_act->setStatusTip(trUtf8("Open existing savegame."));
    connect(open_act, SIGNAL(triggered()), this, SLOT(open()));

    save_act = new QAction(QIcon::fromTheme("document-save"),
                           "&Save", this);
    save_act->setShortcuts(QKeySequence::Save);
    save_act->setStatusTip(trUtf8("Save game state"));
    connect(save_act, SIGNAL(triggered()), this, SLOT(save()));

    save_as_act = new QAction(QIcon::fromTheme("document-save-as"),
                              "Save &As", this);
    save_as_act->setShortcuts(QKeySequence::SaveAs);
    save_as_act->setStatusTip(
        "Save game state in a new file, without overwriting the existing one");
    connect(save_as_act, SIGNAL(triggered()), this, SLOT(save_as()));

    quit_act = new QAction(QIcon::fromTheme("application-exit"),
                           "Quit", this);
    quit_act->setShortcuts(QKeySequence::Quit);
    connect(quit_act, SIGNAL(triggered()), this, SLOT(quit()));

    show_prefs_window_act = new QAction("Preferences", this);
    show_prefs_window_act->setShortcuts(QKeySequence::Preferences);
    connect(show_prefs_window_act, SIGNAL(triggered()),
            this, SLOT(show_prefs_window()));

    zoom_in_act = new QAction(QIcon::fromTheme("zoom-in"),
                              "&Zoom in", this);
    zoom_in_act->setShortcuts(QKeySequence::ZoomIn);
    connect(zoom_in_act, SIGNAL(triggered()), this, SLOT(zoom_in()));

    zoom_out_act = new QAction(QIcon::fromTheme("zoom-out"),
                               "Zoom &out", this);
    zoom_out_act->setShortcuts(QKeySequence::ZoomOut);
    connect(zoom_out_act, SIGNAL(triggered()), this, SLOT(zoom_out()));

    reset_coords_act = new QAction(QIcon::fromTheme("zoom-original"),
                                   "&Reset coordinates", this);
    reset_coords_act->setStatusTip(
        "Get viewport back at default coordinates and world color");
    connect(reset_coords_act, SIGNAL(triggered()), this, SLOT(reset_coords()));

    show_help_act = new QAction(QIcon::fromTheme("help-contents"),
                                "&Manual", this);
    show_help_act->setShortcuts(QKeySequence::HelpContents);
    connect(show_help_act, SIGNAL(triggered()), this, SLOT(show_help()));

    about_openwar_act = new QAction(QIcon::fromTheme("help-about"),
                                  trUtf8("&About OpenWAR"), this);
    connect(about_openwar_act, SIGNAL(triggered()),
            this, SLOT(about_openwar()));

    about_qt_act = new QAction(trUtf8("About &Qt"), this);
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

    ctrl_dock = new QDockWidget(trUtf8("Funções"), this);
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
    msg_box.setText("Not implemented yet");
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

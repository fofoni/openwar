/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

#include <sstream>

#include <QtGui>

#include "Screen.h"
#include "OpenWAR.h"
#include "newgamedialog.h"
#include "colors.h"

OpenWAR::OpenWAR(QWidget *parent) :
    QMainWindow(parent), game(NULL)
{

    /*
     * ACTIONS
     *
     */

    // new
    newgame_act = new QAction(QIcon::fromTheme("document-new"), "&New Game",
                              this);
    newgame_act->setShortcuts(QKeySequence::New);
    newgame_act->setToolTip("Start a new game.");
    connect(newgame_act, SIGNAL(triggered()), this, SLOT(newgame()));

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
    file_menu->addAction(newgame_act);
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
    toolbar->addAction(newgame_act);
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
     * SCREEN
     *
     */

    screen = new Screen(this);
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

}

void OpenWAR::newgame() {

    if (game) {
        QMessageBox msg_box;
        msg_box.setText(
            "Are you sure you want to stop and discard the current game?"
        );
        msg_box.setWindowTitle("OpenWAR [are you sure?]");
        msg_box.setIcon(QMessageBox::Warning);
        msg_box.addButton(QMessageBox::Yes);
        msg_box.addButton(QMessageBox::No);
        msg_box.exec();
        if (msg_box.result() == QMessageBox::No) return;
    }

    NewGameDialog *form_dialog = new NewGameDialog(this);
    int num_players;
    QList<QString> player_names;
    QList<QColor> player_colors;
    if (! form_dialog->run(num_players, player_names, player_colors))
        return;
    screen->destroy_armies();
    if (game) delete game;
    game = new Game(screen->graph, num_players, player_names, player_colors);

    QMessageBox msg_box;
    std::stringstream ss;
    ss << "Ordem: ";
    ss << game->players[0].html();
    for (int i = 1; i < num_players; i++) {
        // make more informative: one line for each player, and number of terrs in parenthesis
        ss << ", " << game->players[i].html();
    }
    ss << ".";
    msg_box.setText(ss.str().c_str());
    msg_box.setWindowTitle("OpenWAR [info]");
    msg_box.setIcon(QMessageBox::Information);
    msg_box.exec();

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

void OpenWAR::show_help() {
    QMessageBox msg_box(this);
    std::stringstream ss;
    ss << "<font size='20'><center><b>OpenWAR</b></center></font><br /><br />";
    ss << "<b>Para visualizar o globo:</b><br />";
    ss << "- Teclas de \"setinhas\" andam pelo globo (alternativamente, teclas ASDW)<br />";
    ss << "- Teclas Z e X fazem zoom in e out<br />";
    ss << "- Tecla TAB (ou alternativamente, V) muda o tipo de mapa<br />";
    ss << "- Teclas K e L deixam o modelo do globo mais escuro ou mais claro (o que pode ajudar a ver melhor as pecinhas)<br />";
    ss << "- Tecla H volta para a posicao/mapa/cor normal<br />";
    ss << "- Outros shortcuts podem ser vistos no menu View<br /><br />";
    ss << "<b>Para iniciar um novo jogo:</b><br />";
    ss << "- Clique em File -> New<br />";
    ss << "- Escolha os nomes e cores dos jogadores<br />";
    ss << "- Os jogadores vao ser ordenados aleatoriamente<br /><br />";
    ss << "<b>A unica coisa que e possivel fazer nessa versao do jogo eh:</b><br />";
    ss << "- Clique em um territorio para adicionar exercitos a ele.<br />";
    ss << "- Lembre-se de que uma pecinha grande vale cinco pecinhas pequenas";
    msg_box.setText(ss.str().c_str());
    msg_box.setWindowTitle("OpenWAR [help]");
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
    QMessageBox::aboutQt(this);
}

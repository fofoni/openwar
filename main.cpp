/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

/*****************************************
************** STL includes **************
*****************************************/

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>

#include <vector>
#include <map>

#include <exception>
#include <stdexcept>

/******************************************
************** math includes **************
******************************************/

#include <cmath>

/****************************************
************** Qt includes **************
****************************************/

#include <QtGui>

/*******************************************
************** local includes **************
*******************************************/

#include "main.h"
#include "loadpng.h"
#include "OpenWAR.h"

/**************************************
************** main prog **************
**************************************/

int main(int argc, char** argv) {

    std::cout << "OpenWAR" << std::endl ;//<< OpenWAR_VERSION_STR << endl;
    std::cout << std::endl;

    QApplication app(argc, argv);
    OpenWAR window;
    window.resize(window.sizeHint());
    window.show();

    return app.exec();

    // TODO: Qt GUI
    /*players.push_back(Player(string("p0"), Color::dark_cyan));
    players.push_back(Player(string("p1"), Color::dark_magenta));
    players.push_back(Player(string("p2"), Color::dark_yellow));
    players.push_back(Player(string("p3"), Color::dark_red));
    players.push_back(Player(string("p4"), Color::dark_green));
    players.push_back(Player(string("p5"), Color::dark_blue));*/

}

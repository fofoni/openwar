/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

/**
  * TODO: try C++11 and Qt5. Add Doxygen comments. Get doxygen in github-pages.
  *       make "install" targets (we currently have only a "build" target), The
  *       install target must ask for a location to put the textures and DBs,
  *       we must pass this same location to the compiler.
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

}

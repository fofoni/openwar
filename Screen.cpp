/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

#include <QtGui>
#include <QtOpenGL>

#include <cmath>

#include "Screen.h"

Screen::Screen(QWidget *parent) :
    QGLWidget(parent)
{

    color_red        = QColor::fromRgbF(.5 , 0  , 0  );
    color_green      = QColor::fromRgbF(0  , .5 , 0  );
    color_blue       = QColor::fromRgbF(0  , 0  , .5 );
    color_yellow     = QColor::fromRgbF(.35, .35, 0  );
    color_white      = QColor::fromRgbF(.3 , .3 , .3 );
    color_black      = QColor::fromRgbF(.05, .05, .05);
    color_background = QColor::fromRgbF(.14, .18, .2 );

}

Screen::~Screen()
{
}

QSize Screen::minimumSizeHint() const {
    return QSize(100, 100);
}

QSize Screen::sizeHint() const {
    return QSize(700, 700);
}

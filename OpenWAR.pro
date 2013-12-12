#
# Universidade Federal do Rio de Janeiro
# Trabalho de Introdução à Computação Gráfica (EEL882)
# Semestre 2013.2
# Professor: Cláudio Esperança
# Aluno: Pedro Angelo Medeiros Fonini
#

SOURCES      = main.cpp \
               OpenWAR.cpp \
               Screen.cpp \
               loadpng.cpp
HEADERS      = main.h \
               OpenWAR.h \
               Screen.h \
               loadpng.h
QT          += opengl
LIBS        += -lpng

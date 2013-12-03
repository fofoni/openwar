#
# Universidade Federal do Rio de Janeiro
# Trabalho de Introdução à Computação Gráfica (EEL882)
# Semestre 2013.2
# Professor: Cláudio Esperança
# Aluno: Pedro Angelo Medeiros Fonini
#

OS=$(shell bash os_test.sh)

CC = g++
CFLAGS = -Wall -Wextra -pedantic -O2 -march=native
PROG = openwar
SRCS = main.cpp loadpng.cpp
LIBS = -lglut -lGLU -lpng
ifeq ($(OS),fedora)
    LIBS += -lGL
endif
HEADS = loadpng.h

TEXTURE_MAIN = imgs/earth_tex.png
XCF_MAIN = imgs/earth_map.xcf
TEXTURE_GRAPH = imgs/earth_graph.png
XCF_GRAPH = imgs/earth_graph.xcf

all : $(PROG) $(TEXTURE_MAIN) $(TEXTURE_GRAPH)

$(PROG) : $(SRCS) $(HEADS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

$(TEXTURE_MAIN) : $(XCF_MAIN)
	xcf2png $(XCF_MAIN) -o temp.png
	convert temp.png -resize "1024x1024!" $(TEXTURE_MAIN)
	rm -f temp.png

$(TEXTURE_GRAPH) : $(XCF_GRAPH)
	xcf2png $(XCF_GRAPH) -o temp.png
	convert temp.png -resize "1024x1024!" $(TEXTURE_GRAPH)
	rm -f temp.png

clean :
	rm -f $(PROG)

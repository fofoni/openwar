#
# Universidade Federal do Rio de Janeiro
# Trabalho de Introdução à Computação Gráfica (EEL882)
# Semestre 2013.2
# Professor: Cláudio Esperança
# Aluno: Pedro Angelo Medeiros Fonini
#

OS=$(shell bash os_test.sh)
VERSION_STR=$(shell bash get-git-version.sh .)

CC = g++
CFLAGS = -Wall -Wextra -pedantic -O2 -march=native \
         -D OpenWAR_VERSION=$(VERSION_STR)
PROG = openwar
SRCS = main.cpp loadpng.cpp
LIBS = -lglut -lGLU -lpng
ifeq ($(OS),fedora)
    LIBS += -lGL
endif
ifeq ($(OS),redhat)
    LIBS += -lGL
endif
HEADS = loadpng.h main.h
SCRIPTS = get-git-version.sh os_test.sh

define SCILIN_TEXT
Scientific Linux doens't currently support xcftools. Please run `make $(PROG)'
    instead of just `make' in order to avoid errors.

endef
ifeq ($(OS),scientific-linux)
    SCILIN_WARN=$(warning $(SCILIN_TEXT))
else
    SCILIN_WARN=
endif


TEXTURE_MAIN = imgs/earth_tex.png
XCF_MAIN = imgs/earth_map.xcf
TEXTURE_GRAPH = imgs/earth_graph.png
XCF_GRAPH = imgs/earth_graph.xcf

all : $(PROG) $(TEXTURE_MAIN) $(TEXTURE_GRAPH)

$(PROG) : $(SRCS) $(HEADS) $(SCRIPTS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

$(TEXTURE_MAIN) : $(XCF_MAIN)
	$(SCILIN_WARN)
	xcf2png $(XCF_MAIN) -o temp.png
	convert temp.png -resize "1024x1024!" $(TEXTURE_MAIN)
	rm -f temp.png

$(TEXTURE_GRAPH) : $(XCF_GRAPH)
	$(SCILIN_WARN)
	xcf2png $(XCF_GRAPH) -o temp.png
	convert temp.png -resize "1024x1024!" $(TEXTURE_GRAPH)
	rm -f temp.png

clean :
	rm -f $(PROG)

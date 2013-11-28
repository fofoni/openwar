#
# Universidade Federal do Rio de Janeiro
# Trabalho de Introdução à Computação Gráfica (EEL882)
# Semestre 2013.2
# Professor: Cláudio Esperança
# Aluno: Pedro Angelo Medeiros Fonini
#

CC = g++
CFLAGS = -Wall -Wextra -pedantic -O2 -march=native
PROG = openwar
SRCS = main.cpp loadpng.cpp
LIBS = -lglut -lGLU -lpng
HEADS = loadpng.h

TEXTURE_MAIN = earth_tex.png
XCF_MAIN = earth_map.xcf

all : $(PROG) $(TEXTURE_MAIN)

$(PROG) : $(SRCS) $(HEADS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

$(TEXTURE_MAIN) : $(XCF_MAIN)
	xcf2png $(XCF_MAIN) -o temp.png
	convert temp.png -resize "1024x1024!" $(TEXTURE_MAIN)
	rm -f temp.png

clean :
	rm -f $(PROG)

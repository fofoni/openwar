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

all : $(PROG)

$(PROG) : $(SRCS) $(HEADS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean :
	rm -f $(PROG)

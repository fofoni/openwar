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

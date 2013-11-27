CC = g++
CFLAGS = -Wall -Wextra -pedantic -O2 -march=native
PROG = openwar
SRCS = main.cpp loadpng.cpp
LIBS = -lglut -lGLU -lpng

all : $(PROG)

$(PROG) : $(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean :
	rm -f $(PROG)

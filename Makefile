#
#  Makefile for splines
#

CC     = g++
CFLAGS = -pipe -fpic -O3 -DSHM

INCLUDES =-I/usr/share/src/OpenGL/libaux

GL_LIBS = -lGLU -lGL -L/usr/share/src/OpenGL/libaux -laux
X11_LIBS = -lX11 -lXext
EXTRA_LIBS = -lm

tester:main.o spline.o
	$(CC) $(INCLUDES) $(CFLAGS) main.o spline.o $(GL_LIBS) $(X11_LIBS) $(EXTRA_LIBS) -o tester

main.o:main.cpp
	$(CC) $(INCLUDES) -c main.cpp

spline.o:spline.cpp
	$(CC) $(INCLUDES) -c spline.cpp

spline.cpp:spline.h
	touch spline.cpp

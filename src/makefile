CC=g++
EMCC=emcc

#CFLAGS= -g -Wall -w -std=c++11
CFLAGS= -std=c++11
INCLUDE= -I/usr/local/include
#LIBS= -lGL -lGLU -lglut -lSDL
LIBS= -lSDL
MAIN_SDL= main.cpp
MAIN_GL= main_opengl1x.cpp
SOURCES=  QBall.cpp  QImplicitSurface.cpp  QPlane.cpp  QTriangle.cpp  QVector.cpp  ViewingParams.cpp
PRELOAD=data.txt
EXEC=demo
#EXEC=demo.js

OUT=demo.html
EMCC_OPTS = --bind -s FULL_ES2=1 -O3
#EMCC_OPTS=-s LEGACY_GL_EMULATION=1 -s GL_UNSAFE_OPTS=0
all:	
	$(CC) $(CFLAGS) -O3 $(MAIN_GL) $(SOURCES) $(INCLUDE) -lGL -lGLU -lglut -o $(EXEC)
	
sdl:	
	$(CC) $(CFLAGS) -O3 $(MAIN_SDL) $(SOURCES) $(INCLUDE) $(LIBS) -o $(EXEC)

js:	
	$(EMCC) $(CFLAGS) $(MAIN_GL) $(SOURCES) $(EMCC_OPTS) \
	--preload-file $(PRELOAD) -o $(OUT)

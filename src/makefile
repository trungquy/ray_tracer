CC=g++
EMCC=emcc

#CFLAGS= -g -Wall -w -std=c++11
CFLAGS= -std=c++11
INCLUDE= -I/usr/local/include
#LIBS= -lGL -lGLU -lglut -lSDL
LIBS= -lSDL
SOURCES=  main.cpp  QBall.cpp  QImplicitSurface.cpp  QPlane.cpp  QTriangle.cpp  QVector.cpp  ViewingParams.cpp
PRELOAD=data.txt
EXEC=demo
#EXEC=demo.js

OUT=demo.html
EMCC_OPTS = --bind -s FULL_ES2=1 -O2
#EMCC_OPTS=-s LEGACY_GL_EMULATION=1 -s GL_UNSAFE_OPTS=0
all:	
	$(CC) $(CFLAGS) $(SOURCES) $(INCLUDE) $(LIBS) -o $(EXEC)
js:	
	$(EMCC) $(CFLAGS) $(SOURCES) $(EMCC_OPTS) \
	--preload-file $(PRELOAD) -o $(OUT)
c11:
	g++ -std=c++11 main.cpp -o demo -I/usr/local/include \
	-L/usr/local/lib

# ray_tracer
I wrote this ray tracer by C++ for computer graphic class project in 2013. Recently, I improve it a bit(Use libSDL instead of OpenGL to render the result) for my experiment with [Emscripten](http://emscripten.org). Check out my results by click the bellow link

[Online Demo](http://trungquy.github.io/ray_tracer/demo.html)

For Ubuntu users:
Compiling Requirements: (MakeFile is provided for native and JS compiling)
    - LibSDL1.x
    - C++11 support compiler
    - Emscripten for C++ to JS convert
How to comile source:
    - To native code: 
        1. cd ./src/
        2. make 
    - To JavaScript code:
        1. cd ./src/
        2. make js
Quy Nguyen

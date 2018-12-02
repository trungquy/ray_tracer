# ray_tracer

A ray tracing implementation support:

- Objects: Ball, triangle, implicit surface
- Features: shadowing, reflection, refraction, anti-aliasing

Demo: [click here](http://trungquy.github.io/ray_tracer/)

## Requirements

- LibSDL1.x or freeglut3-dev
- C++11 support compiler
- Emscripten for converting C++ to asm

## Compilation

MakeFile is provided for native and JS compiling

- To native code:
``` 
        1. cd ./src/
        2. make 
```
- To JavaScript code:
```
        1. cd ./src/
        2. make js
```

## Todo

- Replace libsdl by opengl
- WebAssembly version, fall back to javascript
- Nagivate in space with keyboard
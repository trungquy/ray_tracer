#ifndef DEFINE_H
#define DEFINE_H
#ifndef __unix
#define M_PI 3.141592653589793
#define INFINITY 1e8
#endif

#define MIN_DARK 0.1
#define N_HL 3
#define HIGHLIGHT_MIN 0.9
#define EPSILON pow(10.0,-10)
#define SHADOW 0.8
#define DEPTH_MAX 7
#define AIR_ETA 1
// #define ANTI_ALIASING 1
#define ANTI_ALIASING 0
#define NUM_SUB_ANTI_ALIASING 2

// Colors
#define COLOR_WHITE		0xffffff
#define COLOR_BLACK		0x000000
#define COLOR_CHECKBORAD1 0xFF692D
#define COLOR_SKY		0x87CEEB

#define OBJ_TYPE_OBJ 0
#define OBJ_TYPE_PLANE 1
#define OBJ_TYPE_BALL 2
#define OBJ_TYPE_IMPLICIT 3
#define OBJ_TYPE_TRIANGLE 4
#endif
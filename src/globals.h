#ifndef EXTERNALS_H

#include <stdbool.h>

#define EXTERNALS_H

#define MAX_PLAYERS     100
#define MAX_COMPLEX_IA  200
#define MAX_SIMPLE_IA   1000
#define MAX_PROPS       2000
#define MAX_TILES       15000

#define MS_PER_UPDATE 16

#define TILESIZE    16

#define BLACK 0,0,0,255
#define WHITE 255,255,255,255
#define RED 255,0,0,255
#define GREEN 0,255,0,255
#define BLUE 0,0,255,255

#define MAX_ENTITYS (MAX_PLAYERS + MAX_COMPLEX_IA + MAX_SIMPLE_IA + MAX_PROPS + MAX_TILES)

extern bool debug;

#endif

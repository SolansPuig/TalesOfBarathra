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

#define BLACK 0,0,0,100
#define WHITE 255,255,255,100
#define RED 255,0,0,100
#define GREEN 0,255,0,100
#define BLUE 0,0,255,100

#define MAX_ENTITYS 5000

extern bool debug;

#endif

#ifndef EXTERNALS_H
#define EXTERNALS_H

#include <stdbool.h>
#define MAX_PLAYERS     100
#define MAX_COMPLEX_IA  200
#define MAX_SIMPLE_IA   1000
#define MAX_PROPS       2000
#define MAX_TILES       10000

#define TILESIZE    16
#define CHUNKSIZE   (16 * TILESIZE)

#define MAX_ENTITYS (MAX_PLAYERS + MAX_COMPLEX_IA + MAX_SIMPLE_IA + MAX_PROPS + MAX_TILES)

extern bool debug;

#endif

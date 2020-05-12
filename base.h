#ifndef BASE_H
#define BASE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "algif.h"

typedef enum moves Moves;
typedef struct coord Coord;

enum moves {UP, RIGHT, LEFT, DOWN, ESC, EMPTY};

struct coord {
    int x;
    int y;
};

///costants & variables
//map in pixel: 702 x 504
#define CELL 18 //the map is divided by cells of 18x18px
//speeds of characters
#define SPEED 10
#define SPEED_P 9
#define SPEEDFAST 10
#define SPEEDSLOW 10
//how many pixels does the characters move in a frame
#define MOVE CELL / SPEED
#define MOVE_P CELL / SPEED_P
#define MOVEFAST CELL / SPEEDFAST
#define MOVESLOW CELL / SPEEDSLOW
//screen
#define SCREENW 504
#define SCREENH 702
//matrix
#define MAPW 28
#define MAPH 39
//
#define FPS 60
//how much food there is
int nfood = 284;
//it is used only to chage the path's color
int nmove = 0;
//lifes
int life = 3;
//
int contatore_p=0, contatore_i=0, contatore_c=0, contatore_b=0, cont=0;
//saves the prev coords of the ghosts
Coord cord_p, cord_c, cord_i, cord_b;
int count=0;
//time form when the pacman starts to move
//it is reset when pac dies
double tempo = 0;
//time from when the ghosts became blue
double tm = 0;

int matrix[MAPH][MAPW] = {{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
                          {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
                          {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
                          {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
                          {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
                          {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
                          {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
                          {2,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,2},
                          {2,1,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,1,2},
                          {2,4,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,4,2},
                          {2,1,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,1,2},
                          {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
                          {2,1,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,1,2},
                          {2,1,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,1,2},
                          {2,1,1,1,1,1,1,2,2,1,1,1,1,2,2,1,1,1,1,2,2,1,1,1,1,1,1,2},
                          {2,2,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,2,2},
                          {2,2,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,2,2},
                          {2,2,2,2,2,2,1,2,2,1,1,1,1,1,1,1,1,1,1,2,2,1,2,2,2,2,2,2},
                          {2,2,2,2,2,2,1,2,2,1,2,2,2,3,3,2,2,2,1,2,2,1,2,2,2,2,2,2},
                          {2,2,2,2,2,2,1,2,2,1,2,3,3,3,3,3,3,2,1,2,2,1,2,2,2,2,2,2},
                          {0,0,0,0,0,0,1,1,1,1,2,3,3,3,3,3,3,2,1,1,1,1,0,0,0,0,0,0},
                          {2,2,2,2,2,2,1,2,2,1,2,3,3,3,3,3,3,2,1,2,2,1,2,2,2,2,2,2},
                          {2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2},
                          {2,2,2,2,2,2,1,2,2,1,1,1,1,1,1,1,1,1,1,2,2,1,2,2,2,2,2,2},
                          {2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2},
                          {2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2},
                          {2,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,2},
                          {2,1,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,1,2},
                          {2,1,2,2,2,2,1,2,2,2,2,2,1,2,2,1,2,2,2,2,2,1,2,2,2,2,1,2},
                          {2,4,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,4,2},
                          {2,2,2,1,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,1,2,2,2},
                          {2,2,2,1,2,2,1,2,2,1,2,2,2,2,2,2,2,2,1,2,2,1,2,2,1,2,2,2},
                          {2,1,1,1,1,1,1,2,2,1,1,1,1,2,2,1,1,1,1,2,2,1,1,1,1,1,1,2},
                          {2,1,2,2,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,2,2,1,2},
                          {2,1,2,2,2,2,2,2,2,2,2,2,1,2,2,1,2,2,2,2,2,2,2,2,2,2,1,2},
                          {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
                          {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
                          {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
                          {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}}; //0 = void, 1 = route, 2 = wall, 3 = ghost's place

#endif // BASE_H

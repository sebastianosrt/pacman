#ifndef PACMAN_H
#define PACMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "astar.h"
#include "base.h"
#include "algif.h"

typedef struct pacman Pacman;
typedef struct enemy Enemy;

struct pacman {
    int x;
    int y;
    Moves dir;
    Moves prev;
};
struct enemy {
    int x;
    int y;
    int stat;
    Moves dir;
    Moves prev;
    int id;
};

int collision(Pacman pacman) {
    switch (pacman.dir) {
        case UP:
            if ((pacman.y % CELL) == 0) {
                if (matrix[(pacman.y-CELL)/CELL][(pacman.x)/CELL] != 2 && matrix[(pacman.y-CELL)/CELL][(pacman.x)/CELL] != 3)
                    return 0;
                else
                    return 1;
            }
            break;
        case RIGHT:
            if ((pacman.x % CELL) == 0) {
                if (matrix[(pacman.y)/CELL][(pacman.x+CELL)/CELL] != 2 && matrix[(pacman.y)/CELL][(pacman.x+CELL)/CELL] != 3)
                    return 0;
                else
                    return 1;
            }
            break;
        case DOWN:
            if ((pacman.y % CELL) == 0) {
                if (matrix[(pacman.y+CELL)/CELL][(pacman.x)/CELL] != 2 && matrix[(pacman.y+CELL)/CELL][(pacman.x)/CELL] != 3)
                    return 0;
                else
                    return 1;
            }
            break;
        case LEFT:
            if ((pacman.x % CELL) == 0) {
                if (matrix[(pacman.y)/CELL][(pacman.x-CELL)/CELL] != 2 && matrix[(pacman.y)/CELL][(pacman.x-CELL)/CELL] != 3)
                    return 0;
                else
                    return 1;
            }
            break;
        default:
            break;
    }
    return 0;
}

void printPacman(ALGIF_ANIMATION* pac, Pacman pacman) {
    switch(pacman.dir) {
        case UP:
            //if it is motionless
            if (collision(pacman))
                pac = algif_load_animation("media/imgs/pacman/1up.gif");
            //if has a motion
            else
                pac = algif_load_animation("media/imgs/pacman/pacup.gif");
            break;

        case RIGHT:
            //if it is motionless
            if (collision(pacman))
                pac = algif_load_animation("media/imgs/pacman/1dx.gif");
            //if has a motion
            else
                pac = algif_load_animation("media/imgs/pacman/pacdx.gif");
            break;

        case DOWN:
            //if it is motionless
            if (collision(pacman))
                pac = algif_load_animation("media/imgs/pacman/1down.gif");
            //if has a motion
            else
                pac = algif_load_animation("media/imgs/pacman/pacdown.gif");
            break;

        case LEFT:
            //if it is motionless
            if (collision(pacman))
                pac = algif_load_animation("media/imgs/pacman/1sx.gif");
            //if has a motion
            else
                pac = algif_load_animation("media/imgs/pacman/pacsx.gif");
            break;

        case ESC:
            pac = algif_load_animation("media/imgs/pacman/death.gif");
            break;

        default:
            pac = algif_load_animation("media/imgs/pacman/pac.gif");
            break;
    }
    //death animation
    if (pac != 0) {
        if (pacman.dir != ESC)
            al_draw_bitmap(algif_get_bitmap(pac, al_get_time()), pacman.x, pacman.y, 0);
        if (pacman.dir == ESC)  {
            double time = 0;
            //the death lasts 1.2 seconds
            while (time < 1.2) {
                al_draw_bitmap(algif_get_bitmap(pac, time), pacman.x, pacman.y, 0);
                al_flip_display();
                time += 0.0005;
            }
        }
    }
}
void eat(Pacman pacman, Enemy* blinky, Enemy* clyde, Enemy* inky, Enemy* pinky, int* count) {
    ALLEGRO_SAMPLE* eat = NULL;
    eat = al_load_sample("media/sounds/insert_coin.wav");
    if (tm != 0) {
        if ((al_get_time() - tm ) > 7 && (al_get_time() - tm) < 12){
            if (blinky->stat == 1)
                blinky->stat = 3;

            if (clyde->stat == 1)
                clyde->stat = 3;

            if (inky->stat == 1)
                inky->stat = 3;

            if (pinky->stat == 1)
                pinky->stat = 3;
        }

        else if ((al_get_time() - tm) > 12) {
            if (blinky->stat == 3)
                blinky->stat = 0;

            if (clyde->stat == 3)
                clyde->stat = 0;

            if (inky->stat == 3)
                inky->stat = 0;

            if (pinky->stat == 3)
                pinky->stat = 0;
        }
    }
    if(matrix[pacman.y/CELL][pacman.x/CELL] == 4){
        tm = al_get_time();
        matrix[pacman.y/CELL][pacman.x/CELL] = 0;
        if(blinky->stat != 2)
            blinky->stat = 1;
        if(clyde->stat != 2)
            clyde->stat = 1;
        if(inky->stat != 2)
            inky->stat = 1;
        if(pinky->stat != 2)
            pinky->stat = 1;
        (*count) += 20;
    }
    if (matrix[pacman.y/CELL][pacman.x/CELL] == 1) {
        matrix[pacman.y/CELL][pacman.x/CELL] = 0;
        (*count) += 10;
        nfood--;
        //when eats play a sound
        al_stop_samples();
        al_play_sample(eat, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
    }
}
//teleportation
void pacmanEffect(Pacman* pacman) {
    if ((((pacman->x % CELL) == 0) && ((pacman->y % CELL) == 0))) {
        if (pacman->x/CELL == 0 && pacman->y/CELL == 20)
            pacman->x = (MAPW-1)*CELL;
        else if (pacman->x/CELL == MAPW-1 && pacman->y/CELL == 20)
            pacman->x = 0;
    }
}
//collision with the enemies
void checkcollision_e(Pacman *pacman, Enemy *enemy, int* count){
    if(enemy->stat == 0){
        if((abs(pacman->x - enemy->x)) < 5 && (abs(pacman->y - enemy->y)) < 5)
            pacman->dir=ESC;
            ///gameover
    }
    else if(enemy->stat == 1 || enemy->stat == 3){
        if((abs(pacman->x - enemy->x)) < 5 && (abs(pacman->y - enemy->y)) < 5) {
            enemy->stat = 2;
            (*count) += 100;
        }
    }
}
//moves pacman
void move (Pacman* pacman, Moves* sal) {
    if (pacman->dir == UP) {
        if ((pacman->dir == pacman->prev)) {
            if (!collision(*pacman)){
                pacman->y -= MOVE_P;
            }
        }
        else {
            if (!collision(*pacman) && (((pacman->x % CELL) == 0) && ((pacman->y % CELL) == 0))){
                pacman->y -= MOVE_P;
                *sal=EMPTY;
            }
            else {
                pacman->dir = pacman->prev;
                if (!collision(*pacman))
                    move(pacman,sal);
            }
        }
    } else if (pacman->dir == RIGHT) {
        if ((pacman->dir == pacman->prev)) {
            if (!collision(*pacman)){
                pacman->x += MOVE_P;
            }
        }
        else {
            if (!collision(*pacman) && (((pacman->x % CELL) == 0) && ((pacman->y % CELL) == 0))){
                pacman->x += MOVE_P;
                *sal=EMPTY;
            }
            else {
                pacman->dir = pacman->prev;
                if (!collision(*pacman))
                    move(pacman,sal);
            }
        }
    } else if (pacman->dir == DOWN) {
        if ((pacman->dir == pacman->prev)) {
            if (!collision(*pacman)){
                pacman->y += MOVE_P;
            }
        }
        else {
            if (!collision(*pacman) && (((pacman->x % CELL) == 0) && ((pacman->y % CELL) == 0))){
                pacman->y += MOVE_P;
                *sal=EMPTY;
            }
            else {
                pacman->dir = pacman->prev;
                if (!collision(*pacman))
                    move(pacman,sal);
            }
        }
    } else {
        if ((pacman->dir == pacman->prev)) {
            if (!collision(*pacman)){
                pacman->x -= MOVE_P;
            }
        }
        else {
            if (!collision(*pacman) && (((pacman->x % CELL) == 0) && ((pacman->y % CELL) == 0))){
                pacman->x -= MOVE_P;
                *sal=EMPTY;
            }
            else {
                pacman->dir = pacman->prev;
                if (!collision(*pacman))
                    move(pacman,sal);
            }
        }
    }
    pacmanEffect(pacman);
}
void moveGhost(Enemy* ghost, List* path, Pacman pacman) {
    List* tmp = NULL;
    if (path != 0) {//se non è vuoto
        if (path->next == 0) {//se quello dopo è pieno
           tmp = eol(path);
            tmp->node.x *= CELL;
            tmp->node.y *= CELL;
        } else {// se quello dopo non è vuoto
            tmp = path->next;
            tmp->node.x *= CELL;
            tmp->node.y *= CELL;
        }
    }
    if (tmp != NULL) {
        //move up
        if (ghost->y > tmp->node.y) {
            //if cannot go up
            if (ghost-> x % CELL == 0) {
                if (ghost->stat == 1 || ghost->stat == 3)
                    ghost->y -= MOVESLOW;
                else if (ghost->stat == 2)
                    ghost->y -= MOVEFAST;
                else
                    ghost->y -= MOVE;
                ghost->dir = UP;
            }
            //go right or left
            else {
                //move right
                if (ghost->x < tmp->node.x) {
                    if (ghost->stat == 1 || ghost->stat == 3)
                        ghost->x += MOVESLOW;
                    else if (ghost->stat == 2)
                        ghost->x += MOVEFAST;
                    else
                        ghost->x += MOVE;
                    ghost->dir = RIGHT;
                }
                //move left
                else if (ghost->x > tmp->node.x) {
                    if (ghost->stat == 1 || ghost->stat == 3)
                        ghost->x -= MOVESLOW;
                    else if (ghost->stat == 2)
                        ghost->x -= MOVEFAST;
                    else
                        ghost->x -= MOVE;
                    ghost->dir = LEFT;
                }
            }
        }
        //move right
        else if (ghost->x < tmp->node.x) {
            if (ghost->stat == 1 || ghost->stat == 3)
                ghost->x += MOVESLOW;
            else if (ghost->stat == 2)
                ghost->x += MOVEFAST;
            else
                ghost->x += MOVE;
            ghost->dir = RIGHT;
        }
        //move down;
        else if (ghost->y < tmp->node.y) {
            //if cannot go up
            if (ghost-> x % CELL == 0) {
                if (ghost->stat == 1 || ghost->stat == 3)
                    ghost->y += MOVESLOW;
                else if (ghost->stat == 2)
                    ghost->y += MOVEFAST;
                else
                    ghost->y += MOVE;
                ghost->dir = DOWN;
            }
            //go right or left
            else {
                //move right
                if (ghost->x < tmp->node.x) {
                    if (ghost->stat == 1 || ghost->stat == 3)
                        ghost->x += MOVESLOW;
                    else if (ghost->stat == 2)
                        ghost->x += MOVEFAST;
                    else
                        ghost->x += MOVE;
                    ghost->dir = RIGHT;
                }
                //move left
                else if (ghost->x > tmp->node.x) {
                    if (ghost->stat == 1 || ghost->stat == 3)
                        ghost->x -= MOVESLOW;
                    else if (ghost->stat == 2)
                        ghost->x -= MOVEFAST;
                    else
                        ghost->x -= MOVE;
                    ghost->dir = LEFT;
                }
            }
        }
        //move left
        else if (ghost->x > tmp->node.x) {
            if (ghost->stat == 1 || ghost->stat == 3)
                ghost->x -= MOVESLOW;
            else if (ghost->stat == 2)
                ghost->x -= MOVEFAST;
            else
                ghost->x -= MOVE;
            ghost->dir = LEFT;
        }
    }/*
    if(pacman.x+CELL == ghost->x && pacman.y == ghost->y)
            ghost->x -= CELL;
    if(pacman.y+CELL == ghost->y && pacman.x == ghost->x)
            ghost->y -= CELL;*/
    freeList(path);
}

void printEnemy(ALGIF_ANIMATION* bl, Enemy enemy) {
    char path[50] = "media/imgs/";
    if (enemy.id == 0)
        strcat(path, "blinky/blinky");
    else if (enemy.id == 1)
        strcat(path, "inky/inky");
    else if (enemy.id == 2)
        strcat(path, "pinky/pinky");
    else
        strcat(path, "clyde/clyde");

    switch(enemy.dir) {
        case UP:
            strcat(path, "up.gif");
            if(enemy.stat==0)
                bl = algif_load_animation(path);
            if(enemy.stat==2)
                bl = algif_load_animation("media/imgs/eyes/mup.gif");
            break;

        case RIGHT:
            strcat(path, "dx.gif");
            if(enemy.stat==0)
                bl = algif_load_animation(path);
            else if(enemy.stat==2)
                bl = algif_load_animation("media/imgs/eyes/mdx.gif");
            break;

        case DOWN:
            strcat(path, "down.gif");
            if(enemy.stat==0)
                bl = algif_load_animation(path);
            if(enemy.stat==2)
                bl = algif_load_animation("media/imgs/eyes/mdown.gif");
            break;

        case LEFT:
            strcat(path, "sx.gif");
            if(enemy.stat==0)
                bl = algif_load_animation(path);
            if(enemy.stat==2)
                bl = algif_load_animation("media/imgs/eyes/msx.gif");
            break;

        default:
            break;
    }
    if(enemy.stat == 1)
        bl = algif_load_animation("media/imgs/ghost_blue/blue.gif");
    if (enemy.stat == 3)
        bl = algif_load_animation("media/imgs/ghost_blue/blue_white.gif");
    al_draw_bitmap(algif_get_bitmap(bl, al_get_time()), enemy.x, enemy.y, 0);
}

#endif // PACMAN_H

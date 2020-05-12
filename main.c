//librerie
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
#include "pacman.h"
#include "base.h"
#include "algif.h"
#include "astar.h"

void setUp(ALLEGRO_KEYBOARD_STATE state, ALLEGRO_DISPLAY* display);
void printFood(ALLEGRO_BITMAP* ball, ALGIF_ANIMATION* power);
Moves takeDirection(ALLEGRO_KEYBOARD_STATE state);
Coord randPos();
void refresh(ALLEGRO_BITMAP* map, ALLEGRO_BITMAP* ball, ALGIF_ANIMATION* power, ALLEGRO_FONT* font);
void win();

///main
int main() {
    srand(time(0));
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_KEYBOARD_STATE state;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP* map;
    //int i;
///characters
    //pacman
    Pacman pacman;
    pacman.dir = EMPTY;
    pacman.prev = RIGHT;
    ALGIF_ANIMATION* pac;
    //ghost red
    Enemy blinky;
    ALGIF_ANIMATION* bl;
    //ghost orange
    Enemy clyde;
    ALGIF_ANIMATION* cl;
    //ghost lightblue
    Enemy inky;
    ALGIF_ANIMATION* in;
    //ghost pink
    Enemy pinky;
    ALGIF_ANIMATION* pi;
    //food
    ALLEGRO_BITMAP* ball;
    ALGIF_ANIMATION* power;
    //sound
    ALLEGRO_SAMPLE* initial_sound = NULL;
    ALLEGRO_SAMPLE* siren_slow = NULL;
    ALLEGRO_SAMPLE* blue = NULL;
    //corners
    Coord top_dx_corner;
    Coord top_sx_corner;
    Coord bottom_dx_corner;
    Coord bottom_sx_corner;
    Coord center;
    //ghost's path to pacman
    List* pathbl = NULL;
    List* pathin = NULL;
    List* pathpi = NULL;
    List* pathcl = NULL;
    //things

    Coord start_c, start_i, start_p, start_b,target;
    Moves sal=EMPTY,s=EMPTY;

///start program
    setUp(state, display);
///init
    //map
    map = al_load_bitmap("media/imgs/map/map.png");
    //font
    font = al_load_font("media/fonts/Cheapmot.ttf", 30 , 0);
    //food
    ball = al_load_bitmap("media/imgs/food/ball.gif");
    power = algif_load_animation("media/imgs/food/power.gif");
    //sound
    initial_sound = al_load_sample("media/sounds/intro.wav");
    siren_slow = al_load_sample("media/sounds/siren_slow.wav");
    blue = al_load_sample("media/sounds/blue.wav");
    //pac
    pac = algif_load_animation("media/imgs/pacman/pacdx.gif");
    //blinky
    bl = algif_load_animation("media/imgs/blinky/blinkyup.gif");
    //inky
    in = algif_load_animation("media/imgs/inky/inkyup.gif");
    //pinky
    pi = algif_load_animation("media/imgs/pinky/pinkyup.gif");
    //clyde
    cl = algif_load_animation("media/imgs/clyde/clydeup.gif");
    //timer set
    timer = al_create_timer(1.0 / FPS);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    al_reserve_samples(1);
    ///game start
    while (life > 0 && nfood > 0) {
        pacman.x = 14*CELL;
        pacman.y = 29*CELL;
        pacman.dir = EMPTY;
        pacman.prev = EMPTY;

        blinky.x = 14*CELL;
        blinky.y = 17*CELL;
        blinky.stat = 0;
        blinky.id = 0;

        inky.x = 11*CELL;
        inky.y = 21*CELL;
        inky.stat = 0;
        inky.id = 1;

        pinky.x = 14*CELL;
        pinky.y = 21*CELL;
        pinky.stat = 0;
        pinky.id = 2;

        clyde.x = 16*CELL;
        clyde.y = 21*CELL;
        clyde.stat = 0;
        clyde.id = 3;
        //corners
        top_dx_corner.x = (MAPW-2)*CELL;
        top_dx_corner.y = 7*CELL;
        top_sx_corner.x = 1*CELL;
        top_sx_corner.y = 7*CELL;
        bottom_dx_corner.x = (MAPW-2)*CELL;
        bottom_dx_corner.y = (MAPH-6)*CELL;
        bottom_sx_corner.x = 2;
        bottom_sx_corner.y = bottom_dx_corner.y;
        center.x = 14*CELL;
        center.y = 20*CELL;
    ///game loop
        al_stop_samples();
        al_play_sample(initial_sound, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
        while (pacman.dir != ESC && nfood != 0 && life > 0) {
            //graphics
            refresh(map, ball, power, font);
            printEnemy(bl, blinky);
            printEnemy(in, inky);
            printEnemy(pi, pinky);
            printEnemy(cl, clyde);
            printPacman(pac, pacman);
            //take the initial direction
            s=EMPTY;
            s=takeDirection(state);
            if(s != EMPTY && s != UP && s != DOWN && al_get_time()-tempo < 4.5)
                sal=s;
            if(s != EMPTY && al_get_time()-tempo > 4.5)
                sal = s;
    ///game starts when the initial_sound finishes
            if (al_get_time()-tempo > 4.5) {
                if ((blinky.stat == 1 || inky.stat == 1 || pinky.stat == 1 || clyde.stat == 1) || (blinky.stat == 3 || inky.stat == 3 || pinky.stat == 3 || clyde.stat == 3))
                    al_play_sample(blue, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
                else
                    al_play_sample(siren_slow, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);





                ///blinky
                nmove = 0;
                target.x = blinky.x;
                target.y = blinky.y;

                //when turns into eyes moves it till it is perfectly in a cell
                /*if(blinky.stat==2 && contatore_b == 1){
                    blinky.x+=(1-blinky.x%CELL);
                    blinky.y+=(1-blinky.y%CELL);
                    contatore_b++;
                }*/

                //when it is perfectly in a cell it can change direction
                if(blinky.x % CELL == 0 && blinky.y % CELL == 0){
                    //if is blue
                    if (blinky.stat == 1 || blinky.stat == 3) {
                        if (fmod(al_get_time()-tempo, 0.6) >= 0 && fmod(al_get_time()-tempo, 1.3) < 0.05)
                            start_b = randPos();
                        contatore_b = 0;
                    }
                    //if is eyes
                    else if (blinky.stat == 2) {
                        if (target.x == center.x && target.y == center.y)
                            blinky.stat = 0;
                        start_b = center;
                        contatore_b++;
                    }
                    //if is normal
                    else {
                        contatore_b = 0;
                        if (al_get_time()-tempo < 8)
                            start_b = top_dx_corner;
                        else {
                            start_b.x = pacman.x;
                            start_b.y = pacman.y;
                        }
                    }
                }
                //if it has notmoved it haves to move
                if(blinky.x == cord_b.x && blinky.y == cord_b.y && blinky.stat != 2 )
                    start_b = randPos();

                //saves the actual coords
                cord_b.x = blinky.x;
                cord_b.y = blinky.y;
                //get the path
                pathbl = astar(start_b, target);
                //move blinky
                moveGhost(&blinky, pathbl, pacman);
                checkcollision_e(&pacman, &blinky, &count);




                ///inky
                nmove = 1;
                target.x = inky.x;
                target.y = inky.y;
                /*if((inky.x % CELL != 0 || inky.y % CELL != 0) && inky.stat == 2 && contatore_i == 1){
                    if(inky.x % CELL != 0 ){
                        inky.x+=(1-inky.x%CELL);
                    }

                    if(inky.y % CELL != 0){
                        inky.y+=(1-inky.y%CELL);
                    }
                    contatore_i++;
                }*/
                if(inky.x % CELL == 0 && inky.y % CELL == 0){
                    if (inky.stat == 1 || inky.stat == 3) {
                        if (fmod(al_get_time()-tempo, 0.7) >= 0 && fmod(al_get_time()-tempo, 1.3) < 0.05)
                            start_i = randPos();
                        contatore_i = 0;
                    } else if (inky.stat == 2) {
                        if (target.x == center.x && target.y == center.y)
                            inky.stat = 0;
                        start_i = center;
                        contatore_i++;
                    }

                    else {
                        contatore_i = 0;
                        if (al_get_time()-tempo < 8)
                            start_i = bottom_sx_corner;
                        //if the top or bottom cell isn't a wall
                        else if (matrix[clyde.y-CELL/CELL][clyde.x/CELL] != 2 || matrix[clyde.y+CELL/CELL][clyde.x/CELL] != 2){
                            //if the right or left cell isn't a wall
                            if (matrix[clyde.y/CELL][clyde.x-CELL/CELL] != 2 || matrix[clyde.y/CELL][clyde.x+CELL/CELL] != 2){
                                //if the distance between pacman and inky is > 5
                                if(sqrt(pow(((inky.x-pacman.x)/CELL), 2) + pow(((inky.y-pacman.y)/CELL), 2) > 5)){
                                    //if blinky is at the top of pacman
                                    if (blinky.y < pacman.y) {
                                        //get the y of the end path of inky
                                        if (pacman.y/CELL < MAPH-7)
                                            start_i.y = pacman.y + 4*CELL;
                                        else {
                                            start_i.y = pacman.y;
                                            while (start_i.y/CELL < MAPH-3)
                                                start_i.y += CELL;
                                        }
                                        //get the x of the end path of inky
                                        if (start_i.x < pacman.x) {
                                            if (pacman.x/CELL < MAPW-5)
                                                start_i.x = pacman.x + 4*CELL;
                                            else {
                                                start_i.x = pacman.x;
                                                while (start_i.x/CELL < MAPW-1)
                                                    start_i.x += CELL;
                                            }
                                            while (matrix[start_i.y/CELL][start_i.x/CELL] == 2 || matrix[start_i.y/CELL][start_i.x/CELL] == 3) {
                                                start_i.y -= CELL;
                                                start_i.x -= CELL;
                                            }
                                        } else {
                                            if (pacman.x/CELL > 5)
                                                start_i.x = pacman.x - 4*CELL;
                                            else {
                                                start_i.x = pacman.x;
                                                while (start_i.x/CELL > 1)
                                                    start_i.x -= CELL;
                                            }
                                            while (matrix[start_i.y/CELL][start_i.x/CELL] == 2 || matrix[start_i.y/CELL][start_i.x/CELL] == 3) {
                                                start_i.y -= CELL;
                                                start_i.x += CELL;
                                            }
                                        }
                                    //if blinky is at the bottom of pacman
                                    } else {
                                        if (pacman.y/CELL > 7)
                                            start_i.y = pacman.y - 4*CELL;
                                        else {
                                            start_i.y = pacman.y;
                                            while (start_i.y/CELL > 7)
                                                start_i.y -= CELL;
                                        }
                                        if (blinky.x < pacman.x) {
                                            if (pacman.x/CELL < MAPW-5)
                                                start_i.x = pacman.x + 4*CELL;
                                            else {
                                                start_i.x = pacman.x;
                                                while (start_i.x/CELL < MAPW-1)
                                                    start_i.x += CELL;
                                            }
                                            while (matrix[start_i.y/CELL][start_i.x/CELL] == 2 || matrix[start_i.y/CELL][start_i.x/CELL] == 3) {
                                                start_i.y += CELL;
                                                start_i.x -= CELL;
                                            }
                                        } else {
                                            if (pacman.x/CELL > 5)
                                                start_i.x = pacman.x - 4*CELL;
                                            else {
                                                start_i.x = pacman.x;
                                                while (start_i.x/CELL > 1)
                                                    start_i.x -= CELL;
                                            }
                                            while (matrix[start_i.y/CELL][start_i.x/CELL] == 2 || matrix[start_i.y/CELL][start_i.x/CELL] == 3) {
                                                start_i.y += CELL;
                                                start_i.x += CELL;
                                            }
                                        }
                                    }
                                }else{
                                    start_i.x = pacman.x;
                                    start_i.y = pacman.y;
                                }
                            }
                        }
                    }
                }
                if(inky.x == cord_i.x && inky.y == cord_i.y ){
                    if(inky.stat != 2)
                        start_i = randPos();
                    if(inky.stat == 2)
                        start_i = center;
                }
                cord_i.x = inky.x;
                cord_i.y = inky.y;
                pathin = astar(start_i, target);

                //move inky
                moveGhost(&inky, pathin, pacman);
                checkcollision_e(&pacman, &inky, &count);





                ///pinky
                nmove = 2;
                target.x = pinky.x;
                target.y = pinky.y;

                /*if((pinky.x % CELL != 0 || pinky.y % CELL != 0) && pinky.stat==2 && contatore_i == 1){
                    if(pinky.x % CELL != 0 ){
                        pinky.x+=(1-pinky.x%CELL);
                    }

                    if(pinky.y % CELL != 0){
                        pinky.y+=(1-pinky.y%CELL);
                    }
                    contatore_p++;
                }*/

                if(pinky.x % CELL == 0 && pinky.y % CELL == 0){
                    if (pinky.stat == 1 || pinky.stat == 3) {
                        if (fmod(al_get_time()-tempo, 0.8) >= 0 && fmod(al_get_time()-tempo, 1.3) < 0.05)
                            start_p = randPos();
                        contatore_p=0;
                    }else if (pinky.stat == 2) {
                        if (target.x == center.x && target.y == center.y)
                            pinky.stat = 0;
                        start_p = center;
                        contatore_p++;
                    }

                    else {
                        contatore_p = 0;
                        if (al_get_time()-tempo < 8){
                            start_p = top_sx_corner;
                        }
                        else if(sqrt( ((pinky.x-pacman.x)/CELL) * ((pinky.x-pacman.x)/CELL) + ((pinky.y-pacman.y)/CELL) * ((pinky.y-pacman.y)/CELL)) < 3){
                            start_p.x = pacman.x;
                            start_p.y = pacman.y;
                        }
                        else if (pacman.dir == RIGHT) {
                            if (pacman.x/CELL < MAPW-3)
                                start_p.x = pacman.x+CELL*4;
                            else
                                start_p.x = pacman.x;
                            start_p.y = pacman.y;
                            while (matrix[start_p.y/CELL][start_p.x/CELL] == 2 || matrix[start_p.y/CELL][start_p.x/CELL] == 3)
                                start_p.x -= CELL;
                        } else if (pacman.dir == UP) {
                            if (pacman.y/CELL > 7)
                                start_p.y = pacman.y-CELL*4;
                            else
                                start_p.y = pacman.y;
                            start_p.x = pacman.x;
                            while (matrix[start_p.y/CELL][start_p.x/CELL] == 2 || matrix[start_p.y/CELL][start_p.x/CELL] == 3)
                                start_p.y += CELL;
                        } else if (pacman.dir == LEFT) {
                            if (pacman.x/CELL > 4)
                                start_p.x = pacman.x-CELL*4;
                            else
                                start_p.x = pacman.x;
                            start_p.y = pacman.y;
                            while (matrix[start_p.y/CELL][start_p.x/CELL] == 2 || matrix[start_p.y/CELL][start_p.x/CELL] == 3)
                                start_p.x -= CELL;
                        } else {
                            if (pacman.y/CELL < MAPH-6)
                                start_p.y = pacman.y+CELL*4;
                            while (matrix[start_p.y/CELL][start_p.x/CELL] == 2 || matrix[start_p.y/CELL][start_p.x/CELL] == 3)
                                start_p.y -= CELL;
                        }
                    }

                }
                if(pinky.x == cord_p.x && pinky.y == cord_p.y )
                    start_p = randPos();

                cord_p.x = pinky.x;
                cord_p.y = pinky.y;
                pathpi = astar(start_p, target);
                 //move pinky
                moveGhost(&pinky, pathpi, pacman);
                checkcollision_e(&pacman, &pinky, &count);







                ///clyde
                nmove = 3;
                target.x = clyde.x;
                target.y = clyde.y;

                /*if((clyde.x % CELL != 0 || clyde.y % CELL != 0) && clyde.stat==2 && contatore_i == 1){
                    if(clyde.x % CELL != 0 )
                        clyde.x+=(1-clyde.x%CELL);
                    if(clyde.y % CELL != 0)
                        clyde.y+=(1-clyde.y%CELL);
                    contatore_c++;
                }*/
                cont=0;
                if(clyde.x % CELL == 0 && clyde.y % CELL == 0){
                    if (clyde.stat == 1 || clyde.stat == 3) {
                        if (fmod(al_get_time()-tempo, 0.9) >= 0 && fmod(al_get_time()-tempo, 1.3) < 0.05)
                            start_c = randPos();
                        contatore_c = 0;
                    } else if (clyde.stat == 2) {
                        if (target.x == center.x && target.y == center.y)
                            clyde.stat = 0;
                        start_c = center;
                        contatore_c++;
                    }
                    else {
                        contatore_c = 0;
                        //start = bottom_dx_corner;
                        if (al_get_time()-tempo < 8){
                                    start_c = bottom_dx_corner;
                        }
                        //if the top or bottom cell isn't a wall
                        if (matrix[clyde.y-CELL/CELL][clyde.x/CELL] != 2 || matrix[clyde.y+CELL/CELL][clyde.x/CELL] != 2){
                            //if the left or right cell isn't a wall
                            if (matrix[clyde.y/CELL][clyde.x-CELL/CELL] != 2 || matrix[clyde.y/CELL][clyde.x+CELL/CELL] != 2){
                               if(al_get_time()-tempo > 8){
                                   //if clyde is >= 6 cells far to pacman
                                    if (sqrt(pow(((clyde.x-pacman.x)/CELL), 2) + pow(((clyde.y-pacman.y)/CELL), 2) >= 6)){
                                        do{
                                            if (fmod(al_get_time()-tempo, 10) >= 0 && fmod(al_get_time()-tempo, 10) < 0.1)
                                                start_c = randPos();
                                            cont++;
                                        }while(sqrt(sqrt(pow(((clyde.x-pacman.x)/CELL), 2) + pow(((clyde.y-pacman.y)/CELL), 2)) < 9 && cont < 15));
                                    }else{
                                        start_c.x = pacman.x;
                                        start_c.y = pacman.y;
                                    }
                                }
                            }
                        }
                    }
                }
                if(clyde.x == cord_c.x && clyde.y == cord_c.y){
                    //if (fmod(al_get_time()-tempo, 0.6) >= 0 && fmod(al_get_time()-tempo, 1.3) < 0.05)
                    start_c = randPos();
                }
                cord_c.x = clyde.x;
                cord_c.y = clyde.y;
                pathcl = astar(start_c, target);
                //move clyde
                moveGhost(&clyde, pathcl, pacman);
                checkcollision_e(&pacman, &clyde, &count);

                //to visualize the ghost's path
                al_flip_display();
            ///pacman
                pacman.prev = pacman.dir;
                if(pacman.x%CELL==0 && pacman.y%CELL==0){
                    if(sal!=EMPTY)
                        pacman.dir = sal;
                    else {
                        //if is the first move it cannot be up or down
                        if (pacman.dir == EMPTY)  {
                            while (pacman.dir == EMPTY || pacman.dir == UP || pacman.dir == DOWN)
                                pacman.dir = takeDirection(state);
                        } else
                            pacman.dir = takeDirection(state);
                    }
                }
                if (pacman.dir == EMPTY)
                    pacman.dir = pacman.prev;
                if (pacman.dir != ESC && pacman.dir != EMPTY) {
                //move pacman
                    move(&pacman,&sal);
                    eat(pacman, &blinky, &inky, &clyde, &pinky, &count);
                }
            }
            al_flip_display();
            al_wait_for_event(event_queue, &event);
        }
        ///end
        if (pacman.dir == ESC || life == 0) {
            al_stop_samples();
            ALLEGRO_SAMPLE* death = al_load_sample("media/sounds/death.wav");
            al_play_sample(death, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
            refresh(map, ball, power, font);
            printPacman(pac, pacman);
            if (life > 0)
                life--;
            if (life == 0) {
                al_draw_textf(font,al_map_rgb(255, 0, 0), SCREENW/2, SCREENH/2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
                al_flip_display();
                al_rest(1.3);
            }
            tempo = al_get_time();
        }
        else
            win();
    }
    ///end game loop
    al_destroy_display(display);
    algif_destroy_animation(pac);
    algif_destroy_animation(bl);
    algif_destroy_animation(in);
    algif_destroy_animation(cl);
    algif_destroy_animation(pi);
    algif_destroy_animation(power);
    al_destroy_bitmap(ball);
    al_destroy_sample(initial_sound);


    return 0;
}

//the set up to let the program work properly
void setUp(ALLEGRO_KEYBOARD_STATE state, ALLEGRO_DISPLAY* display) {
//allegro init
    if (!al_init())
        printf("Allegro not initialized.\n");
//display init
    display = al_create_display(SCREENW, SCREENH);
    if (!display)
        printf("Display not initialized.\n");
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
//keyboard init
    al_install_keyboard();
    if (!al_is_keyboard_installed()) {
        printf("Keyboard not installed\n");
    }
    al_get_keyboard_state(&state);
//addons
    al_init_font_addon();
    al_init_ttf_addon();
    if (!al_init_image_addon())
        printf("Image addon not initialized.\n");
    if (!al_init_primitives_addon())
        printf("Primitives addon not initialized.\n");
//musica
    if (!al_install_audio())
        printf("Audio addon not initialized.\n");
    if (!al_init_acodec_addon())
        printf("Acodec addon not initialized.\n");
    srand(time(0));
}

void printFood(ALLEGRO_BITMAP* ball, ALGIF_ANIMATION* power) {
    int i, j;
    for (i = 0; i < MAPH; i++) {
        for (j = 0; j < MAPW; j++) {
            if (matrix[i][j] == 1)
                al_draw_bitmap(ball, j*CELL, i*CELL, 0);
            if (matrix[i][j] == 4)
                al_draw_bitmap(algif_get_bitmap(power, al_get_time()-tempo), j*CELL, i*CELL, 0);
        }
    }
}

//check which button is pressed and returns the move to do
Moves takeDirection(ALLEGRO_KEYBOARD_STATE state) {
    al_get_keyboard_state(&state);
    if (al_key_down(&state, ALLEGRO_KEY_UP))
        return UP;
    if (al_key_down(&state, ALLEGRO_KEY_RIGHT))
        return RIGHT;
    if (al_key_down(&state, ALLEGRO_KEY_DOWN))
        return DOWN;
    if (al_key_down(&state, ALLEGRO_KEY_LEFT))
        return LEFT;
    if (al_key_down(&state, ALLEGRO_KEY_ESCAPE)) {
        //exit the game
        life = 0;
        return ESC;
    }
    return EMPTY;
}
//get a random coordinate of the maze
Coord randPos() {
    Coord pos;
    do {
        pos.x = (rand() % MAPW-1)+1;
        pos.y = (rand() % MAPH-3-7)+7;
        if (pos.y < 0)
            pos.y *= -1;
    } while (matrix[pos.y][pos.x] == 2 || matrix[pos.y][pos.x] == 3);
    pos.x *= CELL;
    pos.y *= CELL;
    return pos;
}

void refresh(ALLEGRO_BITMAP* map, ALLEGRO_BITMAP* ball, ALGIF_ANIMATION* power, ALLEGRO_FONT* font) {
    ALLEGRO_BITMAP* lifes = al_load_bitmap("media/imgs/pacman/1sx.gif");
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(map, 0, 0, 0);
    al_draw_textf(font,al_map_rgb(255, 0, 0), SCREENW-120, 2, ALLEGRO_ALIGN_CENTRE, "SCORE: %d", count);
    for (int i = 0; i < life; i++)
        al_draw_bitmap(lifes, 0+i*20, 0, 0);
    printFood(ball, power);
}

void win() {
    ALLEGRO_FONT* win = al_load_font("media/fonts/Cheapmot.ttf", 20, 0);
    ALLEGRO_BITMAP* map_white = al_load_bitmap("media/imgs/map/map_white.png");
    ALLEGRO_BITMAP* map = al_load_bitmap("media/imgs/map/map.png");
    int i = 0;
    al_rest(0.8);
    while (i < 4) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        if (i % 2 == 0 || i == 0)
            al_draw_bitmap(map, 0, 0, 0);
        else
            al_draw_bitmap(map_white, 0, 0, 0);
        al_draw_textf(win, al_map_rgb(255, 0, 0), SCREENW/2, SCREENH/2, ALLEGRO_ALIGN_CENTRE, "WIN");
        al_flip_display();
        al_rest(0.5);
        i++;
    }
}

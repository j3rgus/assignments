#ifndef _MAZE_
#define _MAZE_

#ifndef TILE_H
#define TILE_H  10
#endif

#ifndef TILE_W
#define TILE_W  10
#endif

#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define MAZE_MAX_LENGTH 100

typedef struct {
    int x;
    int y;
} COORDINATE;

typedef struct {
    char data[MAZE_MAX_LENGTH][MAZE_MAX_LENGTH];
    unsigned int w;
    unsigned int h;
} MAZE;

void print_block(COORDINATE points, ALLEGRO_COLOR color);
int init(void);
ALLEGRO_BITMAP *prepare_bitmap(unsigned int w, unsigned int h);
ALLEGRO_COLOR get_color(char *str_color);
void draw_maze(MAZE *maze, ALLEGRO_COLOR color);
int get_ends(MAZE *maze, COORDINATE *start, COORDINATE *end);
int find_path(MAZE *maze, COORDINATE start, COORDINATE goal);
void correct_maze(MAZE *maze);

#endif

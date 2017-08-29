#include "maze.h"

void print_block(COORDINATE points, ALLEGRO_COLOR color)
{
    for (int i = points.y; i < (points.y+TILE_H); i++)
        for (int j = points.x; j < (points.x+TILE_W); j++)
            al_put_pixel(j, i, color);
}

int init(void)
{
    if (!al_init()) {
        fprintf(stderr, "Failed to init allegro\n");
        return -1;
    }
    if (!al_init_image_addon()) {
        fprintf(stderr, "Failed to init allegro_image_addon\n");
        return -1;
    }
    return 0;
}

ALLEGRO_BITMAP *prepare_bitmap(unsigned int w, unsigned int h)
{
    ALLEGRO_BITMAP *bit_map;

    bit_map = al_create_bitmap(w, h);
    al_set_target_bitmap(bit_map);
    al_clear_to_color(al_map_rgb(0,0,255));
    return bit_map;
}

ALLEGRO_COLOR get_color(char *str_color)
{
    char *col;
    unsigned int r, g, b;

    col = (char *) malloc(9);
    if (col == NULL)
        exit(1);

    if ((str_color[0] == '#') && (strlen(str_color) == 7)) {
        strncpy(&col[0], &str_color[1], 2);
        strncpy(&col[3], &str_color[3], 2);
        strncpy(&col[6], &str_color[5], 2);
        col[2] = col[5] = col[8] = ' ';
        sscanf(col, "%X %X %X", &r, &g, &b);
    }
    else {
        r = g = b = 0;
    }
    free(col);
    return al_map_rgb(r, g, b);
}

void draw_maze(MAZE *maze, ALLEGRO_COLOR color)
{    
    for (unsigned int i = 0; i < maze->h; i++) {
        for (unsigned int j = 0; j < maze->w; j++) {
            if (maze->data[i][j] == ' ')
                print_block((COORDINATE){j*TILE_W, i*TILE_H},  al_map_rgb(255,255,255));
            if (maze->data[i][j] == '#') {
                print_block((COORDINATE){j*TILE_W, i*TILE_H},  color);
            }
            if (maze->data[i][j] == 'c')
                print_block((COORDINATE){j*TILE_W, i*TILE_H},  al_map_rgb(255,0,0));
        }
    }
}

int get_ends(MAZE *maze, COORDINATE *start, COORDINATE *goal)
{
    int set = 0;

    for (unsigned int i = 0; i < maze->h; i++) {
        for (unsigned int j = 0; j < maze->w; j++) {
            if (!i || !j || (i == maze->h-1) || (j == maze->w-1)) {
                if (maze->data[i][j] == ' ') {
                    if (!set) {start->x = j; start->y = i;}
                    else {goal->x = j; goal->y = i;}
                    set <<= 1; set |= 1;
                }
            }
        }
    }
    return !(set ^ 3);
}

int find_path(MAZE *maze, COORDINATE start, COORDINATE goal)
{
    if ((start.x >= (signed)maze->w) || (start.x < 0) || (start.y >= (signed)maze->h) || (start.y < 0)) return 0;
    if (maze->data[start.y][start.x] == '#') return 0;
    if (maze->data[start.y][start.x] == 'c') return 0;
    maze->data[start.y][start.x] = 'c';
    if ((start.x == goal.x) && (start.y == goal.y)) return 1;
    if (find_path(maze, (COORDINATE) {start.x+1, start.y}, goal) == 1) return 1;
    if (find_path(maze, (COORDINATE) {start.x, start.y-1}, goal) == 1) return 1;
    if (find_path(maze, (COORDINATE) {start.x-1, start.y}, goal) == 1) return 1;
    if (find_path(maze, (COORDINATE) {start.x, start.y+1}, goal) == 1) return 1;
    maze->data[start.y][start.x] = ' ';
    return 0;
}

void correct_maze(MAZE *maze)
{
    for (unsigned int i = 0; i < maze->h; i++) {
        memset(&maze->data[i][strlen(maze->data[i])], ' ', maze->w - strlen(maze->data[i]));
        maze->data[i][maze->w] = 0;
    }
}

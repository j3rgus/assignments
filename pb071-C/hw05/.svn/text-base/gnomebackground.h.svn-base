#ifndef G_BG_
#define G_BG_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FORMAT_1    ".jpg"
#define FORMAT_2    ".png"

typedef struct {
    char *file_name;
    char *full_path;
    time_t modified;
} IMAGE;

IMAGE *imgs;

int com_time(const void *a, const void *b);
int com_lex(const void *a, const void *b);
int format_compare(char *str);
void add_img(char *path, char *name, time_t modify, int num);
void free_all(int num);
int search_pics(char *path);
void print_file(char *name, int num, int dur, int (*p_mode)());
int test_dir(char *path);

#endif

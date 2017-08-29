#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "errorHandling.h"
#include "picture.h"

unsigned int center_pic(unsigned int large, unsigned int height)
{
    return ((large - height) / 2) + ((large - height) % 2);
}

void print_row_picture(unsigned char *pic, unsigned int row, unsigned int length)
{
    for (unsigned int i = 0; i < length; i++) {
        if (pic != NULL)
            printf("%c", pic[row*length + i]);
        else printf(" ");
    }
}

void print_one_picture(unsigned char *pic, unsigned int height, unsigned int length)
{
    for (unsigned int i = 0; i < height; i++) {
        print_row_picture(pic, i, length);
        printf("\n");
    }
}

// [i*act_pic->width + j]
unsigned int get_largest_size_picture(unsigned int a, unsigned int b, unsigned int c)
{
    unsigned int max;

    if (a>b) {
        if (a>c) max=a;
        else max=c;
    } else {
        if (b>c) max=b;
        else max=c;
    }
    return max;
}

void delete_picture(unsigned char *pic)
{
    free((void *) pic);
}

unsigned char *load_picture(unsigned int *width, unsigned int *height)
{
    unsigned char *pic_mem = NULL, *more_mem;;
    unsigned char in, err, temp;
    unsigned int count, len = 0;

    *height = 0;
    err = 0;
    count = 0;
    while ((temp = getchar()) != '\n') {
        ungetc(temp, stdin);
        while (1) {
            in = getchar();
            if (err) { if (in != '\n') continue; else break; }
            more_mem = (unsigned char *) realloc(pic_mem, count+1);
            if (more_mem != NULL) {
                pic_mem = more_mem;
                pic_mem[count] = in;
            } else {
                errorHandle(ERROR_NO_MEMORY);
                free((void *) pic_mem);
                err = 1;
            }
            if (in == '\n') {
                if (*height == 0) *width = len;
                if (*width != len) {
                    errorHandle(ERROR_WRONG_FORMAT);
                    free((void *)more_mem);
                    err = 2;
                }
                (*height)++;
                len = 0;
                break;
            }
            count++;
            len++;
        }
    }

    if (err || !count) return NULL;
    pic_mem[count] = 0;
    return pic_mem;
}

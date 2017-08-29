#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "errorHandling.h"
#include "container.h"
#include "picture.h"
#include "gallery.h"

void make_gallery(void)
{
    init_list();
}

int insert_picture(int in)
{
    unsigned int width, height;
    unsigned char *pic;

    if (!in) {
        pic = load_picture(&width, &height);
        if (pic == NULL) return -1;
    } else {
        if (box.picture == NULL)
            return -1;
        pic = (unsigned char *) malloc(box.height * box.width);
        if (pic == NULL) {
            errorHandle(ERROR_NO_MEMORY);
            return -1;
        }
        memcpy(pic, box.picture, box.height * box.width);
        height = box.height;
        width = box.width;
    }
    return insert_list(pic, width, height);
}

void delete_actual_picture(void)
{
    ELEMENT *actual_pic;

    if (get_size_elements() == 0) return;
    actual_pic = get_actual_element();
    delete_picture(actual_pic->picture);
    delete_list();
}

void rotate_pictures(char c)
{
    if (c == 'l')
        shift_right_list();
    if (c == 'r')
        shift_left_list();
}

void print_size_of_gallery(void)
{
    printf("size: %u\n", get_size_elements());
}

void delete_gallery(void)
{
    if (box.picture != NULL) {
        delete_picture(box.picture);
    }
    while (get_size_elements())
        delete_actual_picture();
}

void print_gallery(void)                // TAK TOTO JE BORDEL, MAL SOM SI TEN OBRAZOK LEPSIE UKLADAT
{                                       // ANI TO NELUSTI, myslienka je ze najdem nizsi stred vo vzdtahu s i, cize podielu
    ELEMENT *act_pic;                   // najvacsi_obrazok - aktualna_velkost_obrazku / 2 pricitam este ich zvysok po deleni
    unsigned int large_pic;             // hlavny cyklus je robeny pre najv. obr. (i) a tytmo viem kedy zapisovat vzhladom na velkosti i
                                        // podm. potom zistuje ten "stred" (&& velkost druheho obrazku) - pokial by si mi to chcel kvoli tomuto vratit
    if (!get_size_elements()) return;   // tak nemusis, nic ineho ma nenapada a musel by som asi upravit sposob zapisu obrazku co sa mi fakt nechce uz menit
    act_pic = get_actual_element();

    if (get_size_elements() == 1) {
        print_one_picture(act_pic->picture, act_pic->height, act_pic->width);
        return;
    }
    large_pic = get_largest_size_picture(act_pic->prev->height, act_pic->height, act_pic->next->height);
    if (get_size_elements() >= 2) {
        for (unsigned i = 0; i < large_pic; i++) {
            if (get_size_elements() > 2) {
                if ((center_pic(large_pic, act_pic->prev->height) <= i) && ((i - center_pic(large_pic, act_pic->prev->height)) < act_pic->prev->height))
                    print_row_picture(act_pic->prev->picture, i-center_pic(large_pic, act_pic->prev->height), act_pic->prev->width);
                else print_row_picture(NULL, 0, act_pic->prev->width);
                printf(" | ");
            }
            if ((center_pic(large_pic, act_pic->height) <= i) && ((i - center_pic(large_pic, act_pic->height)) < act_pic->height))
                print_row_picture(act_pic->picture, i-center_pic(large_pic, act_pic->height), act_pic->width);
            else print_row_picture(NULL, 0, act_pic->width);
            printf(" | ");
            if ((center_pic(large_pic, act_pic->next->height) <= i) && ((i - center_pic(large_pic, act_pic->next->height)) < act_pic->next->height))
                print_row_picture(act_pic->next->picture, i-center_pic(large_pic, act_pic->next->height), act_pic->next->width);
            else print_row_picture(NULL, 0, act_pic->next->width);
            printf("\n");
        }
    }
}

int save_to_box(void)
{
    ELEMENT *actual_pic;

    if (box.picture != NULL) free((void *)box.picture);
    if (!get_size_elements()) {
        box.picture = NULL;
        return -1;
    }
    actual_pic = get_actual_element();
    box.picture = (unsigned char *) malloc(actual_pic->height * actual_pic->width);
    if (box.picture == NULL) {
        errorHandle(ERROR_NO_MEMORY);
        return -1;
    }
    box.height = actual_pic->height;
    box.width = actual_pic->width;
    memcpy(box.picture, actual_pic->picture, (unsigned char) actual_pic->height * actual_pic->width);

    return 0;
}

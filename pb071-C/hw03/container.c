#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "errorHandling.h"
#include "container.h"

LIST list;

void init_list(void)
{
    box.picture = NULL;
    list.actual = NULL;
    list.size = 0;
}

int insert_list(unsigned char *pic, unsigned int width, unsigned char height)
{
    ELEMENT *newElem;

    newElem = (ELEMENT *) malloc(sizeof(ELEMENT));
    if (newElem == NULL)
        errorHandle(ERROR_NO_MEMORY);
    newElem->picture = pic;
    newElem->width = width;
    newElem->height = height;
    if (list.size == 0) {
        list.actual = newElem;
        list.actual->prev = list.actual;
    }
    newElem->next = list.actual;
    newElem->prev = list.actual->prev;
    if (list.size > 0) {
        list.actual->prev->next = newElem;
        list.actual->prev = newElem;
    }
    list.actual = newElem;
    list.size++;
    return 0;
}

void delete_list(void)
{
    ELEMENT *tmp;

    if (!list.size)
        return;
    tmp = list.actual;
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    list.actual = tmp->next;
    list.size--;
    free((void *) tmp);
}

void shift_left_list(void)
{
    if (!list.size)
        return;
    list.actual = list.actual->prev;
}
void shift_right_list(void)
{
    if (!list.size)
        return;
    list.actual = list.actual->next;
}

unsigned int get_size_elements(void)
{
    return list.size;
}

ELEMENT *get_actual_element(void)
{
    return list.actual;
}

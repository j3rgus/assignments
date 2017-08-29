#ifndef CONTAINER_H_
#define CONTAINER_H_

typedef struct element {
    unsigned char *picture;
    unsigned int height;
    unsigned int width;
    struct element *prev;
    struct element *next;
} ELEMENT;

typedef struct {
    unsigned int size;
    ELEMENT *actual;
} LIST;

ELEMENT box;

void init_list(void);
int insert_list(unsigned char *pic, unsigned int width, unsigned char height);
void delete_list(void);
void shift_left_list(void);
void shift_right_list(void);
unsigned int get_size_elements(void);
ELEMENT *get_actual_element(void);

#endif

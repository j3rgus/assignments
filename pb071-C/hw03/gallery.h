#ifndef GALLERY_H_
#define GALLERY_H_

#define BOX 1
#define LOAD_STDIN 0

void make_gallery(void);
int insert_picture(int in);
void delete_actual_picture(void);
void rotate_pictures(char c);
void print_size_of_gallery(void);
void print_gallery(void);
void delete_gallery(void);
int save_to_box(void);

#endif

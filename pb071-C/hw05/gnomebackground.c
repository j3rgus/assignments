#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include "gnomebackground.h"

int com_time(const void *a, const void *b)
{
  return ( (*(IMAGE *)a).modified - (*(IMAGE *)b).modified );
}

int com_lex(const void *a, const void *b)
{
  return ( strcmp((*(IMAGE *)a).file_name, (*(IMAGE *)b).file_name ));
}


int format_compare(char *str)
{
    char *tmp_str;
    int err = 0;

    tmp_str = (char *) malloc(strlen(str)+1);
    if (tmp_str == NULL) {
        exit(1);
    }
    strcpy(tmp_str, str);
    for (unsigned int i = 0; i < strlen(tmp_str); i++) {
        tmp_str[i] = tolower(tmp_str[i]);
    }
    if ((strstr(tmp_str, FORMAT_1) == NULL) && (strstr(tmp_str, FORMAT_2) == NULL))
        err = 1;

    free((void *) tmp_str);
    return err;
}

void add_img(char *path, char *name, time_t modify, int num)
{
    IMAGE *tmp_imgs;

    tmp_imgs = (IMAGE *) realloc(imgs, sizeof(IMAGE) * (num + 1));
    if (tmp_imgs != NULL) {
        imgs = tmp_imgs;
        imgs[num].file_name = (char *) malloc(strlen(name)+1);
        if (imgs[num].file_name == NULL) {
            free_all(num);
            exit(1);
        }
        strcpy(imgs[num].file_name, name);
        imgs[num].full_path = (char *) malloc(strlen(path)+1);
        if (imgs[num].full_path == NULL) {
            free_all(num);
            exit(1);
        }
        strcpy(imgs[num].full_path, path);
        imgs[num].modified = modify;
    } else {
        free_all(num);
        exit(1);
    }
}

void free_all(int num)
{
    if (!num)
        return;
    for (int i = 0; i < num; i++) {
        free((void *) imgs[i].file_name);
        free((void *) imgs[i].full_path);
    }
    free((void *) imgs);
}

int search_pics(char *path)
{
    DIR *p_dir;
    static int num = 0;
    struct dirent *p_dirent;
    struct stat fstat;
    char full_path[257];

    if ((p_dir = opendir(path)) == NULL) {
        return 0;
    }
    while ((p_dirent = readdir(p_dir)) != NULL) {
        sprintf(full_path, "%s%s", path, p_dirent->d_name);
        stat(full_path, &fstat);
        if (S_ISDIR(fstat.st_mode)) {
            if ((p_dirent->d_name[0] != '.')) {
                strcat(full_path, "/");
                search_pics(full_path);
            }
        }else if (!format_compare(p_dirent->d_name)) {
            add_img(full_path, p_dirent->d_name, fstat.st_mtime, num);
            num++;
        }
    }
    closedir(p_dir);
    return num;
}

void print_file(char *name, int num, int dur, int (*p_mode)())
{
    FILE *fw;

    fw = fopen(name, "w");
    if (fw == NULL) {
        perror(name);
        return;
    }
    qsort(imgs, num, sizeof(IMAGE), p_mode);
    fprintf(fw, "<background>\n");
    for (int i = 0; i < num; i++) {
        fprintf(fw, "<static>\n");
        fprintf(fw, "<duration>%d</duration>\n", dur);
        fprintf(fw, "<file>%s</file>\n", imgs[i].full_path);
        fprintf(fw, "</static>\n");
        fprintf(fw, "<transition>\n");
        fprintf(fw, "<duration>5</duration>\n");
        fprintf(fw, "<from>%s</from>\n", imgs[i].full_path);
        fprintf(fw, "<to>%s</to>\n", ((i+1)==(num)) ? imgs[0].full_path : imgs[i+1].full_path);
        fprintf(fw, "</transition>\n");
    }
    fprintf(fw, "</background>\n");

    fclose(fw);
}

int test_dir(char *path)
{
    DIR *dir;

    if ((dir = opendir(path)) == NULL)
        return 1;
    closedir(dir);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errorHandling.h"
#include "gallery.h"

#define flush_stdin while (getchar() != '\n') ;

void print_help(void)
{
    printf("Napoveda pro fungovani:\n");
    printf("q - ukonceni\nn - nacte obrazek do galerie\nr - rotace seznamu vpravo\n");
    printf("l - rotace seznamu vlevo\nd - odstraneni akutalniho obrazku ze seznamu\ns - vypise se velikost seznamu\n");
    printf("------\n");
}

int main(int argc, char* argv[])
{
    int command;

    if ((argc == 1) || (argc > 1 && strcmp(argv[1], "-s")))     //zmenit
        print_help();

    make_gallery();
    while (1) {
        printf(">");
        command = getchar();
        if (getchar() != '\n') {
            command = -1;
            flush_stdin;        // vyprazdnim vstupny buffer
        }
        switch (command) {
            case 'q' : delete_gallery();
                return 0;
            case 'n' : insert_picture(LOAD_STDIN);
                print_gallery();
                break;
            case 'd' : delete_actual_picture();
                print_gallery();
                break;
            case 'l' :
            case 'r' : rotate_pictures(command);
                print_gallery();
                break;
            case 's' : print_size_of_gallery();
                break;
            case 'c' : save_to_box();
                break;
            case 'v' : insert_picture(BOX);
                print_gallery();
                break;
            case 'x' : save_to_box();
                delete_actual_picture();
                print_gallery();
                break;
            case -1 :
            default : errorHandle(ERROR_WRONG_CMD);
                break;
        }
    }

    return 0;
}

#include "gnomebackground.h"

int main(int argc, char *argv[])
{
    int num = 0;

    imgs = NULL;
    if ((argc < 4) || (argc > 5)) {
        printf("Usage: ./gnomebackground output.xml path_to_find duration [-lg]\n");
        exit(1);
    }
    if (test_dir(argv[2]))
        exit(1);
    num = search_pics(argv[2]);
    print_file(argv[1], num, atoi(argv[3]), ((argc == 5) && !strcmp(argv[4], "-lg") ? com_lex : com_time));
    printf("Subor %s obsahuje %d pozadi.\n", argv[1], num);
    free_all(num);
    return 0;
}

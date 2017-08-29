#include <stdio.h>
#include <stdint.h>
#include "utfconvert.h"

#define move_string(x)       while (*x++ != ' ') ;

static void print_dek(uint32_t utf16)
{
    int n = (get_num_bytes(utf16) <= 2) ? 2 : 4;

    for (int i = n-1; i >= 0; i--) {
        printf("%03d", (utf16 >> i*8 ) & 0xff );
    }
}

int main(void)
{
    char inputStr[255];
    char *in = inputStr;
    uint32_t utf8_in;
    int n;

    fgets(in, 255, stdin);
    sscanf(in,"%d", &n);

    for (int i = 0; i < n; i++) {
        move_string(in);            //posuvam sa na zaciatok dalsieho 'podretazca'
        sscanf(in, "%X", &utf8_in);
        utf8_in = utfconvert(utf8_in);
        print_dek(utf8_in);
        printf(" 0x%0*X\n", (get_num_bytes(utf8_in) <= 2) ? 4 : 8, utf8_in);
    }

    return 0;
}

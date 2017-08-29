#include "utfconvert.h"

//UTF 8 constants
#define UTF8_BYTE_TWO_MASK      0x1F3F
#define UTF8_BYTE_THREE_MASK    0x0F3F3F
#define UTF8_BYTE_FOUR_MASK     0x073F3F3F

//UTF 16 constants
#define UTF16_OVER_RANGE    0x00010000
#define UTF16_MASK          0xD800DC00
#define MASK_L              0x03FF
#define MASK_H              0x0FFC00

int get_num_bytes(uint32_t uni)
{
    int n = 1;
    while (uni >>= 8)
        n++;
    return n;
}

static uint32_t unitoutf16(uint32_t uniC)
{
    uint32_t utf_16 = uniC;
    uint32_t aux = 0;

    if (utf_16 < UTF16_OVER_RANGE) return utf_16;

    utf_16 -= UTF16_OVER_RANGE;     //odcitam 0x10000, tak ako pozaduje utf16 - dostanem 20bit
    aux |= utf_16 & MASK_H;  //odstranim spodnich 10bit a priradim do aux
    aux <<= 6;                      //posuniem o 6 bitov dolava - kvoli maske UTF-16 formatu
    aux |= utf_16 & MASK_L;  //zapisem do spodnych 10 bitov do aux
    utf_16 = UTF16_MASK | aux;      //aux prekryjem s maskou formatu UTF-16 - dostanem konecny tvar

    return utf_16;
}

static uint32_t utf8touni(uint32_t utf_8)
{
    uint32_t uniCode = 0;
    uint32_t aux = 0;
    int n = get_num_bytes(utf_8);

    if (utf_8 <= 0x7f ) return utf_8;

    if (n == 2) aux = utf_8 & UTF8_BYTE_TWO_MASK;
    if (n == 3) aux = utf_8 & UTF8_BYTE_THREE_MASK;
    if (n == 4) aux = utf_8 & UTF8_BYTE_FOUR_MASK;

    for (int i = n-1; i >= 0; i--) {        //do uniCode davam najvyssie bajty, nasledne posuvam o 6 bitov dolava (inicialne uniCode = 0)
        uniCode <<= 6;                      //--toto sa deje podla poctu bajtov ktory zistim a zapisem do n
        uniCode |= ((aux >> 8*i) & 0xff);
    }

    return uniCode;
}

uint32_t utfconvert(uint32_t utf_8)
{
    return unitoutf16(utf8touni(utf_8));
}

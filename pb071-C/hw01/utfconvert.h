#ifndef _UTF_CONVERT_H
#define _UTF_CONVERT_H

#include <stdint.h>

// nie su ziadne zdielane konstanty

extern uint32_t utfconvert(uint32_t utf_8);
extern int get_num_bytes(uint32_t uni);

#endif

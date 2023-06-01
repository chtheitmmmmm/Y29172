//
// Created by cmtheit on 23-5-31.
//

#ifndef SERVER_UTIL_H
#define SERVER_UTIL_H
#include <stdio.h>
#include <hv/hloop.h>

size_t get_line(char * restrict dest, size_t max_size);
void hio_write_str(hio_t * hio, const char * str);

#endif //SERVER_UTIL_H

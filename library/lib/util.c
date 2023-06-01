//
// Created by cmtheit on 23-5-31.
//

#include "util.h"

size_t get_line(char * restrict dest, size_t max_size) {
    int c;
    size_t size = 0;
    while ((c = getchar()) != '\n' && c != EOF && size < max_size) {
        dest[size++] = (char) c;
    }
    return size;
}

void hio_write_str(hio_t * hio, const char * str) {
    hio_write(hio, str, strlen(str));
}
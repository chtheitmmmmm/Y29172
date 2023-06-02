



#include "string.h"
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>



char ** split(char * string) {
    size_t seg_num = 1;
    size_t tot = strlen(string);
    for (size_t i = 0; i < tot; ++i) {
        if (isspace(string[i]))
            seg_num++;
    }
    char ** ctn = calloc(seg_num + 1, sizeof(char*));
    if (!ctn) {
        return NULL;
    }
    size_t idx = 0;
    ctn[0] = malloc((tot + 1) * sizeof (char));
    for (size_t i = 0, l = 0; i < tot; ++i) {
        if (isspace(string[i])) {
            ctn[idx] = realloc(ctn[idx], (l + 1) * sizeof(char));
            if (!ctn[idx]) {
                for (int j = 0; j < idx; ++j) {
                    free(ctn[j]);
                }
                free(ctn);
                return NULL;
            }
            ctn[idx][l] = '\0';
            l = 0;
            idx++;
            ctn[idx] = malloc(sizeof(char) * (tot + 1));
            if (!ctn[idx]) {
                for (int j = 0; j < idx; ++j) {
                    free(ctn[j]);
                }
                free(ctn);
                return NULL;
            }
        } else {
            ctn[idx][l++] = string[i];
        }
        if (i == tot - 1) {
            ctn[idx][l] = '\0';
        }
    }
    return ctn;
}

size_t strings_len(char ** strings) {
    size_t len = 0;
    while (*strings) {
        len++;
        strings++;
    }
    return len;
}

void strings_free(char ** strings) {
    char ** head = strings;
    while (*strings) {
        free(*strings);
        strings++;
    }
    free(head);
}

MyAtoiResult myatoi(const char * src) {
    MyAtoiResult res = result_new_err(0);
    if (src) {
        int buf = 0;
        while (*src) {
            if (isdigit(*src)) {
                if (buf < INT_MAX * 1.0 / 10) {
                    buf *= 10;
                    buf += *src - '0';
                } else {
                    break;
                }
                src++;
            } else {
                break;
            }
        }
        if (!*src) {
            result_turn_ok(res, buf);
        }
    }
    return res;
}
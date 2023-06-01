//
// Created by cmtheit on 23-6-1.
//

#include "string.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char ** split(char * string, char delimiter) {
    size_t seg_num = 1;
    size_t tot = strlen(string);
    for (size_t i = 0; i < tot; ++i) {
        if (string[i] == delimiter)
            seg_num++;
    }
    char ** ctn = calloc(seg_num + 1, sizeof(char*));
    if (!ctn) {
        return NULL;
    }
    size_t idx = 0;
    ctn[0] = malloc((tot + 1) * sizeof (char));
    for (size_t i = 0, l = 0; i < tot; ++i) {
        if (string[i] == delimiter) {
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
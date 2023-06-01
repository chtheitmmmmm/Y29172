//
// Created by cmtheit on 23-6-1.
//

#ifndef SERVER_STRING_H
#define SERVER_STRING_H

#include <string.h>
char ** split(char * string, char delimiter);
size_t strings_len(char ** strings);
void strings_free(char ** strings);

#endif //SERVER_STRING_H





#ifndef SERVER_STRING_H
#define SERVER_STRING_H

#include <string.h>
#include <lib/result.h>

char ** split(char * string);
size_t strings_len(char ** strings);
void strings_free(char ** strings);

typedef Result(int, int) MyAtoiResult;
MyAtoiResult myatoi(const char * src);

#endif //SERVER_STRING_H

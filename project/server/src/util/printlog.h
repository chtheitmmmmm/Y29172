//
// Created by cmtheit on 23-5-27.
//

#ifndef SERVER_PRINTLOG_H
#define SERVER_PRINTLOG_H
#include <time.h>
#include <stdio.h>
#include <chalk.c/chalk.h>
#include <event2/event.h>

#define SUCCESSFULLY CHALK_GREEN("successfully")
#define FAILED       CHALK_RED("failed")

static const char * format = "%Y-%m-%d %H:%M:%S";
static char buffer[4 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1];


#define print_log(...) { \
    time_t t = time(NULL);                                                \
    strftime(buffer, sizeof(buffer), format, localtime(&t));              \
    printf(CHALK_LIGHT_BLACK("%s "), buffer); \
    printf(__VA_ARGS__);    \
}

#define print_logl(...) { \
    print_log(__VA_ARGS__);  \
    printf("\n");           \
}

void event2_print_log(int severity, const char * msg);

#endif //SERVER_PRINTLOG_H

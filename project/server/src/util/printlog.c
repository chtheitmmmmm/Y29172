//
// Created by cmtheit on 23-5-28.
//

#include "printlog.h"

void event2_print_log(int severity, const char * msg) {
    print_log("");
    switch (severity) {
        case EVENT_LOG_WARN:
            printf(CHALK_BLUE(CHALK_BG_YELLOW("WARN"))" ");
            break;
        case EVENT_LOG_ERR:
            printf(CHALK_BLUE(CHALK_BG_RED("ERR"))" ");
            break;
        case EVENT_LOG_DEBUG:
            printf(CHALK_BG_LIGHT_BLACK("DB")" ");
        case EVENT_LOG_MSG:
        default:
            break;
    }
    printf("%s\n", msg);
}
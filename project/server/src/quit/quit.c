//
// Created by cmtheit on 23-5-30.
//

#include "quit.h"
#include "../event/event.h"
#include "../util/printlog.h"

void quit() {
    global_events_quit();
    event2_print_log(EVENT_LOG_MSG, "Server quit.");
}
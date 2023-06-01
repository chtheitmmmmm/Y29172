//
// Created by cmtheit on 23-5-30.
//

#include "quit.h"
#include "../event/event.h"
#include "../util/printlog.h"
#include "../core/server.h"
#include "../core/session.h"
#include "../core/victim.h"

void quit() {
    session_quit();
    global_events_quit();
    server_quit();
    victims_quit();
    event2_print_log(EVENT_LOG_MSG, "Server quit "CHALK_GREEN("successfully")".");
}
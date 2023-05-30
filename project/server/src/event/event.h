//
// Created by cmtheit on 23-5-29.
//

#ifndef SERVER_EVENT_H
#define SERVER_EVENT_H

#include <event2/event.h>
#include <lib/result.h>
#include <pthread.h>

typedef struct GlobalEvents {
    struct event_base * event_base;
    struct event * init_event;
    struct event * sigint_event;
    pthread_mutex_t lock;
} GlobalEvents;

extern GlobalEvents global_events;

enum EventsInitErrCode {
    EventsInitErrorHeap,
    EventsInitErrorOther
};

typedef Result(enum EventsInitErrCode, int) EventsInitResult;

EventsInitResult global_events_init();
void global_event_loop();
void global_events_trigger_init_event();
void global_events_quit();

#endif //SERVER_EVENT_H

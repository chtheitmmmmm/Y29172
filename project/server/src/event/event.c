



#include "event.h"
#include "../config/config.h"
#include "../util/printlog.h"
#include "../quit/quit.h"
#include "../core/server.h"
#include "../core/session.h"
#include <pthread.h>

GlobalEvents global_events = {
        .event_base = NULL,
        .init_event = NULL,
        .sigint_event = NULL,
};

static void init_event_cb(int, short, void *);  // called when init_event triggered
static void sigint_event_cb(int, short, void *); // called when receive a SIGINT signal
static void session_establish_event_cb(int, short, void *); // called when session established.

void global_event_loop() {
    event_base_loop(global_events.event_base, EVLOOP_NO_EXIT_ON_EMPTY);
}

EventsInitResult global_events_init() {
    EventsInitResult res = result_new_ok(0);        // 初始化函数的结果
    global_events.event_base = event_base_new();    // 初始化主事件循环
    if (!global_events.event_base) {
        result_turn_err(res, EventsInitErrorHeap);  // 分配 event_base 失败
        return res;
    }
    global_events.init_event = event_new(global_events.event_base, -1, 0, init_event_cb, NULL); // init 事件
    if (!global_events.init_event) {
        event_base_free(global_events.event_base);  // 释放 event_base
        result_turn_err(res, EventsInitErrorHeap);  // 分配 init_event 失败
        return res;
    }
    global_events.sigint_event = evsignal_new(global_events.event_base, SIGINT, sigint_event_cb, NULL); // sigint 信号处理回调注册
    if (!global_events.sigint_event) {
        event_free(global_events.init_event);
        event_base_free(global_events.event_base);  // 释放 init_event
        result_turn_err(res, EventsInitErrorHeap);  // 分配 sigint_event 失败
        return res;
    }
    pthread_mutex_init(&global_events.lock, NULL);      // 初始化互斥锁
    event_add(global_events.sigint_event, NULL);            // 添加 sigint_event
    return res;
}

void global_events_trigger_init_event() {
    event_active(global_events.init_event, 0, 0);
}

// 执行清理工作
void global_events_quit() {
    pthread_mutex_lock(&global_events.lock);
    event_base_loopbreak(global_events.event_base);
    event_free(global_events.init_event);
    event_free(global_events.sigint_event);
    event_base_free(global_events.event_base);
    pthread_mutex_unlock(&global_events.lock);
    pthread_mutex_destroy(&global_events.lock);
}

static void init_event_cb(int, short, void *) {
    config_report();
    event2_print_log(EVENT_LOG_MSG, "Server init ok");
    server_start();
}

static void sigint_event_cb(int, short, void *) {
    quit();
    exit(0);
}
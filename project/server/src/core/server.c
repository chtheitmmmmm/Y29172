//
// Created by cmtheit on 23-5-26.
//


#include "server.h"
#include "../util/printlog.h"
#include <pthread.h>
#include <event2/event.h>
#include <hv/hloop.h>
#include <unistd.h>

Server server;
Session session;

// 为主控端提供服务
void server_m_on_accept(hio_t * hio) {
    static const char * welcome_msg = "Welcome to the horse server, your server is right for you.\n";
    size_t welcome_msg_len = strlen(welcome_msg);
    size_t sent = hio_write(hio, welcome_msg, welcome_msg_len);
    if (sent != welcome_msg_len) {

    }
    hio_close(hio);
}

// 为受控端提供服务
void server_e_on_accept(hio_t * hio) {

}

void server_init() {
    server.m.port = config.mport;
    server.m.hloop = hloop_new(0);
    hloop_create_ssl_server(server.m.hloop, "0.0.0.0", server.m.port, server_m_on_accept);
    server.e.port = config.eport;
    server.e.hloop = hloop_new(0);
    hloop_create_ssl_server(server.e.hloop, "0.0.0.0", server.m.port, server_e_on_accept);

    pthread_mutex_init(&session.lock, NULL);    // session lock
    session_id_t none_id = option_new_none();
    session.id = none_id;   // set session id to none
}


static void * server_mptherad_routine(void *) {
    event2_print_log(EVENT_LOG_MSG, "Controller side server start...");
    hloop_run(server.m.hloop);
}

static void * server_cpthread_routine(void *) {
    event2_print_log(EVENT_LOG_MSG, "Controllee side server start...");
    hloop_run(server.e.hloop);
}

void server_start() {
    // 启动服务器线程
    pthread_create(&server.m.pthread, NULL, server_mptherad_routine, NULL);
    pthread_create(&server.e.pthread, NULL, server_cpthread_routine, NULL);
}

void server_quit() {
    // 停止服务器循环，停止服务器线程
    hloop_stop(server.m.hloop);
    pthread_join(server.m.pthread, NULL);
    hloop_free(&server.m.hloop);

    hloop_stop(server.e.hloop);
    pthread_join(server.e.pthread, NULL);
    hloop_free(&server.e.hloop);

    // 释放 session
    pthread_mutex_lock(&session.lock);
    pthread_mutex_unlock(&session.lock);
    pthread_mutex_destroy(&session.lock);
}
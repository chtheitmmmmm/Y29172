//
// Created by cmtheit on 23-5-31.
//

#include "session.h"
#include "../util/printlog.h"
#include "../util/string.h"
#include "../config/config.h"
#include "../event/event.h"
#include "server.h"
#include "cmd.h"

Session session;

static void session_on_close(hio_t *);
static void session_on_read(hio_t *, void * buf, int readbytes);
static void session_on_login();

session_id_t session_id_alloc() {
    static session_id_t last_id = 0;
    return ++last_id;
}

bool session_occupied() {
    return session.state != SessionStatePending;
}

static inline void session_reset() {
    session.io = NULL;
    session.state = SessionStatePending;
    session.id = -1;
}

SessionInitResult session_init() {
    SessionInitResult res = result_new_ok(0);
    if (pthread_mutex_init(&session.lock, NULL)) {
        result_turn_err(res, SessionInitErrMutexInit);
        return res;
    }
    session_reset();
    return res;
}

SessionEstablishResult session_establish(hio_t * io) {  // 尝试创建一个会话
    SessionEstablishResult res = result_new_ok(0);
    static unpack_setting_t unpack_setting = {
            .mode = UNPACK_BY_DELIMITER,
            .package_max_length = DEFAULT_PACKAGE_MAX_LENGTH,
            .delimiter = CLR_DELIMITER,
            .delimiter_bytes = CLR_DELIMITER_BYTES
    };
    hio_set_unpack(io, &unpack_setting);
    if (session_occupied()) {
        result_turn_err(res, SessionEstablishErrOccupied);
        server_write_msg(io,
                  "另一个会话已经建立。请求驳回。");
        hio_close(io);
        return res;
    }
    pthread_mutex_lock(&session.lock);
    session.state = SessionStateTryEstablish;
    session.id = session_id_alloc();    // 分配会话id
    session.io = io;
    hio_setcb_read(io, session_on_read);
    hio_setcb_close(io, session_on_close);
    session.state = SessionStateGetUserName;
    hio_read_start(io);
    pthread_mutex_unlock(&session.lock);
    return res;
}

static void session_on_close(hio_t *) {    // 关闭会话的清理工作
    event2_print_log(EVENT_LOG_MSG, "会话关闭");
    session_reset();
}

static void session_on_read(hio_t *, void * buf, int readbytes) {    // 从控制端读取数据
    static uint8_t pswsha[SHA256_SIZE_BYTES];
    pthread_mutex_lock(&session.lock);
    switch (session.state) {
        case SessionStateGetUserName:       // 获取用户名
            if (strncmp(buf, config.uname, readbytes - 1)) {
                server_write_msg(session.io,
                          "用户名错误，请检查服务器启动日志");
                hio_close(session.io);
                session_reset();
            } else {
                session.state = SessionStateGetPassword;
            }
            break;
        case SessionStateGetPassword:       // 获取密码
            sha256(buf, readbytes - 1, pswsha);
            if (strncmp(pswsha, config.pswsha, SHA256_SIZE_BYTES)) {
                server_write_msg(session.io,
                          "密码错误，请检查服务器启动日志");
                hio_close(session.io);
                session_reset();
            } else {
                hevent_t ev;
                memset(&ev, 0, sizeof(ev));
                ev.event_type = (hevent_type_e)(HEVENT_TYPE_CUSTOM + 1);
                ev.cb = session_on_login;
                hloop_post_event(server.m.hloop, &ev);
            }
            break;
        case SessionStateEstablished:
            if (readbytes > 1) {
                print_logl("接收命令: " CHALK_YELLOW("%.*s"), readbytes - 1, (char *)buf);
                *(char*)(buf + readbytes - 1) = '\0';
                char ** argv = split((char*)buf, ' ');
                cmd_exe(buf, strings_len(argv), argv);
                strings_free(argv);
            }
            break;
        case SessionStatePending:
        default:
            event2_print_log(EVENT_LOG_ERR, "服务器内部错误");
            break;
    }
    pthread_mutex_unlock(&session.lock);
}

static void session_on_login() {
    pthread_mutex_lock(&session.lock);
    session.state = SessionStateEstablished;
    server_write_msg(session.io, "欢迎，您的服务器仍在运行中（输入 help 查看所有命令）");
    pthread_mutex_unlock(&session.lock);
    session_report();
}

void session_report() {     // 输出当前会话
    print_log("当前会话: ");
    if (session_occupied()) {
        printf(CHALK_GREEN("%d"), session.id);
    } else {
        printf(CHALK_RED("none"));
    }
    printf("\n");
}

SessionUnEstablishResult session_unestablish() {    // 销毁会话
    SessionUnEstablishResult res = result_new_ok(0);
    if (!session_occupied() || !session.io) {
        result_turn_err(res, SessionUnEstablishErrOther);
        return res;
    }
    server_write_msg(session.io, "再见！");
    pthread_mutex_lock(&session.lock);
    hio_close(session.io);
    session_reset();
    pthread_mutex_unlock(&session.lock);
    return res;
}

void session_quit() {   // 退出程序是时调用
    if (session_occupied()) {
        event2_print_log(EVENT_LOG_MSG, "关闭当前会话...");
        SessionUnEstablishResult s_res = session_unestablish();
        if (result_is_ok(s_res)) {
            event2_print_log(EVENT_LOG_MSG, "当前会话关闭 "SUCCESSFULLY".");
        } else {
            event2_print_log(EVENT_LOG_ERR, "当前会话关闭 "FAILED".");
        }
    } else {
        event2_print_log(EVENT_LOG_MSG, "当前无会话");
    }
    pthread_mutex_destroy(&session.lock);
}
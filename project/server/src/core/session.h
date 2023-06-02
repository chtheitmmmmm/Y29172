



#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H
#include <lib/option.h>
#include <pthread.h>
#include <stdbool.h>
#include <lib/result.h>
#include <hv/hloop.h>

typedef int session_id_t;

enum SessionState {
    SessionStatePending,
    SessionStateTryEstablish,
    SessionStateGetUserName,
    SessionStateGetPassword,
    SessionStateEstablished
};

typedef struct Session {
    enum SessionState state;
    session_id_t id;        // 会话id。如果存在则不允许接受新的会话
    pthread_mutex_t lock;   // 会话锁。一个服务器同一时间只能服务一个会话
    hio_t * io;             // the io of this session
} Session;
extern Session session; // 全局会话对象

session_id_t session_id_alloc();
bool session_occupied();
enum SessionInitErrCode {
    SessionInitErrMutexInit,
    SessionInitErrOther
};
typedef Result(enum SessionInitErrCode, int) SessionInitResult;
SessionInitResult session_init();

enum SessionEstablishErrCode {
    SessionEstablishErrOther,
    SessionEstablishErrOccupied
};
typedef Result(enum SessionEstablishErrCode, int) SessionEstablishResult;
SessionEstablishResult session_establish(hio_t * io);

void session_report();

enum SessionUnEstablishErrCode {
    SessionUnEstablishErrOther,
};
typedef Result(enum SessionUnEstablishErrCode, int) SessionUnEstablishResult;
SessionUnEstablishResult session_unestablish();
void session_quit();

#endif //SERVER_SESSION_H

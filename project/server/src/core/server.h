//
// Created by cmtheit on 23-5-26.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "../config/config.h"
#include <hv/hloop.h>

typedef struct SingleServer {
    port_t port;
    hloop_t * hloop;
    hio_t * io;
    pthread_t pthread;
} SingleServer; // 一个服务器

typedef struct Server {
    SingleServer m;     // 主控端
    SingleServer e;     // 被控端
} Server;   // 服务器，包含两个子服务器
extern Server server; // 全局服务器对象

enum ServerInitErrCode {
    ServerInitErrOther,
    ServerInitErrSSLServerCreate,
    ServerInitErrSSLCtx,
    ServerInitErrConfig,
};
typedef Result(enum ServerInitErrCode, int) ServerInitResult;

ServerInitResult server_init();     // 服务器初始化
void server_start();    // 服务器启动
void server_quit();     // 服务器退出

#endif //SERVER_SERVER_H

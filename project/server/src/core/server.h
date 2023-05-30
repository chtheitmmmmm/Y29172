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
    pthread_t pthread;
} SingleServer; // 一个服务器

typedef struct Server {
    SingleServer m;     // 主控端
    SingleServer e;     // 被控端
} Server;   // 服务器，包含两个子服务器
extern Server server; // 全局服务器对象

typedef Option(int) session_id_t;
typedef struct Session {
    session_id_t id;        // 会话id。如果存在则不允许接受新的会话
    pthread_mutex_t lock;   // 会话锁。一个服务器同一时间只能服务一个会话
} Session;
extern Session session; // 全局会话对象

void server_init();     // 服务器初始化
void server_start();    // 服务器启动
void server_quit();     // 服务器退出

#endif //SERVER_SERVER_H

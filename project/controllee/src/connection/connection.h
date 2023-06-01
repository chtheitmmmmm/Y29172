//
// Created by cmtheit on 23-6-1.
//

#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#ifndef SERVER_IP       // 这里可以定义服务器 IP
#define SERVER_IP "127.0.0.1"
#endif
#ifndef SERVER_PORT
#define SERVER_PORT 29001
#endif

#include <hv/hloop.h>
extern hloop_t * main_loop;
extern hio_t * main_hio;

void connection_init(); // 与服务器建立连接

#endif //SERVER_CONNECTION_H

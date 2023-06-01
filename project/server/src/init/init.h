//
// Created by cmtheit on 23-5-27.
//

#ifndef SERVER_INIT_H
#define SERVER_INIT_H
#include <lib/result.h>
#include <event2/event.h>

enum InitResultErrCode {
    InitResultErrOther,

    InitResultErrInitEventHeap,       // 初始化事件堆不足
    InitResultErrInitEventOther,      // 初始化事件其他的错误

    InitResultErrServerOther,         // 服务器初始化出错
    InitResultErrServerSSLCtx,        // 服务器SSL上下文初始化失败
    InitResultErrSSLServerCreate,     // SSL 服务器创建失败
    InitResultErrServerConfig,        // 服务器配置出错

    InitResultErrSessionOther,        // 会话初始化出错
    InitResultErrSessionMutexInit,    // 会话初始化因为全局对象互斥锁初始化失败

    InitResultErrCmdsOther,             // 初始化命令出错

    InitResultErrVictimOther,           // 初始化受害者表失败
};

typedef Result(enum InitResultErrCode, int) InitResult;

InitResult init(int argc, const char ** argv);

#endif //SERVER_INIT_H

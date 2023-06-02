#ifndef SERVER_HIOS_H
#define SERVER_HIOS_H
#ifndef CONFIG_DEFAULT_SERVER_IP        // 编译时确定服务器 ipv4 地址
#define CONFIG_DEFAULT_SERVER_IP "127.0.0.1"
#endif
#ifndef CONFIG_DEFAULT_SERVER_PORT      // 编译时确定服务器端口
#define CONFIG_DEFAULT_SERVER_PORT 29001
#endif
#include <hv/hloop.h>
#include <protocol/protocol.h>
#include <lib/result.h>
#define STRINGFY(x) #x
#define TOSTRING(x) STRINGFY(x)
#pragma message("使用服务器IP地址：" TOSTRING(CONFIG_DEFAULT_SERVER_IP))
#pragma message("使用服务器端口号：" TOSTRING(CONFIG_DEFAULT_SERVER_PORT))

extern hio_t * sockio;
extern hio_t * stdinio;
extern hloop_t * hloop;


enum HioInitErrCode {
    HioInitErrOther
};

typedef Result(enum HioInitErrCode, int) HioInitResult;
HioInitResult hios_init();

#endif //SERVER_HIOS_H

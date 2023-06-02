



#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <protocol/protocol.h>
#include <lib/option.h>
#include <lib/result.h>
#include <SHA256/sha256.h>

#define CONFIG_DEFAULT_MPORT MPORT_DEFAULT
#define CONFIG_DEFAULT_EPORT EPORT_DEFAULT
#define CONFIG_DEFAULT_UNAME USER_NAME_DEFAULT
#define CONFIG_DEFAULT_PSW USER_PSW_DEFAULT

typedef uint16_t port_t;

typedef struct Config {
    port_t mport; // 主控程序监听端口
    port_t eport; // 受控程序监听端口
    const char * uname;   // 用户名
    uint8_t pswsha[SHA256_SIZE_BYTES];  // 密码的 sha256 编码
} Config;

enum ConfigInitErrCode {
    ConfigInitErrorOther
};

extern Config config;
typedef Option(const char*) ConfigUnameOption;
typedef Option(const char*) ConfigPswOption;
typedef Option(port_t) ConfigPortOption;
typedef Result(enum ConfigInitErrCode, int) ConfigInitResult;

ConfigInitResult config_init(ConfigUnameOption uname_opt, ConfigPswOption psw_opt, ConfigPortOption mport_opt, ConfigPortOption eport_opt);
void config_report();
#endif //SERVER_CONFIG_H

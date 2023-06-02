



#ifndef CONTROLLER_CONFIG_H
#define CONTROLLER_CONFIG_H

#define CONFIG_DEFAULT_SERVER_IP "127.0.0.1"
#define CONFIG_DEFAULT_SERVER_PORT 29000

#include <lib/option.h>
#include <lib/result.h>
#include <SHA256/sha256.h>

typedef uint16_t port_t;

typedef struct Config {
    const char * server_ip; // 服务器 ipv4 地址
    port_t server_port;     // 服务器端口
} Config;

enum ConfigInitErrCode {
    ConfigInitErrorOther
};

extern Config config;
typedef Option(const char*) ConfigServerIPOption;
typedef Option(port_t) ConfigServerPortOption;
typedef Result(enum ConfigInitErrCode, int) ConfigInitResult;

ConfigInitResult config_init(ConfigServerIPOption sip_opt, ConfigServerPortOption sport_opt);
void config_report();
#endif //CONTROLLER_CONFIG_H

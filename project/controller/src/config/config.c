



#include "config.h"
#include <chalk.c/chalk.h>
#include <stdlib.h>
#include <stdio.h>

Config config;

ConfigInitResult config_init(ConfigServerIPOption sip_opt, ConfigServerPortOption sport_opt) {
    config.server_ip = option_is_none(sip_opt) ? CONFIG_DEFAULT_SERVER_IP : option_unwrap(sip_opt);
    config.server_port = option_is_none(sport_opt) ? CONFIG_DEFAULT_SERVER_PORT : option_unwrap(sport_opt);
    ConfigInitResult res = result_new_ok(0);
    return res;
}

void config_report() {
    printf("Now using config:\n");
    printf("-- Server ip: " CHALK_BLUE("%s\n"), config.server_ip);
    printf("-- Server port: " CHALK_BLUE("%d\n"), config.server_port);
}
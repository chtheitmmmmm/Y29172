



#include "config.h"
#include "../util/printlog.h"
#include <chalk.c/chalk.h>
#include <stdlib.h>

Config config;

ConfigInitResult config_init(
        ConfigUnameOption uname_opt,
        ConfigPswOption psw_opt,
        ConfigPortOption mport_opt,
        ConfigPortOption eport_opt
) {
    config.uname = option_is_none(uname_opt) ? CONFIG_DEFAULT_UNAME : option_unwrap(uname_opt);
    const char * password = option_is_none(psw_opt) ? CONFIG_DEFAULT_PSW : option_unwrap(psw_opt);
    sha256(password, strlen(password), config.pswsha);
    config.mport = option_is_none(mport_opt) ? CONFIG_DEFAULT_MPORT : option_unwrap(mport_opt);
    config.eport = option_is_none(eport_opt) ? CONFIG_DEFAULT_EPORT : option_unwrap(eport_opt);
    ConfigInitResult res = result_new_ok(0);
    return res;
}

void config_report() {
    event2_print_log(EVENT_LOG_MSG, "Now using config:");
    print_logl("-- user name: " CHALK_YELLOW("%s"), config.uname);
    event2_print_log(EVENT_LOG_MSG, "-- user password sha256: ");
    const int col = 8;
    for (int i = 0; i < SHA256_SIZE_BYTES / col; ++i) {
        print_log("\t");
        for (int j = 0; j < col; ++j) {
            printf(CHALK_YELLOW("%02x "), config.pswsha[i * col + j]);
        }
        printf("\n");
    }
    print_logl("-- Controller port: " CHALK_BLUE("%d"), config.mport);
    print_logl("-- Controllee port: " CHALK_BLUE("%d"), config.eport);
}
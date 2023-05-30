#include <stdio.h>
#include <hv/hloop.h>
#include "init/init.h"
#include "config/config.h"
#include <chalk.c/chalk.h>

void on_connect() {
    printf("Connection installed\n");
}

void on_close() {
    printf("Connection closed\n");
}

int main(int argc, const char ** argv) {
    hloop_t * hloop = hloop_new(0);
    InitResult result = init(argc, argv);
    if (result_is_err(result)) {
        switch (result_unwrap(result)) {
            case InitResultConfigErr:
                printf(CHALK_RED("Config init error!\n"));
                break;
            case InitResultErrOther:
            default:
                printf(CHALK_RED("Init error!\n"));
                break;
        }
        return 1;
    }
    if (!hloop) {
        printf(CHALK_RED("Error when new a hloop!\n"));
        exit(1);
    }
    config_report();
    printf("Connecting to the server...\n");
    hloop_create_ssl_client(hloop, config.server_ip, config.server_port, on_connect, NULL);
    hloop_run(hloop);
    return 0;
}
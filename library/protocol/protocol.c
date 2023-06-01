//
// Created by cmtheit on 23-5-31.
//

#include "protocol.h"
#include "lib/util.h"

void write_msg(hio_t * hio, const char * msg, const char * deli, int deli_size) {
    if (strlen(msg))
        hio_write_str(hio, msg);
    hio_write(hio, deli, deli_size);
}

void server_write_msg(hio_t * hio, const char * msg) {
    write_msg(hio, msg, SERVER_DELIMITER, SERVER_DELIMITER_BYTES);
}

void clr_write_msg(hio_t * hio, const char * msg) {
    write_msg(hio, msg, CLR_DELIMITER, CLR_DELIMITER_BYTES);
}
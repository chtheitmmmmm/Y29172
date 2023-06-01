//
// Created by cmtheit on 23-6-1.
//

#ifndef SERVER_HIOS_H
#define SERVER_HIOS_H
#include <hv/hloop.h>
#include <protocol/protocol.h>
#include <lib/result.h>

extern hio_t * sockio;
extern hio_t * stdinio;
extern hloop_t * hloop;


enum HioInitErrCode {
    HioInitErrOther
};

typedef Result(enum HioInitErrCode, int) HioInitResult;
HioInitResult hios_init();

#endif //SERVER_HIOS_H

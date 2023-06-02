



#ifndef SERVER_INIT_H
#define SERVER_INIT_H
#include <lib/result.h>

enum InitResultErrCode {
    InitResultErrOther,
    InitResultErrConfig,
    InitResultErrHio,
    InitResultErrAccount,
};

typedef Result(enum InitResultErrCode, int) InitResult;

InitResult init(int argc, const char ** argv);

#endif //SERVER_INIT_H

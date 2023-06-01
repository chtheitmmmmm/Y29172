//
// Created by cmtheit on 23-6-1.
//

#ifndef SERVER_ACCOUNT_H
#define SERVER_ACCOUNT_H
#include <lib/result.h>

extern char user_name[];
extern char user_password[];

enum AccountInitErrCode {
    AccountInitErrOther,
    AccountINitErrInput
};

typedef Result(enum AccountInitErrCode, int) AccountInitResult;

AccountInitResult account_init();

#endif //SERVER_ACCOUNT_H

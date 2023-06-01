//
// Created by cmtheit on 23-6-1.
//

#include "account.h"
#include "protocol/protocol.h"
#include "lib/util.h"

char user_name[USER_NAME_MAX] = {0};
char user_password[USER_PSW_MAX] = {0};

AccountInitResult account_init() {
    // 获取用户名和密码
    AccountInitResult res = result_new_ok(0);
    size_t input = 0;
    printf("请输入用户名（最大长度%d，默认为%s）\n", USER_NAME_MAX, USER_NAME_DEFAULT);
    input = get_line(user_name, USER_NAME_MAX);     // 获取用户名
    if (input == USER_NAME_MAX) {
        result_turn_err(res, AccountINitErrInput);
        return res;
    }
    printf("请输入密码（最大长度%d，默认为%s）\n", USER_PSW_MAX, USER_PSW_DEFAULT);
    input = get_line(user_password, USER_PSW_MAX);
    if (input == USER_NAME_MAX) {
        result_turn_err(res, AccountINitErrInput);
        return res;
    }
    return res;
}
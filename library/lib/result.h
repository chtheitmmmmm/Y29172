//
// Created by cmtheit on 23-5-27.
//

#ifndef SERVER_RESULT_H
#define SERVER_RESULT_H
#define RESULT_OK  0
#define RESULT_ERR 1

#define Result(E, V) struct { \
        int type;             \
        union {               \
           E reason;             \
           V value;\
        } payload;\
}

#define result_new_err(res) { \
    .type = RESULT_ERR,          \
    .payload = {                 \
        .reason = res         \
    }\
}

#define result_new_ok(v) { \
    .type = RESULT_OK,         \
    .payload = {               \
        .value = v       \
    }\
}

#define result_is_err(result) (result.type == RESULT_ERR)
#define result_is_ok(result) (result.type == RESULT_OK)

#define result_unwrap(result) (result_is_ok(result) ? result.payload.value : result.payload.reason)

#define result_turn_err(result, res) { \
    result.type = RESULT_ERR;          \
    result.payload.reason = res;\
}

#define result_turn_ok(result, v) { \
    result.type = RESULT_OK;          \
    result.payload.value = v;\
}

#endif //SERVER_RESULT_H

//
// Created by cmtheit on 23-6-1.
//

#ifndef SERVER_CMD_H
#define SERVER_CMD_H
// 此文件定义所有可用的主控端命令

#include <hv/hloop.h>
#include <lib/result.h>
#define CMD_NUM 4
#define CMD_HELP_ID 0
#define CMD_QUIT_ID 1
#define CMD_LIST_ID 2
#define CMD_undefined_ID 3

typedef struct Cmd {
    hevent_t ev;
    const char * name;
    const char * description;
} Cmd;

extern Cmd cmds[CMD_NUM];

enum CmdInitErrCode {
       CmdInitErrOther,
};

typedef Result(enum CmdInitErrCode, int) CmdInitResult;

CmdInitResult cmd_init();
void cmd_exe(const char * name, int argc, char ** argv);

#endif //SERVER_CMD_H

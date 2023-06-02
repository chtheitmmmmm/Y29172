#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int is_internal_ip(const char* ip) {
    struct in_addr addr;
    inet_pton(AF_INET, ip, &addr);

    unsigned int network = ntohl(addr.s_addr) >> 24;
    return (network == 10) || (network == 172 && (addr.s_addr >> 20 & 0xF) == 16) || (network == 192 && (addr.s_addr >> 16 & 0xFF) == 168);
}

int main() {
    struct ifaddrs *ifaddr, *ifa;

    // 获取所有接口地址信息
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(1);
    }

    // 遍历接口地址信息并找到匹配的内网IPv4地址
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr != NULL && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in* sa = (struct sockaddr_in*)ifa->ifa_addr;
            char* ip = inet_ntoa(sa->sin_addr);

            if (is_internal_ip(ip)) {
                printf("%s", ip);
                break;
            }
        }
    }

    freeifaddrs(ifaddr);
    return 0;
}

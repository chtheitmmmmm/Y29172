#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int is_internal_ip(const char* ip) {
    struct in_addr addr;
    inet_pton(AF_INET, ip, &addr);
    unsigned int network = ntohl(addr.s_addr) >> 16;
    return (network == 0xC0A8);
}

int main() {
    struct ifaddrs *ifaddr, *ifa;

    // Retrieve all interface address information
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(1);
    }

    // Traverse the interface address information and find matching internal IPv4 addresses
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

//
// Created by cmtheit on 23-6-2.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>

int main() {
    struct if_nameindex *if_nidxs, *intf;
    struct ifreq ifr;
    int fd;

    // 获取所有接口名称索引
    if_nidxs = if_nameindex();
    if (if_nidxs == NULL) {
        perror("if_nameindex");
        exit(1);
    }

    // 遍历接口名称索引并打印设备名称
    for (intf = if_nidxs; intf->if_index != 0 || intf->if_name != NULL; intf++) {
        printf("Device name: %s\n", intf->if_name);
    }

    if_freenameindex(if_nidxs);
    return 0;
}

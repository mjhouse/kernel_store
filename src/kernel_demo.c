#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
extern int errno;

#include "ks_common.h"

void log_exit(char* msg){
    printf("msg: %snum: %d\n",msg,errno);
    exit(1);
}

int main(){
    node n = { .key = "KEY", .val = "VALUE" };
    node p = { .key = "KEY", .val = "NOVAL" };

    int fd = open("/dev/ks", O_RDWR);
    if (fd == -1) log_exit("couldn't open device");

    printf("trying to set: %s / %s\n",n.key,n.val);
    if (ioctl(fd, KS_SET_VALUE, &n) < 0) {
        log_exit("couldn't set to device\n");
    }

    printf("trying to get: %s / %s\n",p.key,p.val);
    if (ioctl(fd, KS_GET_VALUE, &p) < 0) {
        log_exit("couldn't get from device\n");
    }

    printf("returned: %s/%s\n",p.key,p.val);
    return 0;
}

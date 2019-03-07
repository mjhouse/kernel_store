#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
extern int errno;

#include "ks_user.h"

void log_exit(char* msg){
    printf("msg: %s, num: %d\n",msg,errno);
    exit(1);
}

int main(){
    node* n = make_node("KEY","VALUE");

    int fd = open("/dev/ks", O_RDWR);
    if (fd == -1) log_exit("couldn't open device");

    printf("fd: %d\n",fd);

    if (ioctl(fd, KS_SET_VALUE, n) < 0) {
        log_exit("couldn't input to device");
    }

    return 0;
}

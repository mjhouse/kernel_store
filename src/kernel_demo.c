#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
extern int errno;

#include "ks_common.h"

// fail with an error message
void log_exit(char* msg){
    printf("msg: %snum: %d\n",msg,errno);
    exit(1);
}

// open the kernel_store module or fail with a message
int get_device(){
    int fd = open("/dev/ks", O_RDWR);
    if (fd == -1) log_exit("couldn't open device");
    return fd;
}

int main(){
    node n = { .key = "KEY", .val = "VALUE" }; // test node for set
    node p = { .key = "KEY", .val = "NOVAL" }; // test node for get

    int fd = get_device();

    // set the value in kernel_store or exit
    printf("trying to set: %s / %s\n",n.key,n.val);
    if (ioctl(fd, KS_SET_VALUE, &n) < 0) {
        log_exit("couldn't set to device\n");
    }

    // get the value from the kernel_store or exit
    printf("trying to get: %s / %s\n",p.key,p.val);
    if (ioctl(fd, KS_GET_VALUE, &p) < 0) {
        log_exit("couldn't get from device\n");
    }

    printf("returned: %s/%s\n",p.key,p.val);
    return 0;
}

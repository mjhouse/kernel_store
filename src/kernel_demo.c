#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>

#include "ks_common.h"

int main(void)
{
        int fd;
        node before = { .key = "KEY", .val = "VALUE" };
        node after = { .key = {0}, .val = {0} };

        printf("\nopening driver:");
        fd = open("/dev/ks", O_RDWR);

        if(fd < 0) {
            printf(" cannot open device\n");
            return 0;
        }
        else {
            printf(" open\n");
        }

        printf("\twriting value to key: %s/%s\n",before.key,before.val);
        ioctl(fd, KS_SET_VALUE, (node*) &before);

        ioctl(fd, KS_GET_VALUE, (node*) &after);
        printf("\treading value from key: %s/%s\n",after.key,after.val);

        printf("closing driver\n");
        close(fd);
}

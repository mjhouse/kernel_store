COMMON  = -Wextra -Wall -Wconversion -Wshadow
EXTRA_CFLAGS=-I$(PWD)/inc

obj-m += src/kernel_store.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) $(INC) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	-sudo rmmod src/kernel_store.ko
	sudo dmesg -C
	sudo insmod src/kernel_store.ko
	dmesg

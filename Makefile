COMMON  = -Wextra -Wall -Wconversion -Wshadow
EXTRA_CFLAGS=-I$(PWD)/inc

obj-m += src/kernel_store.o

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) $(INC) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

demo:
	gcc $(EXTRA_CFLAGS) -Wstrict-prototypes src/kernel_demo.c -o src/demo 

test:
	-sudo rmmod src/kernel_store.ko
	sudo dmesg -C
	sudo insmod src/kernel_store.ko
	dmesg

	# sudo mknod /dev/kernel_store c <MAJOR> 0 && sudo chmod 766 /dev/kernel_store
	# sudo rm /dev/kernel_store && sudo rmmod kernel_store

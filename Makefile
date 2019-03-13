COMMON  = -Wextra -Wall -Wconversion -Wshadow
EXTRA_CFLAGS=-I$(PWD)/inc

obj-m += src/kernel_store.o

# I'm not an expert with kbuild, so I wasted a lot of time trying to
# figure out how to generate intermediate files in a 'obj' directory
# and put the *.ko and bin files (demo) in a 'bin' directory before
# I gave up. Ideally this wouldn't be an in-source build.

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

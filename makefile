# https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234

# https://www.tldp.org/LDP/lkmpg/2.6/html/x181.html

#obj-m += hello-1.o
#
#all:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
#
#clean:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

CC=gcc

COMMON  = -Wextra - Wall -Wconversion -Wshadow

TARGET = bin/commonstore.ko
OUTDIR = bin
BLDDIR = obj
INCDIR = inc
SRCDIR = src

# create directories
$(shell mkdir -p obj bin )

# find all source files in srcdir
SOURCES  := $(shell find $(SRCDIR)/ -type f -name '*.c')

# assembles each source file into a BLDIR/*.o filename
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(BLDDIR)/%.o)

INC = -I$(INCDIR)

# ----------------------------------------------------------------------
# DON'T EDIT BELOW THIS LINE
# ----------------------------------------------------------------------

debug: CFLAGS = ${COMMON} -Wall -Wextra -Wpedantic -g -ggdb
debug: link-debug

release: CFLAGS = ${COMMON} -DNDEBUG -O3
release: link-release

# LINK
link-debug: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(CFLAGS)

link-release: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(CFLAGS)

# BUILD
obj/%.o: $(SRCDIR)/%.c
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

clean:
	find $(BLDDIR)/ -type f -exec rm -f {} +;
	find $(OUTDIR)/ -type f -exec rm -f {} +;

# WARN    := -W -Wall -Wstrict-prototypes -Wmissing-prototypes
# INCLUDE := -isystem /lib/modules/`uname -r`/build/include
# CFLAGS  := -O3 -DMODULE -D__KERNEL__ ${WARN} ${INCLUDE}
# CC      := gcc
# OBJS    := ${patsubst %.c, %.o, ${wildcard *.c}}
# all: ${OBJS}
#
# .PHONY: clean
# clean:
# 	rm -rf *.o

obj-m += phantom-file-system.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	sudo dmesg -C
	sudo insmod phantom-file-system.ko
	sudo rmmod phantom-file-system.ko
	dmesg

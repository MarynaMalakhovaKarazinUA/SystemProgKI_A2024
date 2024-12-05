obj-m += char_device.o

KDIR ?= /home/user1/buildroot/output/build/linux-6.6.32
PWD := $(shell pwd)

ARCH := x86_64
CROSS_COMPILE := x86_64-buildroot-linux-gnu-
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/home/user1/buildroot/output/host/bin

all:
	make -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	make -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
obj-m += mm.o

KDIR ?= /home/user1/buildroot/output/build/linux-6.6.32
PWD := $(shell pwd)

ARCH=x86_64
CROSS_COMPILE='x86_64-buildroot-linux-gnu-'
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/home/user1/buildroot/output/host/bin

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
obj-m	:= samsung-backlight.o

INST_DIR  ?= /lib/modules/$(shell uname -r)/extra/samsung-backlight/
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers

install:
	install -D -m 644 -p samsung-backlight.ko $(INST_DIR)/samsung-backlight.ko

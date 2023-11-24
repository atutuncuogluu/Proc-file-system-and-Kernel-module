obj-m += my_module.o # you can change your module name this should be like  my_module.c

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
        $(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
        $(MAKE) -C $(KERNELDIR) M=$(PWD) clean
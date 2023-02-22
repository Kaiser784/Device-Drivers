obj-m := module1.o
obj-m += module2.o

KDIR = /lib/modules/$(shell uname -r)/build 
all:
	make -C $(KDIR)  M=$(shell pwd) modules
 
clean:
	make -C $(KDIR)  M=$(shell pwd) clean

.PHONY: all
all: ioctl_eject.out myioctl_app.out myioctl_driver

# Lab 1 #################################################################
ioctl_eject.out: ioctl_eject.c
	gcc -o ioctl_eject.out ioctl_eject.c

# Lab 2 ################################################################
myioctl_app.out: myioctl_app.c myioctl_driver.h
	gcc -o myioctl_app.out myioctl_app.c

obj-m += myioctl_driver.o
myioctl_driver: myioctl_driver.c myioctl_driver.h
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# clean ###############################################################
.PHONY: clean
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm *.out
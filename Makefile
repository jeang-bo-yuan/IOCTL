.PHONY: all
all: ioctl_eject myioctl_app myioctl_app_reset myioctl_driver.ko

# Lab 1 #################################################################
ioctl_eject: ioctl_eject.c
	gcc -o ioctl_eject ioctl_eject.c

# Lab 2 ################################################################
myioctl_app: myioctl_app.c myioctl_driver.h
	gcc -o myioctl_app myioctl_app.c

myioctl_app_reset: myioctl_app_reset.c myioctl_driver.h
	gcc -o myioctl_app_reset myioctl_app_reset.c

obj-m += myioctl_driver.o
myioctl_driver.ko: myioctl_driver.c myioctl_driver.h
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# clean ###############################################################
.PHONY: clean
clean: unload
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm ioctl_eject myioctl_app myioctl_app_reset

# load_myioctl ########################################################
.PHONY: load
load: myioctl_driver.ko unload
	sudo insmod myioctl_driver.ko;
	export MAJOR=`sudo dmesg | tail | awk '/myioctl module loaded./{print $$8}' | tail -n 1`; \
	echo Major Number is $${MAJOR}; \
	sudo mknod /dev/myioctl c  $${MAJOR} 0;
	@sudo chmod 666 /dev/myioctl;

.PHONY: unload
unload:
	-sudo rmmod myioctl_driver.ko
	-sudo rm /dev/myioctl
	@echo Unload myioctl ............................................................................ Done
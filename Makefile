.PHONY: all
all: ioctl_eject

ioctl_eject: ioctl_eject.c
	gcc -o ioctl_eject.out ioctl_eject.c

.PHONY: all
clean:
	rm *.out
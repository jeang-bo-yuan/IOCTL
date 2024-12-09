# IOCTL
鄭欣明 作業系統 Homework 5

# Lab1: Eject CD-ROM

Source code: ioctl_eject.c

Compile and Run
```sh
make
./ioctl_eject.out
```

# Lab2: Linux Kernel Driver

Dependency: `sudo apt install gcc-12`

```sh
# compile
make

# Load kernel module
sudo insmod myioctl_driver.ko

# Remember the major number assigned to myioctl module
sudo dmesg | tail
# Create character device
# Replace <major-number> with the major number shown above
sudo mknod /dev/myioctl c <major-number> 0

sudo chmod 666 /dev/myioctl
```

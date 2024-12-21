#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "myioctl_driver.h"

int main() {
    int fd = open("/dev/myioctl", O_RDWR);

    if (fd == -1) {
        perror("Cannot open /dev/myioctl");
        return EXIT_FAILURE;
    }

    int counter_value;
    ioctl(fd, MYIOCTL_GET_COUNT, &counter_value);
    printf("Current counter value: %d\n", counter_value);

    if (ioctl(fd, MYIOCTL_RESET) == -1) {
        perror("Failed to reset counter");
        close(fd);
        return EXIT_FAILURE;
    }

    ioctl(fd, MYIOCTL_GET_COUNT, &counter_value);
    printf("Current counter value: %d\n", counter_value);

    close(fd);
    return EXIT_SUCCESS;
}
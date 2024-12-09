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

    int increment_value;
    printf("Enter the value to increment the counter: ");
    if (scanf("%d", &increment_value) != 1) {
        perror("Error reading input");
        close(fd);
        return EXIT_FAILURE;
    }

    ioctl(fd, MYIOCTL_INCREMENT, &increment_value);

    ioctl(fd, MYIOCTL_GET_COUNT, &counter_value);
    printf("Updated counter value: %d\n", counter_value);

    close(fd);
    return EXIT_SUCCESS;
}
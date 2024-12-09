#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>  // for close
#include <linux/cdrom.h>

int main() {
    // open CD-ROM
    int cdrom_fd = open("/dev/cdrom", O_RDONLY);
    if (cdrom_fd == -1) {
        perror("Error opening CD-ROM device");
        exit(EXIT_FAILURE);
    }

    // unlock door
    if (ioctl(cdrom_fd, CDROM_LOCKDOOR, 0) == -1) {
        perror("Error unlocking CD-ROM door");
        exit(EXIT_FAILURE);
    }

    // eject CD-ROM
    if (ioctl(cdrom_fd, CDROMEJECT, 0) == -1) {
        perror("Error ejecting CD-ROM");
        exit(EXIT_FAILURE);
    }

    printf("CD-ROM is ejected successfully\n");
    close(cdrom_fd);
    return EXIT_SUCCESS;
}

#include "myioctl_driver.h" // for some constants
#include <linux/module.h>
#include <linux/fs.h>

// only accessible in kernel space
// https://stackoverflow.com/questions/77714580/broken-linux-kernel-linux-includes-on-ubuntu-20-04
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NTUST");
MODULE_DESCRIPTION("Kernel Space Driver");

static int myioctl_major;
static int count = 0;

// prototypes
static int myioctl_open(struct inode *inode, struct file *filp);
static int myioctl_release(struct inode *inode, struct file *filp);
static long myioctl_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

// file operations structure
static const struct file_operations myioctl_fops = {
    .open = myioctl_open,
    .release = myioctl_release,
    .unlocked_ioctl = myioctl_ioctl
};

static int __init myioctl_init(void) {
    // register character device
    myioctl_major = register_chrdev(0, "myioctl", &myioctl_fops);

    if (myioctl_major < 0) {
        pr_err("Failed to register character device\n");
        return myioctl_major;
    }

    pr_info("myioctl module loaded. Major number: %d\n", myioctl_major);
    return 0;
}

static void __exit myioctl_exit(void) {
    unregister_chrdev(myioctl_major, "myioctl");
    pr_info("myioctl module unloaded\n");
}

static int myioctl_open(struct inode *inode, struct file *filp) {
    pr_info("myioctl device opened\n");
    return 0;
}

static int myioctl_release(struct inode *inode, struct file *filp) {
    pr_info("myioctl device closed");
    return 0;
}

static long myioctl_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    int err = 0;
    int tmp;

    if (_IOC_TYPE(cmd) != MYIOCTL_MAGIC) {
        pr_err("Invalid magic number\n");
        return -ENOTTY;
    }

    switch (cmd)
    {
    case MYIOCTL_RESET:
        pr_info("MYIOCTL: Resetting counter\n");
        break;
    
    case MYIOCTL_GET_COUNT:
        pr_info("MYIOCTL: Getting counter value\n");
        err = copy_to_user((int*)arg, &count, sizeof(int));
        break;

    case MYIOCTL_INCREMENT:
        pr_info("MYIOCTL: Incrementing counter\n");
        err = copy_from_user(&tmp, (int*)arg, sizeof(int));
        if (err == 0)
            count += tmp;
        break;

    default:
        pr_err("Unknown IOCTL command\n");
        return -ENOTTY;
    }

    return err;
}

module_init(myioctl_init);
module_exit(myioctl_exit);
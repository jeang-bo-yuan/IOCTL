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

static int read_persistent_counter(void) {
    struct file *file;
    char buf[16];
    int value = 0;

    file = filp_open(PERSIST_FILE, O_RDONLY, 0);
    if (IS_ERR(file)) {
        pr_info("Failed to open persist file. Initializing counter to 0.\n");
        return 0; 
    }

    memset(buf, 0, sizeof(buf));
    if (kernel_read(file, buf, sizeof(buf) - 1, 0) > 0) {
        if (kstrtoint(buf, 10, &value) != 0) {
            pr_err("Failed to convert string to integer\n");
            value = 0;
        }
    }

    filp_close(file, NULL);
    return value;
}

static void write_persistent_counter(int value) {
    struct file *file;
    char buf[16];
    int len;

    snprintf(buf, sizeof(buf), "%d", value);

    file = filp_open(PERSIST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (IS_ERR(file)) {
        pr_err("Failed to open persist file for writing.\n");
        return;
    }

    len = strlen(buf);
    if (kernel_write(file, buf, len, 0) < 0) {
        pr_err("Failed to write to persist file.\n");
    }

    filp_close(file, NULL);
}

// file operations structure
static const struct file_operations myioctl_fops = {
    .open = myioctl_open,
    .release = myioctl_release,
    .unlocked_ioctl = myioctl_ioctl
};

static int __init myioctl_init(void) {

    count = read_persistent_counter();
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
    write_persistent_counter(count);
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
        count = 0;
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
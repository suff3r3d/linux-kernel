#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "challenge"
#define CLASS_NAME "challenge"

MODULE_AUTHOR("ir0nstone");
MODULE_DESCRIPTION("Interactive Drivers");
MODULE_LICENSE("GPL");

// setting up the device
int major;
static struct class*  my_class  = NULL;
static struct device* my_device = NULL;

static ssize_t challenge_read(struct file *filp, char __user *buffer, size_t len, loff_t *off) {
    printk(KERN_ALERT "reading...");

    copy_to_user(buffer, "QWERTY", 6);

    return 0;
}

static ssize_t challenge_write(struct file *flip, char __user *buffer, size_t len, loff_t *off) {
    return 0;
}

static struct file_operations fops = {
    .read = challenge_read
    .write = challenge_write
};

static int __init challenge_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if ( major < 0 )
        printk(KERN_ALERT "[Intro] Error assigning Major Number!");
    
    // Register device class
    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[Intro] Failed to register device class\n");
    }

    // Register the device driver
    my_device = device_create(my_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[Intro] Failed to create the device\n");
    }

    return 0;
}

static void __exit challenge_exit(void) {
    device_destroy(my_class, MKDEV(major, 0));              // remove the device
    class_unregister(my_class);                             // unregister the device class
    class_destroy(my_class);                                // remove the device class
    unregister_chrdev(major, DEVICE_NAME);                  // unregister the major number
    printk(KERN_INFO "[Intro] Closing!\n");
}

module_init(challenge_init);
module_exit(challenge_exit);

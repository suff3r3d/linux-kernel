#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

// #include <mm/slab.h>
#include <linux/slab.h>
#include <linux/gfp_types.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>

#define DEVICE_NAME "challenge"
#define CLASS_NAME "challenge"

MODULE_AUTHOR("suff3r3d");
MODULE_DESCRIPTION("test heap overflow");
MODULE_LICENSE("GPL");

// setting up the device
int major;
static struct class*  my_class  = NULL;
static struct device* my_device = NULL;

static struct kmem_cache * my_cache = 0;

#define BUF_LEN 0x100

char * my_buf;
DEFINE_MUTEX(var_mutex);

static ssize_t ioctl_handler(struct file *file, unsigned int cmd, unsigned long arg) {
    printk("[challenge] Command: %u; Argument: %u", cmd, arg);

    void __user * argp = (void __user *)arg;

    mutex_lock(&var_mutex);

    if (cmd == 0x13370000) { // ALLOC
        long long unsigned int len;
        void __user *addr;

        copy_from_user(&addr, arg, 8);
        copy_from_user(&len, arg+8, 8);

        void * mem = kmalloc(len, GFP_KERNEL);
        printk("[challenge] mem = %llu", mem);
        my_buf = mem;
        if (put_user(my_buf, (char __user * )addr)) {
            mutex_unlock(&var_mutex);
            return -EFAULT;
        }
    } else if (cmd == 0x13370001) { // FREE
        if (my_buf) {
            kfree(my_buf);
            my_buf = 0;
        }
    }
    else if (cmd == 0x13370002) { // READ
        long long unsigned int len;
        void __user *addr;

        copy_from_user(&addr, arg, 8);
        copy_from_user(&len, arg+8, 8);

        printk("[challenge] READ addr %x len %x", addr, len);

        if (len > BUF_LEN) {
            mutex_unlock(&var_mutex);
            return -EFAULT;
        }

        copy_to_user(addr, my_buf, len);
    } else if (cmd == 0x13370003) { // WRITE
        long long unsigned int addr, len;

        copy_from_user(&addr, arg, 8);
        copy_from_user(&len, arg+8, 8);

        if (len > BUF_LEN) {
            mutex_unlock(&var_mutex);
            return -EFAULT;
        }

        printk("[challenge] WRITE addr %x len %x", addr, len);

        copy_from_user(my_buf, addr, len);
    } else {
        mutex_unlock(&var_mutex);
        return -2;
    }

    mutex_unlock(&var_mutex);
    return 0;
}

static struct file_operations fops = {
    .compat_ioctl = ioctl_handler,
    .unlocked_ioctl = ioctl_handler
};

static int __init challenge_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if ( major < 0 )
        printk(KERN_ALERT "[Challenge] Error assigning Major Number!");
    
    // Register device class
    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[Challenge] Failed to register device class\n");
    }

    // Register the device driver
    my_device = device_create(my_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "[Challenge] Failed to create the device\n");
    }

    my_cache = kmem_cache_create("challenge", BUF_LEN, 0, SLAB_HWCACHE_ALIGN, NULL);

    return 0;
}

static void __exit challenge_exit(void) {
    device_destroy(my_class, MKDEV(major, 0));              // remove the device
    class_unregister(my_class);                             // unregister the device class
    class_destroy(my_class);                                // remove the device class
    unregister_chrdev(major, DEVICE_NAME);                  // unregister the major number

    if (my_buf) {
        kmem_cache_free(my_cache, my_buf);
    }
    kmem_cache_destroy(my_cache);

    printk(KERN_INFO "[Challenge] Closing!\n");
}

module_init(challenge_init);
module_exit(challenge_exit);

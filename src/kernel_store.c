
// https://www.xml.com/ldd/chapter/book/ch05.html

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#include "kernel_store.h"

#define DEVICE_NAME "kernel_store"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michael House");
MODULE_DESCRIPTION("A common data store for user-space applications.");
MODULE_VERSION("0.01");

/* Prototypes for device functions */
static int device_open(struct inode *i, struct file *f);
static int device_release(struct inode *i, struct file *f);
static ssize_t device_read(struct file *filp, char *buffer, size_t len, loff_t *offset);
static ssize_t device_write(struct file *filp, const char *buffer, size_t len, loff_t *offset);

static int major_num;

/* FOR TESTING */
struct kern_store store = {
    "KEY",
    "VALUE",
    3, 5
};
/* END TESTING */

/* Device function mapping for this module */
struct file_operations fops = {
    .owner   = THIS_MODULE,
    .read    = device_read,
    .write   = device_write,
    .open    = device_open,
    .release = device_release
};

/* initialize any data for a particular device access */
static int device_open(struct inode *i, struct file *f){
    printk("in device_open");
    return 0;
};

/* release any data for a particular device access */
static int device_release(struct inode *i, struct file *f){
    printk("in device_release");
    return 0;
};

/* read from the keystore */
static ssize_t device_read(struct file *filp, char *buffer, size_t len, loff_t *offset){
    int read_bytes = 0;

    printk("in device_read");

    return read_bytes;
};

/* write to the keystore */
static ssize_t device_write(struct file *filp, const char *buffer, size_t len, loff_t *offset){
    int written_bytes = 0;

    printk("in device_write");

    return written_bytes;
};

static int __init kernel_store_init(void) {
    major_num = register_chrdev(0, DEVICE_NAME, &fops);

    if (major_num < 0) {
        printk(KERN_ALERT "could not register device: %d\n", major_num);
        return major_num;
    } else {
        printk(KERN_INFO "kernel_store module loaded with device major number %d\n", major_num);
    }

    return 0;
}

static void __exit kernel_store_exit(void) {
    unregister_chrdev(major_num, DEVICE_NAME);
    printk("kernel_store unloaded\n");
}

module_init(kernel_store_init);
module_exit(kernel_store_exit);

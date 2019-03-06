
// https://www.xml.com/ldd/chapter/book/ch05.html

// https://www.oreilly.com/library/view/linux-device-drivers/0596005903/ch06.html

// https://lwn.net/Articles/48354/

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

static int major_num  = 0;
static int lock_count = 0;
static char* key_buf  = 0;

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
    if (lock_count) {
        return -EBUSY;
    }
    lock_count++;
    return 0;
};

/* release any data for a particular device access */
static int device_release(struct inode *i, struct file *f){
    printk("in device_release");
    lock_count--;
    return 0;
};

/* read from the keystore */
static ssize_t device_read(struct file *filp, char *buffer, size_t len, loff_t *offset){
    int read_bytes = 0;
    char* val = ks_get(key_buf);

    printk("in device_read");

    // copy_to_user(val,buffer);

    return read_bytes;
};

/* write to the keystore */
static ssize_t device_write(struct file *filp, const char *buffer, size_t len, loff_t *offset){
    int written_bytes = 0;
    int size = (len(buffer) + 1) * sizeof(char); // fix len()

    printk("in device_write");

    // use STRLEN() over LEN()

    // DUMMY CODE -------------------------------------------------
    // // krealloc static buffer
    // key_buf = (char*)krealloc(key_buf, size, GFP_KERNEL);

    // // copy the input string to static buffer
    // strncpy(buffer,key_buf,size); // check if strcpy is available

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

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>

#include "ks_common.h"

MODULE_LICENSE("GPL");
<<<<<<< HEAD
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
=======
MODULE_AUTHOR("Michael House <mjhouse@protonmail.com>");
MODULE_DESCRIPTION("A key/value store");
MODULE_VERSION("1.0");

dev_t dev = 0;
static struct class *dev_class;
static struct cdev ks_cdev;

node stash = { .key = {0}, .val = {0} };

static int __init ks_driver_init(void);
static void __exit ks_driver_exit(void);
>>>>>>> 4982fb82c21f27268a5b3759ddf429e832315d8c

static int ks_open(struct inode *inode, struct file *file);
static int ks_release(struct inode *inode, struct file *file);

<<<<<<< HEAD
    // copy_to_user(val,buffer);

    return read_bytes;
};

/* write to the keystore */
static ssize_t device_write(struct file *filp, const char *buffer, size_t len, loff_t *offset){
    int written_bytes = 0;
    int size = (len(buffer) + 1) * sizeof(char); // fix len()
=======
static long ks_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .open           = ks_open,
        .release        = ks_release,
        .unlocked_ioctl = ks_ioctl,
};

static int ks_open(struct inode *inode, struct file *file) {
        printk(KERN_INFO "kernel store open\n");
        return 0;
}

static int ks_release(struct inode *inode, struct file *file) {
        printk(KERN_INFO "kernel store close\n");
        return 0;
}
>>>>>>> 4982fb82c21f27268a5b3759ddf429e832315d8c

static long ks_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    size_t kl = strlen( ((node*) arg)->key );
    size_t vl = strlen( ((node*) arg)->val );

<<<<<<< HEAD
    // use STRLEN() over LEN()

    // DUMMY CODE -------------------------------------------------
    // // krealloc static buffer
    // key_buf = (char*)krealloc(key_buf, size, GFP_KERNEL);

    // // copy the input string to static buffer
    // strncpy(buffer,key_buf,size); // check if strcpy is available

    return written_bytes;
};
=======
    switch(cmd) {
        case KS_SET_VALUE:
            printk(KERN_INFO "\tFOR KS_SET_VALUE");
            printk(KERN_INFO "\t\tstart stash: {key=%s,val=%s}\n", stash.key,stash.val);

            copy_from_user(&stash ,(node*) arg, sizeof(struct _node));

            printk(KERN_INFO "\t\tend stash:   {key=%s,val=%s}\n", stash.key,stash.val);
            printk(KERN_INFO "\tEND KS_SET_VALUE");
            break;
        case KS_GET_VALUE:
            printk(KERN_INFO "\tFOR KS_GET_VALUE");
            printk(KERN_INFO "\t\tstart stash: {key=%s,val=%s}\n", stash.key,stash.val);

            if(strcmp(((node*) arg)->key,stash.key) == 0){
                copy_to_user((node*) arg, &stash, sizeof(struct _node));
            }

            printk(KERN_INFO "\t\tend stash:   {key=%s,val=%s}\n", stash.key,stash.val);
            printk(KERN_INFO "\tEND KS_GET_VALUE");
            break;
    }
    return 0;
}
>>>>>>> 4982fb82c21f27268a5b3759ddf429e832315d8c


static int __init ks_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "kernel_store")) <0){
                printk(KERN_INFO "cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "major = %d, minor = %d \n",MAJOR(dev), MINOR(dev));

        cdev_init(&ks_cdev,&fops);
        if((cdev_add(&ks_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }

        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"ks_class")) == NULL){
            printk(KERN_INFO "cannot create the struct class\n");
            goto r_class;
        }

        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"ks_device")) == NULL){
            printk(KERN_INFO "cannot create the device\n");
            goto r_device;
        }
        printk(KERN_INFO "device driver inserted\n");
    return 0;

r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

void __exit ks_driver_exit(void) {
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&ks_cdev);
    unregister_chrdev_region(dev, 1);

    printk(KERN_INFO "device driver removed\n");
}

module_init(ks_driver_init);
module_exit(ks_driver_exit);

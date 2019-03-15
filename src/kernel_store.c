#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>

#include "ks_common.h"
#include "ks_tree.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michael House <mjhouse@protonmail.com>");
MODULE_DESCRIPTION("A key/value store");
MODULE_VERSION("1.0");

dev_t dev = 0;
static struct class *dev_class;
static struct cdev ks_cdev;

static int __init ks_driver_init(void);
static void __exit ks_driver_exit(void);

static int ks_open(struct inode *inode, struct file *file);
static int ks_release(struct inode *inode, struct file *file);

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

static long ks_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    node* n = 0;

    switch(cmd) {
        case KS_SET_VALUE:
            printk(KERN_INFO "\tFOR KS_SET_VALUE");

            n = ks_make();
            copy_from_user(n ,(node*) arg, sizeof(struct _node));

            ks_add(n);

            printk(KERN_INFO "\t\tval added:   {key=%s,val=%s}\n", n->key,n->val);
            printk(KERN_INFO "\tEND KS_SET_VALUE");
            break;
        case KS_GET_VALUE:
            printk(KERN_INFO "\tFOR KS_GET_VALUE");

            n = ks_get(((node*) arg)->key);
            copy_to_user((node*) arg, n, sizeof(struct _node));

            printk(KERN_INFO "\t\tval returned:   {key=%s,val=%s}\n", n->key,n->val);
            printk(KERN_INFO "\tEND KS_GET_VALUE");
            break;
    }
    return 0;
}


static int __init ks_driver_init(void) {
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

        if((dev_class = class_create(THIS_MODULE,"ks_class")) == NULL){
            printk(KERN_INFO "cannot create the struct class\n");
            goto r_class;
        }

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

    ks_tree_clr();

    printk(KERN_INFO "device driver removed\n");
}

module_init(ks_driver_init);
module_exit(ks_driver_exit);

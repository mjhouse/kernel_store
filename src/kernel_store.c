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

#define DEVICE_NAME "kernel_store"
static int use_count = 0;
static int major_num;

static int __init ks_driver_init(void);
static void __exit ks_driver_exit(void);

static int ks_open(struct inode *inode, struct file *file);
static int ks_release(struct inode *inode, struct file *file);

static long ks_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops = {
        .open           = ks_open,
        .release        = ks_release,
        .unlocked_ioctl = ks_ioctl,
};

static int ks_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "kernel store open\n");

    if (use_count) {
        return -EBUSY;
    }

    use_count++;
    try_module_get(THIS_MODULE);

    return 0;
}

static int ks_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "kernel store close\n");

    use_count--;
    module_put(THIS_MODULE);

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

    printk(KERN_INFO "use_count = %d",use_count);

    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0) {
        printk(KERN_ALERT "could not register device: %d\n", major_num);
        return major_num;
    }
    else {
        printk(KERN_INFO "major = %d\n", major_num);
        return 0;
    }
}

void __exit ks_driver_exit(void) {
    ks_tree_clr();
    unregister_chrdev(major_num,DEVICE_NAME);

    printk(KERN_INFO "device driver removed\n");
}

module_init(ks_driver_init);
module_exit(ks_driver_exit);

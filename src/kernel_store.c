#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <asm/ioctl.h>

#include "ks_common.h"
#define DEVICE_NAME "kernel_store"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michael House");
MODULE_DESCRIPTION("A common data store for user-space applications.");
MODULE_VERSION("0.01");

// Prototype for the ioctl function
static long device_ioctl(struct file *filp, unsigned int cmd, unsigned long param);

static int major_num;
static node stash = { .key = 0, .val = 0 };

/* Device function mapping for this module */
struct file_operations fops = {
    .owner   = THIS_MODULE,
    .unlocked_ioctl = device_ioctl
};

/*  IOCTL
    Accepts two command flags-
        KS_GET_VALUE: should write out the value if the correct key is given
        KS_SET_VALUE: allows the caller the set the value and key
*/
static long device_ioctl(struct file *filp, unsigned int cmd, unsigned long param) {
    node* n = (node*)param;

    size_t sk = 0;
    size_t sv = 0;
    unsigned long err = 0;

    if(!n) return -EINVAL;

    switch(cmd){
        case KS_GET_VALUE:
            printk("KS_GET_VALUE");

            sv = strlen(stash.val) + 1;
            printk("\tsv: %lu",sv);

            if( access_ok(VERIFY_WRITE,n->val,sv) &&
                strcmp(n->key,stash.key) == 0){

                err = copy_to_user( n->val, stash.val, sv);
                printk("\tval chars not copied: %lu",err);
            }

            break;
        case KS_SET_VALUE:
            printk("KS_SET_VALUE");

            sk = strlen(n->key) + 1;
            sv = strlen(n->val) + 1;

            if(stash.key) kfree(stash.key);
            if(stash.val) kfree(stash.val);

            stash.key = (char*)kmalloc(sk,GFP_KERNEL);
            stash.val = (char*)kmalloc(sv,GFP_KERNEL);

            if( copy_from_user( stash.key, n->key, sk) ||
                copy_from_user( stash.val, n->val, sv) ){
                return -EINVAL;
            }

            printk("\tstash: %s/%s",stash.key,stash.val);
            break;
        default:
            return -EINVAL;
    }

    return 0;
}

/* Init the module, get a major number
*/
static int __init kernel_store_init(void) {
    major_num = register_chrdev(0, DEVICE_NAME, &fops);

    if (major_num < 0) {
        printk(KERN_ALERT "Couldn't register device with major number: %d\n", major_num);
        return major_num;
    } else {
        printk(KERN_INFO "Module loaded with device major number %d\n", major_num);
    }

    return 0;
}

static void __exit kernel_store_exit(void) {
    unregister_chrdev(major_num, DEVICE_NAME);
    printk("Module unloaded\n");
}

module_init(kernel_store_init);
module_exit(kernel_store_exit);

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michael House");
MODULE_DESCRIPTION("A common data store for user-space applications.");
MODULE_VERSION("0.01");

static int __init cstore_init(void) {
 return 0;
}

static void __exit cstore_exit(void) {
}

module_init(cstore_init);
module_exit(cstore_exit);
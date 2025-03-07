#include <linux/module.h>
#include <linux/kernel.h>

//#error Are we building this file?

static int hello_init(void){
    printk(KERN_ALERT "Hello, world\n");
    return(0);
}

static void hello_exit(void){
    printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Author Ariane francisca.silva@alu.ufc.br");
MODULE_DESCRIPTION("Test kernel module");
MODULE_VERSION("1.0");






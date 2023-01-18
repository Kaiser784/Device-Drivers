#include <linux/module.h>     /* Needed by all modules */ 
#include <linux/kernel.h>     /* Needed for KERN_INFO */ 
#include <linux/init.h>       /* Needed for the macros */ 
  
MODULE_LICENSE("MIT"); 
MODULE_AUTHOR("Paul Kadali"); 
MODULE_DESCRIPTION("A simple Hello World program!"); 
MODULE_VERSION("0.1"); 
  
static int __init hello_init(void) 
{ 
    printk(KERN_INFO "Loading Hello World module...\n"); 
    printk(KERN_INFO "Hello world\n"); 
    return 0; 
} 
  
static void __exit hello_exit(void) 
{ 
    printk(KERN_INFO "Exiting the module\n"); 
} 
  
module_init(hello_init); 
module_exit(hello_exit);
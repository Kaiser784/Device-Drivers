#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>
#include<linux/err.h>

#define mem_size 0
int32_t val = 0;

dev_t dev = 0;
struct device *dev_my;
static struct class *dev_class;
static struct cdev my_cdev;

uint8_t *kernel_buffer;

static int __init chr_driver_init(void);
static void __exit chr_driver_exit(void);

static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *filp, const char *buf, size_t len, loff_t *off);

static struct file_operations fops=
{
	.owner		=	THIS_MODULE,
	.read		= 	my_read,
	.write		=	my_write,
	.open		=	my_open,
	.release	=	my_release,
}; 

static int my_open(struct inode *inode, struct file *file)
{
	// Allocating memory
	if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL))==0)
	{
		printk(KERN_INFO"Can NOT allocate the memory to kernel ...\n");
		return -1;
	}
	printk(KERN_INFO "Device File Opened...\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
	kfree(kernel_buffer);   
	printk(KERN_INFO"Device File Closed...\n");
	return 0;
}

static ssize_t my_read(struct file *filp, char __user *buf, size_t  len, loff_t *loff)
{
	copy_to_user(buf, kernel_buffer,mem_size);
	printk(KERN_INFO "Data Read: DONE....\n");
	return mem_size;
}

static ssize_t my_write(struct file *filp, const char __user *buf, size_t len, loff_t  *loff)
{
	copy_from_user(kernel_buffer, buf, len);
	printk(KERN_INFO "Data is written Successfully...\n");
	return len;
}


static int __init chr_driver_init(void)
{
    int ret;

	// Allocating Major Number Dynamically
	ret = alloc_chrdev_region(&dev, 0, 1, "my_Dev");
	if (ret < 0) {
		printk(KERN_INFO"err_handle: Chrdev allocation failure\n");
	    goto out;
	}
	
	// Creating cdev structure
	cdev_init(&my_cdev, &fops);
	
	// Register cdev structure with VFS
	my_cdev.owner = THIS_MODULE;
	ret = cdev_add(&my_cdev, dev, 1);
	if (ret < 0) {
		printk(KERN_INFO"err_handle: Cdev add failed\n");
		goto r_class;
	}
	
	// Creating Struct Class
	dev_class = class_create(THIS_MODULE,"my_class");
	if (IS_ERR(dev_class)) {
		printk(KERN_INFO"err_handle: class creation failed\n");
		ret= PTR_ERR(dev_class);
		goto r_fail;
	}
	
	// Creating Device
	dev_my = device_create(dev_class, NULL, NULL, NULL,"my_device");
	if (IS_ERR(dev_my)) {
		printk(KERN_INFO"err_handle: Test 4: Device creation failure\n");
		ret= PTR_ERR(dev_my);
		goto r_device;
	}
	
	printk(KERN_INFO"err_handle: Device driver created successfully\n");
	return 0;
	
r_device:
	class_destroy(dev_class);
r_fail:
	cdev_del(&my_cdev);
r_class:
	unregister_chrdev_region(dev,1);
out:
    return ret;
}

void __exit chr_driver_exit(void)
{
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO"err_handle: Device driver removed successfully\n");
}

module_init(chr_driver_init);
module_exit(chr_driver_exit);

MODULE_LICENSE("GPL");

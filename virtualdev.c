
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/device.h> // class_create, class_destroy
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("Dual BSD/GPL");


// laite luokka
static struct class *dev_class;

// laite numero
static dev_t my_dev;



#define PARAM(X) #X
static char *param = PARAM(parametri);
module_param(param, charp, S_IRUGO | S_IWUSR);

static ssize_t my_read(struct file *filp, char __user *ubuff, size_t len, loff_t *offs)
{
	printk(KERN_ALERT "my_read\n");
	return 0;
}


static ssize_t my_write(struct file *filp, const char __user *ubuff, size_t len, loff_t *offs)
{
	printk(KERN_ALERT "my_write\n");
	return len;
}

static int my_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "my_open\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "my_release\n");
	return 0;
}


static struct file_operations my_fileops = {
.read = my_read,
.write = my_write,
.open = my_open,
.release = my_release
};

// laite
static struct cdev my_cdev;

static int virtualdev_init(void)
{
    printk(KERN_ALERT "virtualdev_init\n");

	// 1 class
	dev_class  = class_create(THIS_MODULE, "my-virtual-class");
	// 2 char device region
	alloc_chrdev_region(&my_dev, 0, 1, "my-virtual-region");
	// 3 initialize device
	cdev_init(&my_cdev, &my_fileops);
	my_cdev.owner = THIS_MODULE;
	// 4 add device
	cdev_add(&my_cdev, my_dev, 1);



    return 0;
}

static void virtualdev_exit(void)
{
    printk(KERN_ALERT "virtualdev_exit \n");
	class_destroy(dev_class);
	unregister_chrdev_region(my_dev, 1);
	cdev_del(&my_cdev);
}

module_init(virtualdev_init);
module_exit(virtualdev_exit);




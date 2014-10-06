#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/device.h> // class_create, class_destroy
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");


// laite luokka
static struct class *dev_class;

// laite numero
static dev_t my_dev;


#define PARAM(X) #X
static char *param = PARAM(parametri);
module_param(param, charp, S_IRUGO | S_IWUSR);



static int virtualdev_init(void)
{
    printk(KERN_ALERT "virtualdev_init\n");

	// 1 class
	dev_class  = class_create(THIS_MODULE, "my-virtual-class");
	// 2 char device region
	alloc_chrdev_region(&my_dev, 0, 1, "my-virtual-region");


    return 0;
}

static void virtualdev_exit(void)
{
    printk(KERN_ALERT "virtualdev_exit \n");
	class_destroy(dev_class);
	unregister_chrdev_region(my_dev, 1);
}

module_init(virtualdev_init);
module_exit(virtualdev_exit);




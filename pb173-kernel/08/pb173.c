#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/kfifo.h>

#define FIFO_SIZE (8*1024)
DEFINE_KFIFO(fifo, char, FIFO_SIZE);

ssize_t my_read(struct file *filp, char __user *buf,
					size_t count, loff_t *offp)
{
	size_t num;

	kfifo_to_user(&fifo, buf, count, &num);
	printk(KERN_INFO "Size: %d\n", count);
	return num;
}

ssize_t my_write(struct file *filp, const char __user *buf,
					size_t count, loff_t *offp)
{
	size_t real_count;
	size_t num, len;

	len = kfifo_len(&fifo);
	real_count = count;
	if ((len + count) > FIFO_SIZE)
		real_count = FIFO_SIZE - len;
	kfifo_from_user(&fifo, buf, real_count, &num);
	return count;
}

int my_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "OPEN\n");
	return 0;
}

int my_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "RELEASE\n");
	return 0;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
};

static struct miscdevice my_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "pb173_pok",
	.fops = &my_fops,
};

static int my_init(void)
{
	misc_register(&my_misc);
	return 0;
}

static void my_exit(void)
{
	misc_deregister(&my_misc);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

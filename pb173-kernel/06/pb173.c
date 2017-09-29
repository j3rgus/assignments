#include <linux/module.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/string.h>

#define BUFFER_SIZE (20*1024*1024)

static char *buffer;
static size_t index;

ssize_t my_read(struct file *filp, char __user *buf,
					size_t count, loff_t *offp)
{
	size_t retval;
	retval = simple_read_from_buffer(buf, count, offp,
						buffer, BUFFER_SIZE);
	return retval;
}

ssize_t my_write(struct file *filp, const char __user *buf,
					size_t count, loff_t *offp)
{
	if (!count)
		return 0;
	if (copy_from_user(&buffer[index], buf, count))
		return -EFAULT;
	index += count;
	*offp += count;
	return count;
}

int my_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "OPEN");
	return 0;
}

int my_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "RELEASE");
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
	unsigned long i;

	buffer = vmalloc(BUFFER_SIZE);
	if (!buffer)
		return -ENOMEM;
	memset(buffer, 0, BUFFER_SIZE);
	for (i = 0; i < BUFFER_SIZE; i += PAGE_SIZE)
		sprintf(&buffer[i], "%p: %lx\n", &buffer[i],
				vmalloc_to_pfn(&buffer[i]) << PAGE_SHIFT);
	misc_register(&my_misc);
	return 0;
}

static void my_exit(void)
{
	vfree(buffer);
	misc_deregister(&my_misc);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

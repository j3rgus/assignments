#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#define SET_LEN 0
#define GET_LEN 1

#define IOCTL_BASE 'a'
#define IOCTL_SET_LEN _IOW(IOCTL_BASE, SET_LEN, int)
#define IOCTL_GET_LEN _IOR(IOCTL_BASE, GET_LEN, int)

static int len = 4;

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

ssize_t my_read(struct file *filp, char __user *buf,
					size_t count, loff_t *offp)
{
	char *str = "Ahoj";

	if (count < len)
		return -EINVAL;
	if (*offp != 0)
		return 0;
	if (copy_to_user(buf, str, len))
		return -EINVAL;
	*offp = len;
	return len;
}

ssize_t my_write(struct file *filp, const char __user *buf,
					size_t count, loff_t *offp)
{
	char *str;

	str = kmalloc(50, GFP_KERNEL);
	if (copy_from_user(str, buf, (count > 50) ? 50 : count));
		return -EFAULT;
	str[count] = '\0';
	printk(KERN_INFO "User buffer: %s", str);
	kfree(str);
	return count;
}

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	if (cmd == IOCTL_SET_LEN) {
		if (copy_from_user(&len, (int __user *)arg, sizeof(int)) != 0) {
			printk(KERN_ALERT "Error copying from userspace.\n");
			if (len > 4)
				len = 4;
			if (len < 1)
				len = 1;
		}
	}
	if (cmd == IOCTL_GET_LEN) {
		if (copy_to_user((int __user *)arg, &len, sizeof(int)) != 0)
			printk(KERN_ALERT "Error copying to userspace.\n");
	}
	return 0;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
	.unlocked_ioctl = my_ioctl,
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

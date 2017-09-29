#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/atomic.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/mutex.h>

DEFINE_MUTEX(my_lock);

static char buffer[128];
static int len;

static void read_buffer(char *str, int len, loff_t index)
{
	int i;

	for (i = 0; i < len; i++)
		str[i] = buffer[index+i];
	str[len] = 0;
}

static ssize_t dev_read(struct file *filp, char __user *buf,
					size_t count, loff_t *offp)
{
	char b_tmp[6];
	int l_tmp;

	mutex_lock(&my_lock);
	l_tmp = len;
	mutex_unlock(&my_lock);
	if (!count || *offp >= l_tmp)
		return 0;

	if (l_tmp >= 5)
		count = 5;
	else
		count = l_tmp % 5;

	read_buffer(b_tmp, count, *offp);
	if (copy_to_user(buf, b_tmp, count))
		return -EFAULT;

	*offp += count;
	return count;
}

static ssize_t dev_write(struct file *filp, const char __user *buf,
					size_t count, loff_t *offp)
{
	char tmp[6];
	int i;
	
	mutex_lock(&my_lock);
	if (count > 5)
		count = 5;
	if (copy_from_user(tmp, buf, count))
		return -EFAULT;
	for (i = 0; i < count; i++) {
		buffer[len+i] = tmp[i];
		msleep(20);
	}
	buffer[len+count] = 0;
	len += count;
	*offp += count;
	mutex_unlock(&my_lock);
	return count;
}

static const struct file_operations my_read_fops = {
	.owner = THIS_MODULE,
	.read = dev_read
};

static struct miscdevice my_dev_read = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "read",
	.fops = &my_read_fops
};

static const struct file_operations my_write_fops = {
	.owner = THIS_MODULE,
	.write = dev_write
};

static struct miscdevice my_dev_write = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "write",
	.fops = &my_write_fops
};

static int my_init(void)
{
	misc_register(&my_dev_write);
	misc_register(&my_dev_read);
	return 0;
}

static void my_exit(void)
{
	misc_deregister(&my_dev_write);
	misc_deregister(&my_dev_read);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

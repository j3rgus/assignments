#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>
#include <linux/spinlock.h>
#include <asm/atomic.h>

#define SET_LEN 0
#define GET_LEN 1

#define IOCTL_BASE 'a'
#define IOCTL_SET_LEN _IOW(IOCTL_BASE, SET_LEN, int)
#define IOCTL_GET_LEN _IOR(IOCTL_BASE, GET_LEN, int)

#define MAX_WRITE 1

static DEFINE_SPINLOCK(my_lock);

static struct debugfs_blob_wrapper bin_data;
static struct dentry *dir;
static atomic_t len = ATOMIC_INIT(5);
static u16 rw_count;
static u16 w_count;

int my_open(struct inode *inode, struct file *filp)
{
	spin_lock(&my_lock);
	if ((w_count >= MAX_WRITE) && (filp->f_mode & FMODE_WRITE)) {
		printk(KERN_INFO "Max writting: %d", MAX_WRITE);
		spin_unlock(&my_lock);
		return -EBUSY;
	}
	printk(KERN_INFO "OPEN");
	if (filp->f_mode & FMODE_WRITE)
		w_count++;
	rw_count++;
	spin_unlock(&my_lock);
	return 0;
}

int my_release(struct inode *inode, struct file *filp)
{
	spin_lock(&my_lock);
	printk(KERN_INFO "RELEASE");
	if (filp->f_mode & FMODE_WRITE)
		w_count--;
	rw_count--;
	spin_unlock(&my_lock);
	return 0;
}

ssize_t my_read(struct file *filp, char __user *buf,
					size_t count, loff_t *offp)
{
	char *str = "Ahoj";
	int l_tmp;

	l_tmp = atomic_read(&len);
	if (count < l_tmp)
		return -EINVAL;
	if (*offp >= l_tmp)
		return 0;
	if (copy_to_user(buf, str, l_tmp))
		return -EINVAL;
	*offp += l_tmp;
	return l_tmp;
}

ssize_t my_write(struct file *filp, const char __user *buf,
					size_t count, loff_t *offp)
{
	char *str;

	if (count > 50)
		count = 50;
	str = kmalloc(count+1, GFP_KERNEL);
	if (copy_from_user(str, buf, count))
		return -EFAULT;
	str[count] = '\0';
	printk(KERN_INFO "User buffer: %s", str);
	kfree(str);
	return count;
}

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	int l_tmp;

	l_tmp = atomic_read(&len);
	if (cmd == IOCTL_SET_LEN) {
		if (copy_from_user(&l_tmp, (int __user *)arg,
						sizeof(int)) != 0) {
			printk(KERN_ALERT "Error copying from userspace.\n");
			if (l_tmp > 5)
				l_tmp = 5;
			if (l_tmp < 1)
				l_tmp = 1;
			atomic_set(&len, l_tmp);
		}
	}
	if (cmd == IOCTL_GET_LEN) {
		if (copy_to_user((int __user *)arg, &l_tmp, sizeof(int)) != 0)
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
	bin_data.data = THIS_MODULE->module_core;
	bin_data.size = THIS_MODULE->core_text_size;
	dir = debugfs_create_dir("pb173", NULL);
	if (dir == NULL)
		printk(KERN_INFO "Can't make dir in debugfs");
	if (debugfs_create_u16("count", S_IRUSR, dir, &rw_count) == NULL)
		printk(KERN_INFO "Can't make file (count) in debug dir");
	if (debugfs_create_blob("bin_core", S_IRUSR, dir, &bin_data) == NULL)
		printk(KERN_INFO "Can't make file (bin_core) in debug dir");
	print_hex_dump_bytes(": ", DUMP_PREFIX_OFFSET, bin_data.data,
							bin_data.size);
	misc_register(&my_misc);
	return 0;
}

static void my_exit(void)
{
	debugfs_remove_recursive(dir);
	misc_deregister(&my_misc);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

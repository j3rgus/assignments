#include <linux/io.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/miscdevice.h>
#include <linux/etherdevice.h>
#include <linux/string.h>
#include <linux/skbuff.h>
#include <linux/kfifo.h>

#define FIFO_SIZE 8192

DEFINE_KFIFO(fifo, char, FIFO_SIZE);
struct net_device *my_dev;

int my_open(struct net_device *dev)
{
	printk(KERN_INFO "%s\n", __func__);
	return 0;
}

int my_stop(struct net_device *dev)
{
	printk(KERN_INFO "%s\n", __func__);
	return 0;
}

int my_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	int len;

	len = kfifo_len(&fifo);
	if ((len + skb->len) >= FIFO_SIZE)
		goto end;
	kfifo_in(&fifo, skb->data, skb->len);
	end:
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}

ssize_t my_read(struct file *filp, char __user *buf,
					size_t count, loff_t *offp)
{
	int ret, num;

	ret = kfifo_to_user(&fifo, buf, count, &num);
	if (ret < 0)
		return ret;
	return num;
}

ssize_t my_write(struct file *filp, const char __user *buf,
					size_t count, loff_t *offp)
{
	struct sk_buff *skb;
	int retval;

	skb = netdev_alloc_skb(my_dev, count);
	if (!skb)
		return -ENOMEM;
	retval = simple_write_to_buffer(skb->data, count, offp, buf, FIFO_SIZE);
	if (retval < 0)
		return retval;
	skb->len = count;
	skb->protocol = eth_type_trans(skb, my_dev);
	netif_rx(skb);
	return retval;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
};

static struct miscdevice my_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "netfile",
	.fops = &my_fops,
};

static struct net_device_ops ndev_ops = {
	.ndo_open = my_open,
	.ndo_stop = my_stop,
	.ndo_start_xmit = my_start_xmit,
};

static int my_init(void)
{
	int ret;

	my_dev = alloc_etherdev(0);
	if (!my_dev)
		return -ENOMEM;
	strcpy(my_dev->name, "myeth0");
	my_dev->netdev_ops = &ndev_ops;
	random_ether_addr(my_dev->dev_addr);
	ret = register_netdev(my_dev);
	if (ret)
		return ret;
	ret = misc_register(&my_misc);
	if (ret)
		return ret;
	return 0;
}

static void my_exit(void)
{
	misc_deregister(&my_misc);
	unregister_netdev(my_dev);
	free_netdev(my_dev);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

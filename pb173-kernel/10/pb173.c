#include <linux/io.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/slab.h>

#define REG_RAISED_INT(map)		((void *)(map+0x40))
#define REG_ENABLED_INT(map)	((void *)(map+0x44))
#define REG_RAISE_INT(map)		((void *)(map+0x60))
#define REG_ACK_INT(map)		((void *)(map+0x64))

struct dev_data {
	void *mapped;
	struct timer_list *pci_timer;
};

static void int_timer_fun(unsigned long d)
{
	struct dev_data *data = (struct dev_data *) d;

	writel(0x1000, REG_RAISE_INT(data->mapped));
	mod_timer(data->pci_timer, jiffies + msecs_to_jiffies(100));
}

irqreturn_t my_handler(int irq, void *data)
{
	u32 in;

	in = readl(REG_RAISED_INT(data));
	if (!in) {
		printk(KERN_INFO "INT not from this dev.\n");
		return IRQ_NONE;
	}
	printk(KERN_INFO "INT handled.\n");
	writel(0x1000, REG_ACK_INT(data));
	return IRQ_HANDLED;
}

int my_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	struct dev_data *pci_data;
	struct timer_list *timer;
	int error;

	pci_data = kmalloc(sizeof(struct dev_data), GFP_KERNEL);
	if (!pci_data)
		return -ENOMEM;
	timer = kmalloc(sizeof(struct timer_list), GFP_KERNEL);
	if (!timer)
		return -ENOMEM;
	if (pci_enable_device(dev) < 0)
		return -EIO;
	if (pci_request_region(dev, 0, "combo"))
		return -EBUSY;
	pci_data->mapped = ioremap(pci_resource_start(dev, 0),
						pci_resource_len(dev, 0));
	if (!pci_data->mapped)
		return -ENOMEM;
	pci_data->pci_timer = timer;
	setup_timer(pci_data->pci_timer, int_timer_fun,
					(unsigned long)pci_data);
	mod_timer(pci_data->pci_timer, jiffies + msecs_to_jiffies(100));
	error = request_irq(dev->irq, &my_handler, IRQF_SHARED, "pok",
							pci_data->mapped);
	if (error)
		return error;
	writel(0x1000, REG_ENABLED_INT(pci_data->mapped));
	pci_set_drvdata(dev, pci_data);
	return 0;
}

void my_remove(struct pci_dev *dev)
{
	struct dev_data *pci_data;

	pci_data = (struct dev_data *) pci_get_drvdata(dev);
	free_irq(dev->irq, pci_data);
	del_timer_sync(pci_data->pci_timer);
	kfree(pci_data->pci_timer);
	iounmap(pci_data->mapped);
	kfree(pci_data);
	pci_release_region(dev, 0);
	pci_disable_device(dev);
}

struct pci_device_id my_table[] = {
	{PCI_DEVICE(0x18ec, 0xc058)},
	{0,}
};
MODULE_DEVICE_TABLE(pci, my_table);

struct pci_driver my_pci_driver = {
	.name = "combo",
	.id_table = my_table,
	.probe = my_probe,
	.remove = my_remove
};


static int my_init(void)
{
	int ret;

	ret = pci_register_driver(&my_pci_driver);
	if (ret)
		return ret;

	return 0;
}

static void my_exit(void)
{
	pci_unregister_driver(&my_pci_driver);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

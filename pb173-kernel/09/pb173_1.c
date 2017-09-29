#include <linux/io.h>
#include <linux/module.h>
#include <linux/pci.h>

/* 18ec:c058 */

#define YEAR(x)		(x>>28)
#define MONTH(x)	((x>>24) & 0xf)
#define DAY(x)		((x>>16) & 0xff)
#define HOUR(x)		((x>>8) & 0xff)
#define MINUTE(x)	(x & 0xff)

static void *map;

int my_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	u32 m;

	if (pci_enable_device(dev) < 0)
		return -EIO;
	if (pci_request_region(dev, 0, "combo"))
		return -EBUSY;
	printk(KERN_INFO "PHYS: %lx\n", (unsigned long)pci_resource_start(dev, 0));
	map = ioremap(pci_resource_start(dev, 0), pci_resource_len(dev, 0));
	m = readl(map);
	printk(KERN_INFO "%u.%u.20%.2u %.2u:%.2u\n", YEAR(m), MONTH(m),
				DAY(m), HOUR(m), MINUTE(m));
	return 0;
}

void my_remove(struct pci_dev *dev)
{
	iounmap(map);
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
	struct pci_dev *pdev = NULL;
	int ret;

	while ((pdev = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, pdev))) {
		printk(KERN_INFO "vendor: %.4x device: %.4x\n", pdev->vendor,
				pdev->device);
	}

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

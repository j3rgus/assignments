#include <linux/io.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/list.h>

static struct my_pci_devs *s;

struct my_pci_devs {
	struct pci_dev *pdev;
	struct list_head list;
};
LIST_HEAD(my_list);

static int my_init(void)
{
	struct pci_dev *pdev = NULL;

	for_each_pci_dev(pdev) {
		s = kmalloc(sizeof(*s), GFP_KERNEL);
		s->pdev = pci_dev_get(pdev);
		list_add(&s->list, &my_list);
	}

	return 0;
}

static void my_exit(void)
{
	struct pci_dev *pdev = NULL;
	struct my_pci_devs *ss;

	for_each_pci_dev(pdev) {
		list_for_each_entry_safe(s, ss, &my_list, list) {
			if ((pci_domain_nr(pdev->bus) != pci_domain_nr(s->pdev->bus)) &&
				(pdev->bus->number != s->pdev->bus->number) &&
				(PCI_SLOT(pdev->devfn) != PCI_SLOT(s->pdev->devfn)) &&
				(PCI_FUNC(pdev->devfn) != PCI_FUNC(s->pdev->devfn)))
				printk(KERN_INFO "%.4x:%.4x\n", pdev->vendor, pdev->device);
		}
	}
	list_for_each_entry_safe(s, ss, &my_list, list) {
		pci_dev_put(s->pdev);
		list_del(&s->list);
		kfree(s);
	}
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

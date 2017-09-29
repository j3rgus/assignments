#include <linux/io.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/dma-mapping.h>

/* 18ec:c058 */
#define REG_RAISED_INT(x)	((void *)(x+0x40))
#define REG_ENABLED_INT(x)	((void *)(x+0x44))
#define REG_ACK_INT(x)		((void *)(x+0x64))

#define SRC_ADDR_REG(x) 	((void *)(x+0x80))
#define DST_ADDR_REG(x) 	((void *)(x+0x84))
#define COUNT_REG(x)		((void *)(x+0x88))
#define CMD_REG(x)			((void *)(x+0x8c))

void *virt;

struct mem_struct {
	void *map;
	dma_addr_t phys;
	struct tasklet_struct task;
};

int my_map(struct file *filp, struct vm_area_struct *vma)
{
	if ((vma->vm_end - vma->vm_start) > PAGE_SIZE)
		return -EINVAL;
	if (vma->vm_pgoff == 0) {
		if ((vma->vm_flags & (VM_WRITE | VM_READ)) != VM_READ)
			return -EINVAL;
		return remap_pfn_range(vma, vma->vm_start,
					virt_to_phys(virt) >> PAGE_SHIFT,
						PAGE_SIZE, vma->vm_page_prot);
	}
	return 0;
}

void task_print(unsigned long d)
{
	printk(KERN_INFO "%10s\n", (char *)(virt+20));
}

void write_combo(u32 source, u32 dest, u32 count, u32 cmd, void *bar)
{
	writel(source, SRC_ADDR_REG(bar));
	writel(dest, DST_ADDR_REG(bar));
	writel(count, COUNT_REG(bar));
	writel(cmd, CMD_REG(bar));
}

irqreturn_t my_handler(int irq, void *data)
{
	u32 in;
	struct mem_struct *my_mem = (struct mem_struct *) data;

	in = readl(REG_RAISED_INT(my_mem->map));
	if (!in)
		return IRQ_NONE;
	in = 1<<31;
	writel(in, CMD_REG(my_mem->map));
	writel(in, REG_ACK_INT(my_mem->map));
	tasklet_schedule(&my_mem->task);
	return IRQ_HANDLED;
}

int my_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
	struct mem_struct *my_mem;
	int ret;

	my_mem = kmalloc(sizeof(struct mem_struct), GFP_KERNEL);
	if (!my_mem)
		return -ENOMEM;
	if (pci_enable_device(pdev) < 0)
		return -EIO;
	if (pci_request_region(pdev, 0, "combo"))
		return -EBUSY;
	ret = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));
	if (ret)
		return ret;
	pci_set_master(pdev);
	my_mem->map = ioremap(pci_resource_start(pdev, 0),
				pci_resource_len(pdev, 0));
	if (!my_mem->map)
		return -ENOMEM;
	virt = dma_alloc_coherent(&pdev->dev, PAGE_SIZE, &my_mem->phys,
					GFP_KERNEL);
	if (!virt)
		return -ENOMEM;
	strcpy(virt, "Helo World");
	write_combo(my_mem->phys, 0x40000, 10, 0xC5,
			my_mem->map);	/* 11000101 */
	while (readl(CMD_REG(my_mem->map)) & 1)
		msleep(100);
	write_combo(0x40000, my_mem->phys+10, 10, 0xA9,
			my_mem->map);	/* 10101001 */
	while (readl(CMD_REG(my_mem->map)) & 1)
		msleep(100);
	printk(KERN_INFO "%s\n", (char *)virt);
	ret = request_irq(pdev->irq, my_handler, IRQF_SHARED, "pok",
				(void *)my_mem);
	if (ret)
		return ret;
	writel(0x100, REG_ENABLED_INT(my_mem->map));
	tasklet_init(&my_mem->task, task_print, (unsigned long)my_mem);
	write_combo(0x40000, my_mem->phys+20, 10, 0x29,
			my_mem->map);	/* 101001 */
	pci_set_drvdata(pdev, my_mem);
	return 0;
}

void my_remove(struct pci_dev *pdev)
{
	struct mem_struct *my_mem;

	my_mem = (struct mem_struct *) pci_get_drvdata(pdev);
	writel(0, REG_ENABLED_INT(my_mem->map));
	free_irq(pdev->irq, my_mem);
	tasklet_kill(&my_mem->task);
	dma_free_coherent(&pdev->dev, PAGE_SIZE, virt, my_mem->phys);
	iounmap(my_mem->map);
	kfree(my_mem);
	pci_release_region(pdev, 0);
	pci_disable_device(pdev);
}

struct pci_device_id my_table[] = {
	{PCI_DEVICE(0x18ec, 0xc058)},
	{0,}
};
MODULE_DEVICE_TABLE(pci, my_table);

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.mmap = my_map,
};

static struct pci_driver my_pci_driver = {
	.name = "combo",
	.id_table = my_table,
	.probe = my_probe,
	.remove = my_remove
};

static struct miscdevice my_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &my_fops,
	.name = "my_name",
};

static int my_init(void)
{
	pci_register_driver(&my_pci_driver);
	misc_register(&my_misc);
	return 0;
}

static void my_exit(void)
{
	pci_unregister_driver(&my_pci_driver);
	misc_deregister(&my_misc);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

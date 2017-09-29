#include <linux/module.h>
#include <linux/io.h>
#include <linux/mm.h>

static int my_init(void)
{
	char *virt;
	void *map;
	phys_addr_t phys;
	struct page *page;

	virt = (char *)__get_free_page(GFP_KERNEL);
	strcpy(virt, "Ahoj");
	phys = virt_to_phys(virt);
	page = virt_to_page(virt);
	SetPageReserved(page);
	map = ioremap(phys, PAGE_SIZE);
	printk(KERN_INFO"virt: %p\nphys: %lx\npage: %p",
				virt, (long unsigned int)phys, page);
	printk(KERN_INFO "map: %p\npfn: %lx", map, page_to_pfn(page));
	printk(KERN_INFO "virt: %s\nmap: %s", virt, (char *)map);
	strcpy((char *)map, "Hello");
	printk(KERN_INFO "virt: %s", virt);
	iounmap(map);
	ClearPageReserved(page);
	free_page((unsigned long)virt);

	return 0;
}

static void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

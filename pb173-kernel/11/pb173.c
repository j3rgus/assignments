#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/io.h>

void *page1;
void *page2;
void *vmem1;
void *vmem2;

static struct page *my_find_page(unsigned long off)
{
	if (off < PAGE_SIZE)
		return virt_to_page(page1);
	if ((off >= PAGE_SIZE) && (off < PAGE_SIZE*2))
		return virt_to_page(page2);
	if ((off >= PAGE_SIZE*2) && (off < PAGE_SIZE*3))
		return vmalloc_to_page(vmem1);
	if ((off >= PAGE_SIZE*3) && (off < PAGE_SIZE*4))
		return vmalloc_to_page(vmem2);
	return NULL;
}

int my_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	unsigned long offset = vmf->pgoff << PAGE_SHIFT;
	struct page *page;

	page = my_find_page(offset);
	if (!page)
		return VM_FAULT_SIGBUS;
	get_page(page);
	vmf->page = page;
	return 0;
}

struct vm_operations_struct vm_my_ops = {
	.fault = my_fault,
};

int my_map(struct file *filp, struct vm_area_struct *vma)
{
	if ((vma->vm_end - vma->vm_start) > 2*PAGE_SIZE)
		return -EINVAL;
	if (vma->vm_pgoff < 4) {
		if (vma->vm_pgoff <= 1) {
			if ((vma->vm_flags & (VM_WRITE | VM_READ)) != VM_READ)
				return -EINVAL;
		}
		if ((vma->vm_pgoff >= 2) && (vma->vm_pgoff < 4)) {
			if ((vma->vm_flags & (VM_WRITE | VM_READ)) !=
							(VM_WRITE | VM_READ))
				return -EINVAL;
		}
	} else
		return -EINVAL;
	vma->vm_ops = &vm_my_ops;
	return 0;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.mmap = my_map,
};

static struct miscdevice my_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &my_fops,
	.name = "my_name",
};



static int my_init(void)
{
	page1 = (void *)__get_free_page(GFP_KERNEL);
	if (!page1)
		return -ENOMEM;
	page2 = (void *)__get_free_page(GFP_KERNEL);
	if (!page2)
		return -ENOMEM;
	vmem1 = vmalloc_user(PAGE_SIZE);
	if (!vmem1)
		return -ENOMEM;
	vmem2 = vmalloc_user(PAGE_SIZE);
	if (!vmem2)
		return -ENOMEM;

	strcpy(page1, "Ahoj");
	strcpy(page2, "Cau");
	strcpy(vmem1, "Ahoj1");
	strcpy(vmem2, "Cau1");

	misc_register(&my_misc);
	return 0;
}

static void my_exit(void)
{
	free_page((unsigned long)page1);
	free_page((unsigned long)page2);
	vfree(vmem1);
	vfree(vmem2);
	misc_deregister(&my_misc);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

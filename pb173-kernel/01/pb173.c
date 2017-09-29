#include <linux/module.h>
#include <linux/init.h>		/* Needed for init macros */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/slab.h>		/* Needed for GFP_KERNEL */
#include <linux/jiffies.h>	/* Needed for jiffies variable */
#include <linux/bitmap.h>	/* Needed for bitmap_allocate_region() */

static int my_function(void)
{
	return 0;
}

static int my_init(void)
{
	pr_info("Hello World");
	return 0;
}

static void my_exit(void)
{
	void *mem = kmalloc(1000, GFP_KERNEL);
	int stack_var;

	if (mem)
		strcpy(mem, "Bye");
	pr_info("%s", (char *)mem);
	pr_info("Address of allocated memory is %p", mem);
	pr_info("Address of variable on stack is %p", &stack_var);
	pr_info("Address of jiffies is %p", &jiffies);
	pr_info("Address of local function is %p", my_function);
	pr_info("Address of extern function is %p",
		bitmap_allocate_region);
	pr_info("Return address of current function %pF",
		__builtin_return_address(0));
	kfree(mem);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

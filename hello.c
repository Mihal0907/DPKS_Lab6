#define DEBUG

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Oleksandr Arzamastsev");
MODULE_DESCRIPTION("Debugger");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(l_head);

struct timer {
	ktime_t start;
	ktime_t end;
	struct list_head list;
};

static unsigned int ticks = 1;

module_param(ticks, uint, S_IRUGO);
MODULE_PARM_DESC(ticks, "Number of Ticks to print");

static int __init hello_init(void)
{
	unsigned int tick;
	struct timer *ptr;

	if (ticks == 0 || (ticks >= 5 && ticks <= 10))
		pr_warn("Warning\n");

	BUG_ON(ticks > 10);

	pr_debug("In the beginning...");
	for (tick = 0; tick < ticks; ++tick) {
		ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);
		ptr->start = ktime_get();
		pr_debug("Ooohhhhhh! /tick %d/\n", tick);
		ptr->end = ktime_get();
		list_add(&ptr->list, &l_head);
	}
	pr_debug("In the end......");

	return 0;
}

static void __exit hello_exit(void)
{
	struct list_head *pos;
	struct list_head *num;
	struct timer *cur;

	pr_info("Exit module\n");

	list_for_each_safe(pos, num, &l_head) {
		cur = list_entry(pos, struct timer, list);
		pr_info("Time duration = %lld ns\n", cur->end - cur->start);
		list_del(pos);
		kfree(cur);
	}
}

module_init(hello_init);
module_exit(hello_exit);

#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Expose jiffies through /proc/jiffies");
MODULE_AUTHOR("Brandon Tiong");

#define PROC_NAME "jiffies"

static struct proc_dir_entry *jiffies_entry;

static ssize_t proc_read(struct file *file, char __user *usr_buf, 
                         size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[128];
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;
    rv = snprintf(buffer, sizeof(buffer), "Current jiffies = %lu\n", jiffies);

   
    if(copy_to_user(usr_buf, buffer, rv)) {
        rv = -EFAULT;
    }

    return rv;
}


static struct proc_ops my_fops = {
    .proc_read = proc_read,
};

static int __init jiffies_init(void)
{

    jiffies_entry = proc_create(PROC_NAME, 0666, NULL, &my_fops);

    if (jiffies_entry == NULL) {
        return -ENOMEM;
    }

    return 0;
}

static void __exit jiffies_exit(void)
{
    proc_remove(jiffies_entry);
}

module_init(jiffies_init);
module_exit(jiffies_exit);
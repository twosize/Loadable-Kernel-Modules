#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Expose elapsed seconds through /proc/seconds");
MODULE_AUTHOR("Brandon Tiong");

#define PROC_NAME "seconds"

static unsigned long loaded_jiffies; 
static struct proc_dir_entry *seconds_entry;

static ssize_t proc_read(struct file *file, char __user *usr_buf, 
                         size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[128];
    static int completed = 0;
    unsigned long elapsed_seconds;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;

    elapsed_seconds = (jiffies - loaded_jiffies) / HZ;
    rv = snprintf(buffer, sizeof(buffer), "Elapsed seconds = %lu\n", elapsed_seconds);

    if(copy_to_user(usr_buf, buffer, rv)) {
        rv = -EFAULT;
    }

    return rv;
}

static struct proc_ops my_fops = {
    .proc_read = proc_read,
};

static int __init seconds_init(void)
{
    loaded_jiffies = jiffies;  

    seconds_entry = proc_create(PROC_NAME, 0666, NULL, &my_fops);

    if (seconds_entry == NULL) {
        return -ENOMEM;
    }

    return 0;
}

static void __exit seconds_exit(void)
{
    proc_remove(seconds_entry);
}

module_init(seconds_init);
module_exit(seconds_exit);
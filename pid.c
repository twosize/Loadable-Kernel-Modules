#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/sched.h>  
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task Info Kernel Module");
MODULE_AUTHOR("Brandon Tiong");

#define PROC_NAME "pid"
static long stored_pid = -1;

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    int rv = 0;
    char buffer[128];
    static int completed = 0;
    struct task_struct *task;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;

    rcu_read_lock();
    task = pid_task(find_vpid(stored_pid), PIDTYPE_PID);
    if(task) {
        rv = snprintf(buffer, sizeof(buffer), "command = [%s] pid = [%ld] state = [%c]\n", 
                        task->comm, stored_pid, task_state_to_char(task)); // Use task_state_to_char for state
    } else {
        rv = snprintf(buffer, sizeof(buffer), "PID not found\n");
    }
    rcu_read_unlock();

    if (copy_to_user(usr_buf, buffer, rv)) {
        rv = -EFAULT;
    }

    return rv;
}

ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos) {
    char *k_mem = kmalloc(count, GFP_KERNEL);

    if (!k_mem) {
        return -ENOMEM;
    }

    if (copy_from_user(k_mem, usr_buf, count)) {
        kfree(k_mem);
        return -EFAULT;
    }

    kstrtol(k_mem, 10, &stored_pid);
    kfree(k_mem);
    return count;
}

static struct proc_ops my_fops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

static int __init task_info_init(void) {
    proc_create(PROC_NAME, 0666, NULL, &my_fops);
    return 0;
}

static void __exit task_info_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
}

module_init(task_info_init);
module_exit(task_info_exit);
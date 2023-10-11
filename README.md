# Loadable-Kernel-Modules



This assignment will involve designing two kernel modules:

1. Design a kernel module that creates a /proc file named /proc/jiffies that reports the current value of jiffies when the /proc/jiffies file is read, such as with the command

cat /proc/jiffies Be sure to remove /proc/jiffies when the module is removed.

2. Design a kernel module that creates a proc file named /proc/seconds that reports the number of elapsed seconds since the kernel module was loaded. This will involve using the value of jiffies as well as the HZ rate. When a user enters the command

cat /proc/seconds your kernel module will report the number of seconds that have elapsed since the kernel module was first loaded. Be sure to remove /proc/seconds when the module is removed.




Assignment Design a kernel module that iterates through all tasks in the system using the for each process() macro. In particular, output the task command, state, and process id of each task. (You will probably have to read through the task struct structure in <linux/sched.h> to obtain the names of these fields.) Write this code in the module entry point so that its contents will appear in the kernel log buffer, which can be viewed using the dmesg command. To verify that your code is working correctly, compare the contents of the kernel log buffer with the output of the following command, which lists all tasks in the system:

ps -el The two values should be very similar. Because tasks are dynamic, however, it is possible that a few tasks may appear in one listing but not the other.

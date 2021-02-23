/** jiffies.c
*
 * Kernel module that communicates with /proc file system.
 * 
 * The makefile must be modified to compile this program.
 * Change the line "simple.o" to "jiffies.o"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define NAME "jiffies"

ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static struct file_operations proc_ops = {
.owner = THIS_MODULE,
.read = proc_read,
};

int proc_init(void) {
    proc_create(NAME, 0, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", NAME);
    printk( KERN_INFO "In proc_init(), Jiffies is: %lu\n", jiffies);
    return 0;
}

void proc_exit(void) {
    remove_proc_entry("jiffies", NULL);
    printk( KERN_INFO "/proc/%s removed\n", NAME);
    return;
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
int rv = 0;
char buffer[BUFFER_SIZE];
static int finish = 0;
if (finish) {
finish = 0;
return 0;
}
finish = 1;
rv = sprintf(buffer, "The current jiffies value is %lu\n", jiffies);
// copies the contents of buffer to userspace usr_buf
copy_to_user(usr_buf, buffer, rv);

return rv;
}

/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");


// //================================================================================
// // inscrem.sh for jiffies.c
// //================================================================================
// #!/bin/bash
// ​
// # clear the message queue, then
// # insert simple.ko into the kernel and remove it again, then
// # display the messages as a result of doing so
// ​
// sudo -p "osc"
// sudo dmesg -c 
// clear 
// printf "beginning script...\n\n"
// sudo insmod jiffies.ko
// cat /proc/jiffies
// sleep 3
// cat /proc/jiffies
// sleep 3
// cat /proc/jiffies
// sleep 3
// cat /proc/jiffies
// sudo rmmod jiffies
// sudo dmesg 
// printf "\n...end of script\n\n"
// //================================================================================
// // inscrem.sh for jiffies.c
// //================================================================================




// //================================================================================
// // Makefile for jiffies.c
// //================================================================================
// obj-m += jiffies.o
// all:
// 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
// clean:
// 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
// //================================================================================
// // Makefile for jiffies.c
// //================================================================================





// //================================================================================
// // output for jiffies.c
// //================================================================================

// beginning script...

// The current jiffies value is 4295518507
// The current jiffies value is 4295519257
// The current jiffies value is 4295520008
// The current jiffies value is 4295520758
// [ 2505.074640] /proc/jiffies created
// [ 2505.074643] In proc_init(), Jiffies is: 4295518504
// [ 2514.096187] /proc/jiffies removed

// ...end of script

// //================================================================================
// // output for jiffies.c
// //================================================================================

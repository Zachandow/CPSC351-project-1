/** seconds.c
*
 * Kernel module that communicates with /proc file system.
 * 
 * The makefile must be modified to compile this program.
 * Change the line "simple.o" to "seconds.o"
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
#define NAME "seconds"

unsigned long int first_jiffies, second_jiffies;


ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static struct file_operations proc_ops = {
.owner = THIS_MODULE,
.read = proc_read,
};

int proc_init(void) {
    first_jiffies = jiffies;
    proc_create(NAME, 0, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", NAME);
    printk( KERN_INFO "In proc_init(), the amount of seconds have been recorded\n");
    return 0;
}

void proc_exit(void) {
    second_jiffies = jiffies;
    remove_proc_entry(NAME, NULL);
    //printk( KERN_INFO "The time elasped is %d seconds\n", ((second_jiffies-first_jiffies)/HZ));
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
rv = sprintf(buffer, "The amount of seconds are %lu seconds\n", ((jiffies-first_jiffies)/HZ));
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
// // inscrem.sh for seconds.c
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
// sudo insmod seconds.ko
// cat /proc/seconds
// sleep 3
// cat /proc/seconds
// sleep 3
// cat /proc/seconds
// sleep 3
// cat /proc/seconds
// sudo rmmod seconds
// sudo dmesg 
// printf "\n...end of script\n\n"
// //================================================================================
// // inscrem.sh for seconds.c
// //================================================================================




// //================================================================================
// // Makefile for seconds.c
// //================================================================================
// obj-m += seconds.o
// all:
// 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
// clean:
// 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
// //================================================================================
// // Makefile for seconds.c
// //================================================================================





//================================================================================
// output for seconds.c
//================================================================================
// beginning script...

// insmod: ERROR: could not insert module seconds.ko: File exists
// The amount of seconds are 16 seconds
// The amount of seconds are 19 seconds
// The amount of seconds are 22 seconds
// The amount of seconds are 25 seconds
// [ 3814.403736] /proc/seconds removed

// ...end of script

// //================================================================================
// // output for seconds.c
// //================================================================================

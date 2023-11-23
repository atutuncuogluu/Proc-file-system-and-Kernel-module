# Proc-file-system-and-Kernel-module
In this project, we will create a file named "mytaskinfo" in the proc file system. We will implement an interactive read-write operation on this file.



This program operates by extracting the task_state values defined in the task_struct structure, which include:

"R (running)"
"S (sleeping)"
"D (disk sleep)"
"T (stopped)"
"t (tracing stop)"
"X (dead)"
"Z (zombie)"
"P (parked)"
"I (idle)"
When these values are written to a file, the program records the information in the format: "PID: X STATE: X UTIME: X STIME: X UTIME+STIME: X". This data is written to the file in real-time. Upon subsequent reads of the file, you can retrieve the output.

Example usage:

echo "R" > /proc/mytaskinfo 
cat /proc/mytaskinfo
After running the above commands, the content of the file /proc/mytaskinfo would display information in the format mentioned, where "X" is replaced by the corresponding values for PID, STATE, UTIME, STIME, and UTIME+STIME.

Also you can test this module with "test.c" file

If you dont know how to implement a kernel module here is the basic implemantation:


STEP 1:

/* This function is called when the module is loaded. */
static int __init my_module_init(void)
{

    printk(KERN_INFO "Module created", PROCF_NAME);

    return 0;
}

/* This function is called when the module is removed. */
static void __exit my_module_exit(void)
{
    printk(KERN_INFO "Module removed", PROCF_NAME);
}

/* Macros for registering module entry and exit points.
 */
module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My Module");
MODULE_AUTHOR("your name");

STEP 2:
"After creating a .c file, it is necessary to write a Makefile for generating the required files."
here is example Makefile for this project:

obj-m += my_module.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean


# My Module

This repository contains a simple kernel module that can be built using a Makefile. Follow the steps below to create and use the module.

## Prerequisites

Make sure you have the necessary development tools installed on your system.

```bash
sudo apt-get update
sudo apt-get install build-essential


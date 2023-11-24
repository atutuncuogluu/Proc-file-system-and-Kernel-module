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

When these values are written to a file, the program records the information in the format:
 "*PID: X STATE: X UTIME: X STIME: X UTIME+STIME: X*". 
 This data is written to the file in *real-time*. Upon subsequent reads of the file, you can retrieve the output.

Example usage:
``
echo "R" > /proc/mytaskinfo 
cat /proc/mytaskinfo
``

After running the above commands, the content of the file /proc/mytaskinfo would display information in the format mentioned, where "X" is replaced by the corresponding values for PID, STATE, UTIME, STIME, and UTIME+STIME.

Also you can test this module with *"test.c"* file

If you dont know how to implement a kernel module you should check my other repo:
[Create a kernel module](https://github.com/atutuncuogluu/OShw2)

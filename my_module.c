#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define PROCF_NAME "mytaskinfo"

char *cp = NULL;

static const char *const task_state_array[] = {
    "R (running)",
    "S (sleeping)",
    "D (disk sleep)",
    "T (stopped)",
    "t (tracing stop)",
    "X (dead)",
    "Z (zombie)",
    "P (parked)",
    "I (idle)",
};

struct my_data
{
    char *buffer;
    int len;
    int process_count;
};

void my_init_data(struct file *file)
{
    struct task_struct *task;
    int len = 0;
    int new_len = 1024;
    char *ret = kmalloc(sizeof(char) * new_len, GFP_KERNEL);
    char *copy_ch;
    struct my_data *my_data;
    int process_count = 0;

    if(file->private_data)
    {
        my_data = file->private_data;
        if(my_data->buffer)
        {
            kfree(my_data->buffer);
        }
        kfree(my_data);
    }
    if (!cp)
    {
        printk(KERN_INFO"cp init'e bos geldi");
        return;
    }
    printk(KERN_INFO"init ici cp degeri : %s",cp);
    
    for_each_process(task)
    {
        if (task_state_array[task_state_index(task)][0] == cp[0])
        {
            if (len >= new_len)
            {
                new_len *= 2;
                copy_ch = kmalloc(sizeof(char) * new_len, GFP_KERNEL);
                strncpy(copy_ch, ret, len);
                kfree(ret);
                ret = copy_ch;
            }

            len += sprintf(ret + len, "PID: %d, State: %s, Utime: %llu, Stime: %llu, Utime + Stime: %llu, Vruntime: %lld\n",
                           task->pid,
                           task_state_array[task_state_index(task)],
                           task->utime,
                           task->stime,
                           (task->utime + task->stime),
                           task->se.vruntime);
            process_count++;
        }
    }

    ret[len] = '\0';

    printk(KERN_INFO"my_init ret'in total byte :%d",len);


    my_data = kmalloc(sizeof(struct my_data), GFP_KERNEL);
    my_data->buffer = ret;
    my_data->len = len;
    my_data->process_count = process_count;

    file->private_data = my_data;
}

static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO"open calisti");
    my_init_data(file);
    return 0;
}

static ssize_t my_read(struct file *file, char __user *usr_buf, size_t size, loff_t *offset)
{
    struct my_data *my_data = (struct my_data *)file->private_data;
    int to_copy, total_copied = 0;

    printk(KERN_INFO"read calisti");

    if(!my_data || !my_data->buffer)
    {
        printk(KERN_INFO"mydata bos geldi");
        return 0;
    }

    if (my_data->process_count == 0)
        return 0;

    printk(KERN_INFO"initden gelen byte %d",my_data->len);
    while (total_copied < my_data->len) {
        to_copy = min(size, (size_t)(my_data->len - total_copied));
        printk(KERN_INFO"Kullanici buf size %lu \n kopyalanacak size %d\n  şimdiye kadar kopyalanan %d",
        size,
        to_copy,
        total_copied);
        if (to_copy == 0)
        {
            printk(KERN_INFO"kopyalanacak byte tamamlandı");
            return total_copied;
        }

        
        if (copy_to_user(usr_buf + total_copied, my_data->buffer + total_copied, to_copy))
        {
            printk(KERN_INFO"usera kopyalarken hata ");
            return -EFAULT;
        }

        total_copied += to_copy;
    }

    my_data->process_count = 0;
    return total_copied;
}


static ssize_t my_write(struct file *file, const char __user *usr_buf, size_t size, loff_t *offset)
{
    char *buf = kmalloc(size + 1, GFP_KERNEL);

    printk(KERN_INFO"write calisti");

    if (!buf)
    {
        pr_err("Bellek tahsisi başarısız\n");
        return -ENOMEM;
    }

    if (copy_from_user(buf, usr_buf, size))
    {
        pr_err("Kullanıcıdan veri kopyalama hatası\n");
        kfree(buf);
        return -EFAULT;
    }

    buf[size] = '\0';
    printk(KERN_INFO"buffer-size-kullanıcı girdi size  %s-%lu-%lu",buf,strlen(buf),size);
    if (strlen(buf) < 3)
    {
        if (cp)
            kfree(cp);
        cp = kmalloc(strlen(buf) + 1, GFP_KERNEL);
        strcpy(cp, buf);
        printk(KERN_INFO "write cp: %s\n", cp);
    }

    kfree(buf);

    my_init_data(file);
    return size;
}

int my_release(struct inode *inode, struct file *file)
{
    struct my_data *my_data = (struct my_data *)file->private_data;

    if (my_data) {
        if (my_data->buffer) {
            kfree(my_data->buffer);
        }
        kfree(my_data);
        file->private_data = NULL;
    }

    printk(KERN_INFO "my_release() for /proc/%s\n", PROCF_NAME);

    return 0;
}


const struct proc_ops my_ops = {
    .proc_read = my_read,
    .proc_open = my_open,
    .proc_write = my_write,
    .proc_release = my_release,
    /*bunlari kullanarak dosya davranislarini belirleyebilirsiniz*/
};

/* This function is called when the module is loaded. */
static int __init my_module_init(void)
{
    /* creates the [/proc/procf] entry*/
    proc_create(PROCF_NAME, 0666, NULL, &my_ops);
    cp = kmalloc(sizeof(char) * 2, GFP_KERNEL);
    printk(KERN_INFO "/proc/%s created\n", PROCF_NAME);

    return 0;
}

/* This function is called when the module is removed. */
static void __exit my_module_exit(void)
{
    remove_proc_entry(PROCF_NAME, NULL);

    printk(KERN_INFO "/proc/%s removed\n", PROCF_NAME);
}

/* Macros for registering module entry and exit points.
 */
module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My Task Info Module");
MODULE_AUTHOR("axusname");
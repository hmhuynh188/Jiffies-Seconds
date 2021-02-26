#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

unsigned long int volatile f_jiffies,l_jiffies;
const int hz=HZ;

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count,loff_t *pos);

static struct file_operations proc_ops = {
   .owner = THIS_MODULE,
   .read = proc_read,
};

int proc_init(void)
{
    proc_create(PROC_NAME,0666,NULL,&proc_ops);
    f_jiffies = jiffies;
    printk(KERN_INFO "seconds loaded!\n");
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME,NULL);
    l_jiffies = jiffies;
    printk(KERN_INFO "seconds unloaded!\n");
    printk(KERN_INFO "The running time is %d s\n", ((l_jiffies-f_jiffies)/hz));
}


ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed) {
      completed = 0;
      return 0;
    }
    completed = 1;
    rv = sprintf(buffer, "The jiffies is %lu\n", jiffies);
    copy_to_user(usr_buf, buffer, rv);
    return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
MODULE_AUTHOR("SGG");

//========================================================
// output from seconds.ko 
// beginning script...

//The jiffies is 4298575178
//The jiffies is 4298575935
//[14735.808634] seconds loaded!
//[14741.845787] seconds unloaded!
//[14741.845791] The running time is 6 s

//...end of script
// end of output 

//===============================================
//shell code 

//sudo -p "osc"
//sudo dmesg -c 
//clear 

//printf "beginning script...\n\n"
//sudo insmod seconds.ko

//cat /proc/seconds
//sleep 3

//cat /proc/seconds
//sleep 3 

//sudo rmmod seconds
//sudo dmesg 

//printf "\n...end of script\n\n"
//end of shell code 

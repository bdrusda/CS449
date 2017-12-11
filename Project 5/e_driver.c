#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#include <asm/uaccess.h>

#include "e.h"
/*e_digit_read is the function called when a process calls read() on /dev/e_digit.  It writes the specified digits of e to the buffer passed in the read() call.*/

static ssize_t e_read(struct file * file, char * buf, size_t count, loff_t *ppos)
{
	int len;
	len = count + ppos;
	char temp_buf[len];
	e(temp_buf, len);

	/*We only support reading the whole string at once.*/
	if (count < len)
		return -EINVAL;

	/*If file position is non-zero, then assume the string has been read and indicate there is no more data to be read.*/
	if (*ppos != 0)
		return 0;
	/*Besides copying the string to the user provided buffer, this function also checks that the user has permission to write to the buffer, that it is mapped, etc.*/
	if (copy_to_user(buf, &temp_buf[*ppos], count))
		return -EINVAL;

	/*Tell the user how much data we wrote.*/
	*ppos = count;

	return count;
}

static const struct file_operations e_fops = {
	.owner		= THIS_MODULE,
	.read		= e_read,
};

static struct miscdevice e_driver = {
	MISC_DYNAMIC_MINOR,	/*generate whatever number it likes*/
	"e_driver",				/*name the device in /misc/ e_driver????*/
	&e_fops				/*the function called when a program does file ops using this device*/
};

static int __init e_init(void)
{
	int ret;

	ret = misc_register(&e_driver);	/*create "e_driver" device in /misc*/
	if (ret)
		printk(KERN_ERR "Unable to register e_driver misc device\n");

	return ret;
}

module_init(e_init);

static void __exit e_exit(void)
{
	misc_deregister(&e_driver);
}

module_exit(e_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brendan Drusda <bmd66@pitt.edu>");
MODULE_DESCRIPTION("e driver module");
MODULE_VERSION("dev");

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#include <asm/uaccess.h>

/*
 * e_digit_read is the function called when a process calls read() on
 * /dev/e_digit.  It writes the specified digits of e to the buffer passed in the
 * read() call.
 */

static ssize_t e_read(struct file * file, char * buf, size_t count, loff_t *ppos)
{
	char *e_driver_str = "These are the digits of e\n";
	int len = strlen(e_driver_str); /* Don't include the null byte. */
	/*
	 * We only support reading the whole string at once.
	 */
	if (count < len)
		return -EINVAL;
	/*
	 * If file position is non-zero, then assume the string has
	 * been read and indicate there is no more data to be read.
	 */
	if (*ppos != 0)
		return 0;
	/*
	 * Besides copying the string to the user provided buffer,
	 * this function also checks that the user has permission to
	 * write to the buffer, that it is mapped, etc.
	 */


	if (copy_to_user(buf, e_driver_str, len))
		return -EINVAL;
	/*
	 * Tell the user how much data we wrote.
	 */
	*ppos = len;

	return len;
}

static const struct file_operations e_fops = {
	.owner		= THIS_MODULE,
	.read		= e_read,
};

static struct miscdevice e_driver = {
	MISC_DYNAMIC_MINOR,	/*generate whatever number it likes*/
	"e",				/*name the device in /dev/ e*/
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
	misc_deregister(&e_dev);
}

module_exit(e_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brendan Drusda <bmd66@pitt.edu>");
MODULE_DESCRIPTION("e driver module");
MODULE_VERSION("dev");

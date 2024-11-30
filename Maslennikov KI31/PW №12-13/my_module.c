#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple kernel module that stores command line parameters in sysfs");

static int param1;
static char *param2;

module_param(param1, int, 0644);
MODULE_PARM_DESC(param1, "An integer parameter");

module_param(param2, charp, 0644);
MODULE_PARM_DESC(param2, "A string parameter");

static struct kobject *my_kobj;

static ssize_t param1_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", param1);
}

static ssize_t param2_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%s\n", param2);
}

static struct kobj_attribute param1_attr = __ATTR_RO(param1);
static struct kobj_attribute param2_attr = __ATTR_RO(param2);

static struct attribute *attrs[] = {
    &param1_attr.attr,
    &param2_attr.attr,
    NULL,
};

static struct attribute_group attr_group = {
    .attrs = attrs,
};

static int __init my_module_init(void)
{
    int retval;

    my_kobj = kobject_create_and_add("my_module", kernel_kobj);
    if (!my_kobj)
        return -ENOMEM;

    retval = sysfs_create_group(my_kobj, &attr_group);
    if (retval)
        kobject_put(my_kobj);

    printk(KERN_INFO "my_module: Module loaded with parameters: param1=%d, param2=%s\n", param1, param2);
    return retval;
}

static void __exit my_module_exit(void)
{
    kobject_put(my_kobj);
    printk(KERN_INFO "my_module: Module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
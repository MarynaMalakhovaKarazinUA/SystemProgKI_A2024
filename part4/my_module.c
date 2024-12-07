#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KateKov");
MODULE_DESCRIPTION("Kernel module with parameters exported to sysfs");
MODULE_VERSION("1.0");

// Параметри модуля
static int param1 = 0;
static char *param2 = "default";

module_param(param1, int, 0444);
MODULE_PARM_DESC(param1, "An integer parameter");

module_param(param2, charp, 0444);
MODULE_PARM_DESC(param2, "A string parameter");

// Об'єкт kobject
static struct kobject *kobj_ref;

// Буфер для збереження значень у sysfs
static ssize_t param1_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", param1);
}

static ssize_t param2_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%s\n", param2);
}

static ssize_t param1_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    sscanf(buf, "%d", &param1);
    return count;
}

static ssize_t param2_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    sscanf(buf, "%s", param2);
    return count;
}

// Створення атрибутів
static struct kobj_attribute param1_attribute = __ATTR(param1, 0660, param1_show, param1_store);
static struct kobj_attribute param2_attribute = __ATTR(param2, 0660, param2_show, param2_store);

static int __init my_module_init(void) {
    int error = 0;

    // Створення об'єкта в sysfs
    kobj_ref = kobject_create_and_add("my_module", kernel_kobj);
    if (!kobj_ref) {
        pr_err("Failed to create kobject\n");
        return -ENOMEM;
    }

    // Додавання атрибутів
    error = sysfs_create_file(kobj_ref, &param1_attribute.attr);
    if (error) {
        pr_err("Failed to create param1 attribute\n");
        kobject_put(kobj_ref);
        return error;
    }

    error = sysfs_create_file(kobj_ref, &param2_attribute.attr);
    if (error) {
        pr_err("Failed to create param2 attribute\n");
        sysfs_remove_file(kobj_ref, &param1_attribute.attr);
        kobject_put(kobj_ref);
        return error;
    }

    pr_info("Module loaded with param1=%d, param2=%s\n", param1, param2);
    return 0;
}

static void __exit my_module_exit(void) {
    // Видалення атрибутів та об'єкта
    sysfs_remove_file(kobj_ref, &param1_attribute.attr);
    sysfs_remove_file(kobj_ref, &param2_attribute.attr);
    kobject_put(kobj_ref);

    pr_info("Module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

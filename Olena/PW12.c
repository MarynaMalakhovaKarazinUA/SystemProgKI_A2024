#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "simple_char_device"
#define BUFFER_SIZE 1024

static int major;                   // Основний номер пристрою
static struct cdev my_cdev;         // Структура символічного пристрою
static char *device_buffer;         // Буфер для збереження даних

// Відкриття пристрою:
static int device_open(struct inode *inode, struct file *file) {
    pr_info("Device opened\n");
    return 0;
}

// Закриття пристрою:
static int device_release(struct inode *inode, struct file *file) {
    pr_info("Device closed\n");
    return 0;
}

// Читання даних з пристрою:
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t count, loff_t *offset) {
    size_t bytes_to_read = min_t(size_t, count, BUFFER_SIZE - *offset);
    if (bytes_to_read == 0) return 0;

    if (copy_to_user(user_buffer, device_buffer + *offset, bytes_to_read)) {
        return -EFAULT;
    }

    *offset += bytes_to_read;
    pr_info("Read %zu bytes\n", bytes_to_read);
    return bytes_to_read;
}

// Запис даних у пристрій:
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *offset) {
    size_t bytes_to_write = min_t(size_t, count, BUFFER_SIZE - *offset);
    if (bytes_to_write == 0) return -ENOSPC;

    if (copy_from_user(device_buffer + *offset, user_buffer, bytes_to_write)) {
        return -EFAULT;
    }

    *offset += bytes_to_write;
    pr_info("Wrote %zu bytes\n", bytes_to_write);
    return bytes_to_write;
}

// Структура file_operations:
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

// Функція ініціалізації:
static int __init char_device_init(void) {
    dev_t dev;
    int ret;

    device_buffer = kzalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!device_buffer) {
        pr_err("Failed to allocate memory\n");
        return -ENOMEM;
    }

    ret = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Failed to allocate major number\n");
        kfree(device_buffer);
        return ret;
    }

    major = MAJOR(dev);
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;

    ret = cdev_add(&my_cdev, dev, 1);
    if (ret < 0) {
        pr_err("Failed to add cdev\n");
        unregister_chrdev_region(dev, 1);
        kfree(device_buffer);
        return ret;
    }

    pr_info("Char device registered with major %d\n", major);
    return 0;
}

// Функція очищення:
static void __exit char_device_exit(void) {
    cdev_del(&my_cdev);
    unregister_chrdev_region(MKDEV(major, 0), 1);
    kfree(device_buffer);
    pr_info("Char device unregistered\n");
}

// Реєстрація модуля
module_init(char_device_init);
module_exit(char_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Character Device Driver");
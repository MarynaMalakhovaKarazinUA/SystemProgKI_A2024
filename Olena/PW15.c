#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/mutex.h>

static struct task_struct *inc_thread; // Потік для інкременту
static struct task_struct *dec_thread; // Потік для декременту

static int counter = 0;               // Глобальна змінна
static struct mutex counter_mutex;    // М'ютекс для синхронізації

// Функція для потоку інкремента
static int increment_fn(void *data) {
    while (!kthread_should_stop()) {
        msleep(500); // Затримка 500 мс
        mutex_lock(&counter_mutex);
        counter++;
        pr_info("Increment Thread: Counter = %d\n", counter);
        mutex_unlock(&counter_mutex);
    }
    pr_info("Increment Thread stopping...\n");
    return 0;
}

// Функція для потоку декремента
static int decrement_fn(void *data) {
    while (!kthread_should_stop()) {
        msleep(500); // Затримка 500 мс
        mutex_lock(&counter_mutex);
        counter--;
        pr_info("Decrement Thread: Counter = %d\n", counter);
        mutex_unlock(&counter_mutex);
    }
    pr_info("Decrement Thread stopping...\n");
    return 0;
}

// Функція ініціалізації модуля
static int __init my_module_init(void) {
    pr_info("Loading Kernel Module\n");
    
    mutex_init(&counter_mutex); // Ініціалізація м'ютекса

    // Створення потоків
    inc_thread = kthread_run(increment_fn, NULL, "increment_thread");
    if (IS_ERR(inc_thread)) {
        pr_err("Failed to create increment thread\n");
        return PTR_ERR(inc_thread);
    }

    dec_thread = kthread_run(decrement_fn, NULL, "decrement_thread");
    if (IS_ERR(dec_thread)) {
        pr_err("Failed to create decrement thread\n");
        kthread_stop(inc_thread); // Зупиняємо інший потік
        return PTR_ERR(dec_thread);
    }

    pr_info("Kernel Threads created successfully\n");
    return 0;
}

// Функція завершення модуля
static void __exit my_module_exit(void) {
    if (inc_thread)
        kthread_stop(inc_thread); // Зупинка потоку інкремента
    if (dec_thread)
        kthread_stop(dec_thread); // Зупинка потоку декремента
    
    pr_info("Kernel Module Unloaded\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Olena Plakhotia");
MODULE_DESCRIPTION("Kernel module with synchronized threads");

module_init(my_module_init);
module_exit(my_module_exit);
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/time.h>

static struct task_struct *thread_st; // Змінна для нитки
static struct timespec64 start_time; // Час завантаження модуля

// Функція, що виконується ниткою
static int thread_fn(void *data) {
    while (!kthread_should_stop()) {
        pr_info("Kthread is running...\n");
        ssleep(5); // Затримка в секундах
    }
    pr_info("Kthread stopping...\n");
    return 0;
}

// Ініціалізація модуля
static int __init my_module_init(void) {
    struct timespec64 now;

    // Записуємо час завантаження модуля
    ktime_get_real_ts64(&start_time);
    ktime_get_real_ts64(&now);

    pr_info("Hello, World! Module loaded at: %llu.%09lu seconds since epoch\n", 
            (unsigned long long)now.tv_sec, now.tv_nsec);

    // Створення нитки
    thread_st = kthread_run(thread_fn, NULL, "my_kthread");
    if (IS_ERR(thread_st)) {
        pr_err("Failed to create kthread\n");
        return PTR_ERR(thread_st);
    }

    pr_info("Kthread created successfully\n");

    return 0;
}

// Завершення роботи модуля
static void __exit my_module_exit(void) {
    struct timespec64 now;
    struct timespec64 elapsed;

    ktime_get_real_ts64(&now);

    // Розрахунок часу, що минув з моменту завантаження
    elapsed.tv_sec = now.tv_sec - start_time.tv_sec;
    elapsed.tv_nsec = now.tv_nsec - start_time.tv_nsec;
    if (elapsed.tv_nsec < 0) {
        elapsed.tv_nsec += 1000000000;
        elapsed.tv_sec -= 1;
    }

    pr_info("Goodbye, World! Module unloaded after: %llu.%09lu seconds\n", 
            (unsigned long long)elapsed.tv_sec, elapsed.tv_nsec);

    if (thread_st) {
        kthread_stop(thread_st); // Зупиняємо нитку
        pr_info("Kthread stopped\n");
    }
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Olena Plakhotia");
MODULE_DESCRIPTION("A simple Hello World Module with timestamp logging");

module_init(my_module_init);
module_exit(my_module_exit);
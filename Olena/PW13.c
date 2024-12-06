#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/vmalloc.h>  // Додано для vmalloc і vfree

// Оголошення змінної для нитки
static struct task_struct *thread_st;

// Функція для вимірювання часу
static void measure_time(void) {
    unsigned long long start_time, end_time, time_diff;
    size_t block_size;
    void *memory_block;

    // Проходимо по блоках пам'яті 2^n для n від 1 до 64
    for (int n = 1; n <= 64; n++) {
        block_size = 1 << n;  // 2^n

        // Вимірюємо час для kmalloc
        start_time = ktime_get_ns();
        memory_block = kmalloc(block_size, GFP_KERNEL);
        end_time = ktime_get_ns();
        if (memory_block) {
            kfree(memory_block);
            time_diff = end_time - start_time;
            pr_info("kmalloc: Size %zu bytes, Time: %llu ns\n", block_size, time_diff);
        }

        // Вимірюємо час для kzalloc (і нулювання пам'яті)
        start_time = ktime_get_ns();
        memory_block = kzalloc(block_size, GFP_KERNEL);
        end_time = ktime_get_ns();
        if (memory_block) {
            kfree(memory_block);
            time_diff = end_time - start_time;
            pr_info("kzalloc: Size %zu bytes, Time: %llu ns\n", block_size, time_diff);
        }

        // Вимірюємо час для vmalloc
        start_time = ktime_get_ns();
        memory_block = vmalloc(block_size);
        end_time = ktime_get_ns();
        if (memory_block) {
            vfree(memory_block);
            time_diff = end_time - start_time;
            pr_info("vmalloc: Size %zu bytes, Time: %llu ns\n", block_size, time_diff);
        }
    }
}

// Функція, що виконується ниткою
static int thread_fn(void *data) {
    while (!kthread_should_stop()) {
       // pr_info("Kthread is running...\n");
       // ssleep(5); // Затримка в секундах

        // Вимірювання часу для всіх операцій алокації
        measure_time();
    }
    pr_info("Kthread stopping...\n");
    return 0;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Olena Plakhotia");
MODULE_DESCRIPTION("A simple Hello World Module");

static int __init my_module_init(void) {
    printk(KERN_INFO "Hello, World!\n");
    // Створення нитки
    thread_st = kthread_run(thread_fn, NULL, "my_kthread");
    if (IS_ERR(thread_st)) {
        pr_err("Failed to create kthread\n");
        return PTR_ERR(thread_st);
    }

    pr_info("Kthread created successfully\n");

    return 0;
}

static void __exit my_module_exit(void) {
    printk(KERN_INFO "Goodbye, World!\n");
    if (thread_st) {
        kthread_stop(thread_st); // Зупиняємо нитку
        pr_info("Kthread stopped\n");
    }
}

module_init(my_module_init);
module_exit(my_module_exit);
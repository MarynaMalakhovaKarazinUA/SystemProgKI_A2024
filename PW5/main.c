#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void set_variable(char *arg) {
    char *key_value = strdup(arg);
    char *equal_sign = strchr(key_value, '=');
    
    if (equal_sign) {
        *equal_sign = '\0';  // Розділити ключ і значення
        char *value = equal_sign + 1;
        if (setenv(key_value, value, 1) == 0) {
            printf("Змінна %s встановлена зі значенням %s\n", key_value, value);
        } else {
            perror("Не вдалося встановити змінну");
        }
    } else {
        printf("Неправильний формат: потрібно вказати MY_VAR=VALUE\n");
    }
    free(key_value);
}

void get_variable(char *key) {
    char *value = getenv(key);
    if (value) {
        printf("%s=%s\n", key, value);
    } else {
        printf("Змінна %s не знайдена\n", key);
    }
}

void print_pid() {
    printf("PID: %d\n", getpid());
}

void print_ppid() {
    printf("PPID: %d\n", getppid());
}

void print_addresses() {
    int dummy_variable; // Локальна змінна для демонстрації адреси
    printf("Адреси сегментів (приблизно):\n");
    printf("Адреса TEXT сегмента (функція main): %p\n", (void*)&dummy_variable); // Для демонстрації
}

void print_uid() {
    printf("UID: %d\n", getuid());
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Використання: %s [-s|--set VAR=VALUE] [-g|--get VAR] [-p|--pid] [-r|--ppid] [-a|--addresses] [-u|--uid]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--set") == 0) {
        if (argc < 3) {
            printf("Вказати змінну для встановлення\n");
            return 1;
        }
        set_variable(argv[2]);
    } else if (strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "--get") == 0) {
        if (argc < 3) {
            printf("Вказати змінну для отримання\n");
            return 1;
        }
        get_variable(argv[2]);
    } else if (strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "--pid") == 0) {
        print_pid();
    } else if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--ppid") == 0) {
        print_ppid();
    } else if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--addresses") == 0) {
        print_addresses();
    } else if (strcmp(argv[1], "-u") == 0 || strcmp(argv[1], "--uid") == 0) {
        print_uid();
    } else {
        printf("Невідома команда: %s\n", argv[1]);
        return 1;
    }

    return 0;
}

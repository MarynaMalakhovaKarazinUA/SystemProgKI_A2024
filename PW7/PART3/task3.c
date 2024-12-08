#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler1(void) {
    printf("Обробник 1 викликаний за допомогою atexit\n");
}

void handler2(void) {
    printf("Обробник 2 викликаний за допомогою atexit\n");
}

void on_exit_handler(int status, void *arg) {
    printf("Обробник on_exit викликаний з статусом %d і аргументом %s\n", status, (char *)arg);
}

int main() {
    if (atexit(handler1) != 0) {
        perror("Не вдалося зареєструвати handler1");
        return -1;
    }
    if (atexit(handler2) != 0) {
        perror("Не вдалося зареєструвати handler2");
        return -1;
    }
    if (on_exit(on_exit_handler, "Аргумент для on_exit") != 0) {
        perror("Не вдалося зареєструвати on_exit_handler");
        return -1;
    }
    printf("Завершення програми...\n");
    exit(0);
}
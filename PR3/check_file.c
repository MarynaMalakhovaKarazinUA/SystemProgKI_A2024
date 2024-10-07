#include <stdio.h>
#include <unistd.h>  // Для системного виклику access
#include <errno.h>   // Для обробки помилок

int main(int argc, char *argv[]) {
    // Перевіряємо, що користувач передав аргумент (ім'я файлу)
    if (argc < 2) {
        printf("Будь ласка, вкажіть ім'я файлу для перевірки.\n");
        return 1;
    }

    // Перевіряємо, чи існує файл
    if (access(argv[1], F_OK) == 0) {
        printf("Файл '%s' існує.\n", argv[1]);
    } else {
        // Якщо файл не існує, виводимо повідомлення про помилку
        printf("Файл '%s' не існує. Помилка: %d\n", argv[1], errno);
    }

    return 0;
}

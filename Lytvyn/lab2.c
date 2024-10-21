#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>  // Додано для використання strlen

int main() {
    char filename[100];
    char text[1000];
    int file_descriptor;

    // Запитуємо у користувача ім'я файлу
    printf("Введіть ім'я файлу для додавання тексту: ");
    scanf("%99s", filename); // Використовуємо %99s для запобігання переповнення буфера

    // Перевіряємо, чи існує файл
    struct stat st;
    if (stat(filename, &st) != 0) {
        perror("Помилка перевірки файлу");
        return 1;
    }

    // Відкриваємо файл для додавання тексту
    file_descriptor = open(filename, O_WRONLY | O_APPEND);
    if (file_descriptor == -1) {
        perror("Помилка відкриття файлу");
        return 1;
    }

    // Запитуємо у користувача текст для додавання
    printf("Введіть текст для додавання у файл (для завершення введіть порожній рядок):\n");
    getchar(); // Очищуємо буфер введення

    while (1) {
        fgets(text, sizeof(text), stdin);
        if (text[0] == '\n') {
            break; // Завершуємо введення, якщо користувач ввів порожній рядок
        }
        write(file_descriptor, text, strlen(text));
    }

    // Закриваємо файл
    close(file_descriptor);

    printf("Текст успішно додано до файлу %s\n", filename);

    return 0;
}


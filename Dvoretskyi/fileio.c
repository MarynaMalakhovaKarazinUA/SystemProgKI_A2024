#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 1024

void print_usage(const char *program_name) {
    printf("Використання: %s [опції] <вихідний_файл> <цільовий_файл>\n", program_name);
    printf("Опції:\n");
    printf("  -a      Додати до цільового файлу замість перезапису\n");
    printf("  -v      Вивести детальну інформацію про процес копіювання\n");
    printf("  -h      Вивести цю допомогу\n");
}

int main(int argc, char *argv[]) {
    int source_fd, destination_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead, bytesWritten;
    int append_flag = 0;  // Флаг додавання до файлу
    int verbose_flag = 0; // Флаг детального виведення
    int opt;

    // Обробка ключів
    while ((opt = getopt(argc, argv, "avh")) != -1) {
        switch (opt) {
            case 'a':
                append_flag = 1;
                break;
            case 'v':
                verbose_flag = 1;
                break;
            case 'h':
                print_usage(argv[0]);
                return EXIT_SUCCESS;
            default:
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    // Після ключів повинно залишитися два аргументи: вихідний і цільовий файл
    if (optind + 2 != argc) {
        fprintf(stderr, "Помилка: неправильна кількість аргументів\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    char *source_file = argv[optind];
    char *destination_file = argv[optind + 1];

    // Відкриття вихідного файлу для читання
    source_fd = open(source_file, O_RDONLY);
    if (source_fd == -1) {
        perror("Помилка відкриття вихідного файлу");
        return EXIT_FAILURE;
    }

    // Відкриття цільового файлу для запису
    int flags = O_WRONLY | O_CREAT;
    if (append_flag) {
        flags |= O_APPEND; // Додавання до файлу
    } else {
        flags |= O_TRUNC;  // Перезапис файлу
    }

    destination_fd = open(destination_file, flags, 0644);
    if (destination_fd == -1) {
        perror("Помилка відкриття цільового файлу");
        close(source_fd);
        return EXIT_FAILURE;
    }

    // Копіювання вмісту вихідного файлу в цільовий файл
    while ((bytesRead = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        bytesWritten = write(destination_fd, buffer, bytesRead);
        if (bytesWritten == -1) {
            perror("Помилка запису до цільового файлу");
            close(source_fd);
            close(destination_fd);
            return EXIT_FAILURE;
        }
        if (verbose_flag) {
            printf("Копійовано %ld байтів...\n", (long)bytesWritten);
        }
    }

    if (bytesRead == -1) {
        perror("Помилка читання вихідного файлу");
    }

    // Закриття файлів
    close(source_fd);
    close(destination_fd);

    if (verbose_flag) {
        printf("Файл '%s' успішно скопійовано в '%s'\n", source_file, destination_file);
    }

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// Функція для демонстрації зміни змінних у дочірньому процесі
void demonstrate_fork() {
    int variable = 42;
    pid_t pid = fork();

    if (pid == 0) { // Дочірній процес
        variable = 100;
        printf("Дочірній процес: змінна = %d\n", variable);
        exit(0);
    } else { // Батьківський процес
        wait(NULL); // Очікуємо завершення дочірнього процесу
        printf("Батьківський процес: змінна = %d\n", variable);
    }
}

// Функція для демонстрації успадкування файлових дескрипторів
void demonstrate_file_descriptors() {
    int fd = open("testfile.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == 0) { // Дочірній процес
        lseek(fd, 0, SEEK_END);
        write(fd, "Додано дочірнім процесом\n", 26);
        close(fd);
        exit(0);
    } else { // Батьківський процес
        wait(NULL); // Очікуємо завершення дочірнього процесу
        lseek(fd, 0, SEEK_SET);
        char buffer[100];
        read(fd, buffer, sizeof(buffer));
        printf("Батьківський процес прочитав:\n%s", buffer);
        close(fd);
    }
}

// Обробники виходу
void exit_handler1() {
    printf("Обробник виходу 1\n");
}

void exit_handler2(int status, void *arg) {
    printf("Обробник виходу 2\n");
}

int main() {
    // Реєстрація обробників виходу
    atexit(exit_handler1);
    on_exit(exit_handler2, NULL);

    demonstrate_fork();
    demonstrate_file_descriptors();

    return 0;
}

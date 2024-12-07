#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("example.txt", O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("File open failed");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) { // Дочірній процес
        write(fd, "Child writes\n", 13);
        lseek(fd, 0, SEEK_SET); // Змінюємо позицію в файлі
        write(fd, "Child overwrite\n", 16);
        printf("Child: File modified\n");
    } else { // Батьківський процес
        wait(NULL); // Очікуємо завершення дочірнього процесу
        write(fd, "Parent writes\n", 14);
        printf("Parent: File modified\n");
    }
    close(fd);
    return 0;
}

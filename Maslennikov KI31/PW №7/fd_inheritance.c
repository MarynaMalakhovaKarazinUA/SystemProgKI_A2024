#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("test.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    write(fd, "Hello, World!\n", 14);
    lseek(fd, 0, SEEK_SET);

    pid_t pid = fork();

    if (pid == 0) {
        // Дочірній процес
        char buffer[100];
        read(fd, buffer, 14);
        printf("Child Process read: %s", buffer);
        lseek(fd, 0, SEEK_SET);
        write(fd, "Child Process\n", 15);
    } else if (pid > 0) {
        // Батьківський процес
        sleep(1); // Чекаємо на дочірній процес
        lseek(fd, 0, SEEK_SET);
        char buffer[100];
        read(fd, buffer, 15);
        printf("Parent Process read: %s", buffer);
    } else {
        perror("fork failed");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

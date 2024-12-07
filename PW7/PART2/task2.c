#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *file_name = "example.txt";
    char buf[] = "Hello from the child process!";
    char read_buf[50];

    int fd = open(file_name, O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        printf("Дочірній процес (pid=%d): перед записом у файл\n", getpid());
        write(fd, buf, sizeof(buf) - 1);
        close(fd);
        exit(0);
    } else {
        sleep(1);
        printf("Батьківський процес (pid=%d): перед зміною позиції в файлі\n", getpid());
        lseek(fd, 0, SEEK_SET);
        read(fd, read_buf, sizeof(read_buf) - 1);
        printf("Батьківський процес (pid=%d): вміст файлу: %s\n", getpid(), read_buf);
        close(fd);
    }
}

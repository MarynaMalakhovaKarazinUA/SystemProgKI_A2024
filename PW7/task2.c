#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h> // Додано для wait()

int main() {
    int fd = open("testfile.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open failed");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {  // Дочірній процес
        lseek(fd, 0, SEEK_SET);
        write(fd, "Child process\n", 14);
        printf("Child wrote to file.\n");
        close(fd);
        exit(0);
    } else {  // Батьківський процес
        wait(NULL);  // Чекаємо на дочірній процес
        char buffer[100];
        lseek(fd, 0, SEEK_SET);
        read(fd, buffer, sizeof(buffer));
        printf("Parent read from file: %s\n", buffer);
    }

    close(fd);
    return 0;
}

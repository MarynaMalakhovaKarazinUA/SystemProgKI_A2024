#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    int fd = open("example.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    write(fd, "Hello, world!\n", 14);

    printf("Before fork: File descriptor = %d\n", fd);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        close(fd);
        return 1;
    }

    if (pid == 0) {
        printf("Child process: PID = %d\n", getpid());

        lseek(fd, 0, SEEK_SET); 
        write(fd, "CHILD ", 6); 

        printf("Child process: File position after write = %ld\n", lseek(fd, 0, SEEK_CUR));
    } else {
        
        sleep(1); 
        printf("Parent process: PID = %d\n", getpid());

        
        char buffer[20];
        lseek(fd, 0, SEEK_SET); 
        read(fd, buffer, 14);
        buffer[14] = '\0';

        printf("Parent process: File content = \"%s\"\n", buffer);
    }

    close(fd);
    return 0;
}
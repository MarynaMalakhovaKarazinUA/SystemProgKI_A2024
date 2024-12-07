#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int var1 = 5;
    int var2 = 10;

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        printf("Дочірній процес (pid=%d): перед зміною змінних var1 = %d, var2 = %d\n", getpid(), var1, var2);
        var1 = 100;
        var2 = 200;
        printf("Дочірній процес (pid=%d): після зміни змінних var1 = %d, var2 = %d\n", getpid(), var1, var2);
        exit(0);
    }
    else {
        wait(NULL);
        printf("Батьківський процес (pid=%d): після завершення дочірнього процесу var1 = %d, var2 = %d\n", getpid(), var1, var2);
    }
}

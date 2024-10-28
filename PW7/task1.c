#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // Додано для wait()

int global_var = 0;

int main() {
    int local_var = 0;

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {  // Дочірній процес
        global_var = 1;
        local_var = 1;
        printf("Child process:\n");
        printf("Global variable: %d\n", global_var);
        printf("Local variable: %d\n", local_var);
    } else {  // Батьківський процес
        wait(NULL);  // Чекаємо на дочірній процес
        printf("Parent process:\n");
        printf("Global variable: %d\n", global_var);
        printf("Local variable: %d\n", local_var);
    }

    return 0;
}

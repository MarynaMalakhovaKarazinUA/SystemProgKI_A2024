#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_var = 10; // Змінна в сегменті даних

void child_process() {
    int local_var = 20; // Змінна в стеку
    global_var++;
    local_var++;
    printf("Child Process:\n");
    printf("  Global Var: %d\n", global_var);
    printf("  Local Var: %d\n", local_var);
}

void parent_process() {
    printf("Parent Process:\n");
    printf("  Global Var: %d\n", global_var);
}

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Дочірній процес
        child_process();
    } else if (pid > 0) {
        // Батьківський процес
        sleep(1); // Чекаємо на дочірній процес
        parent_process();
    } else {
        perror("fork failed");
        return 1;
    }

    return 0;
}

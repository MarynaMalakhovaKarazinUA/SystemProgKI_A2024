#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int global_var = 10; 

int main() {
    int stack_var = 20; 

    printf("Before fork: PID=%d, global_var=%d, stack_var=%d\n", getpid(), global_var, stack_var);

    pid_t pid = fork(); 

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        printf("Child process: PID=%d\n", getpid());

        global_var += 5;
        stack_var += 10;

        printf("Child process after modification: global_var=%d, stack_var=%d\n", global_var, stack_var);
    } else {
        printf("Parent process: PID=%d\n", getpid());

        sleep(1); 

        printf("Parent process after child modification: global_var=%d, stack_var=%d\n", global_var, stack_var);
    }

    return 0;
}
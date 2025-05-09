#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_var = 0; // Глобальна змінна (сегмент даних)

int main() {
    int local_var = 0; // Локальна змінна (стек)

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) { // Дочірній процес
        global_var = 100;
        local_var = 100;
        printf("Child: global_var = %d, local_var = %d\n", global_var, local_var);
    } else { // Батьківський процес
        sleep(1); // Очікуємо, щоб дочірній процес завершився першим
        printf("Parent: global_var = %d, local_var = %d\n", global_var, local_var);
    }
    return 0;
}

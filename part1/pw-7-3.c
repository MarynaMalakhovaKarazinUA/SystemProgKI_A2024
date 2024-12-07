#include <stdio.h>
#include <stdlib.h>

void atexit_handler1(void) {
    printf("atexit handler 1\n");
}

void atexit_handler2(void) {
    printf("atexit handler 2\n");
}

void on_exit_handler(int status, void *arg) {
    printf("on_exit handler: status=%d, arg=%s\n", status, (char *)arg);
}

int main() {
    if (atexit(atexit_handler1) != 0) {
        perror("Failed to set atexit handler 1");
        exit(1);
    }

    if (atexit(atexit_handler2) != 0) {
        perror("Failed to set atexit handler 2");
        exit(1);
    }

    if (on_exit(on_exit_handler, "Exit argument") != 0) {
        perror("Failed to set on_exit handler");
        exit(1);
    }

    printf("Main program ends\n");
    return 0;
}

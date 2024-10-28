#include <stdio.h>
#include <stdlib.h>

void exit_handler1() {
    printf("Exit handler 1 called.\n");
}

void exit_handler2() {
    printf("Exit handler 2 called.\n");
}

int main() {
    atexit(exit_handler1);
    atexit(exit_handler2);

    printf("Program is running...\n");
    exit(0);
}

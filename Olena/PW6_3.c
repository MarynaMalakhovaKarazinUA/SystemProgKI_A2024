#include <stdio.h>
#include <stdlib.h>

void handlerAtexit1() {
    printf("Handler registered with atexit 1 called.\n");
}

void handlerAtexit2() {
    printf("Handler registered with atexit 2 called.\n");
}

void handlerOnExit1(int exit_status, void *arg) {
    printf("Handler registered with on_exit 1 called. Exit status: %d, Arg: %s\n", exit_status, (char *)arg);
}

void handlerOnExit2(int exit_status, void *arg) {
    printf("Handler registered with on_exit 2 called. Exit status: %d, Arg: %s\n", exit_status, (char *)arg);
}

int main() {
    printf("Program starts.\n");

    atexit(handlerAtexit1);
    atexit(handlerAtexit2);

    on_exit(handlerOnExit1, "First argument");
    on_exit(handlerOnExit2, "Second argument");

    printf("Exiting program...\n");

    exit(0);
}
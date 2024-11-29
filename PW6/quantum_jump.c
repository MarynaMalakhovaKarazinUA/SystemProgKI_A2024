#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>

jmp_buf env; // Global environment for setjmp/longjmp
volatile int state = 0; // State variable to control flow

void functionB();

void functionA() {
    printf("In functionA: Current state = %d\n", state);

    if (state == 0) {
        state = 1; // Change state to jump to functionB
        longjmp(env, 1); // Jump to functionB
    }

    printf("Exiting functionA.\n");
}

void functionB() {
    printf("In functionB: Current state = %d\n", state);

    if (state == 1) {
        state = 2; // Change state to jump back to functionA
        longjmp(env, 2); // Jump to functionA
    }

    printf("Exiting functionB.\n");
}

int main() {
    printf("Starting program.\n");

    int ret = setjmp(env); // Save environment
    if (ret == 0) {
        // First call
        functionA();
    } else if (ret == 1) {
        // Return from functionA to functionB
        functionB();
    } else if (ret == 2) {
        // Return from functionB to functionA
        printf("Back in main: State = %d\n", state);
        printf("Program completed.\n");
        return 0;
    }

    printf("Unexpected state.\n");
    return 1;
}

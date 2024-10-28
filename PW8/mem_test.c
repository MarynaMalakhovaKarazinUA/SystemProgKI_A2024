#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void check_program_break(const char *label) {
    void *current_break = sbrk(0);
    printf("%s: Program break at %p\n", label, current_break);
}

int main() {
    check_program_break("Start");

    // malloc(0) test
    void *ptr = malloc(0);
    if (ptr == NULL) {
        printf("malloc(0) returned NULL\n");
    } else {
        printf("malloc(0) returned a pointer: %p\n", ptr);
    }
    check_program_break("After malloc(0)");

    // Memory allocation in powers of 2
    for (int n = 1; n <= 20; n++) { // Example for smaller values; 64 can be overwhelming
        size_t size = 1 << n;
        void *block = malloc(size);
        printf("Allocated %zu bytes at %p\n", size, block);
        check_program_break("After malloc block");

        // Optional: Uncomment for free
        free(block);
        check_program_break("After free block");
    }

    // Memory leak creation
    for (int n = 1; n <= 10; n++) {
        malloc(1024); // Leak some memory
    }

    check_program_break("End");
    return 0;
}

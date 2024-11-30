#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>

void check_program_break() {
    void *brk = sbrk(0);
    printf("Program break: %p\n", brk);
}

void allocate_memory() {
    for (int n = 1; n <= 64; n++) {
        size_t size = 1 << n;
        void *ptr = malloc(size);
        if (ptr == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        printf("Allocated %zu bytes at %p\n", size, ptr);
        free(ptr);
    }
}

void allocate_memory_calloc() {
    for (int n = 1; n <= 64; n++) {
        size_t size = 1 << n;
        void *ptr = calloc(1, size);
        if (ptr == NULL) {
            perror("calloc");
            exit(EXIT_FAILURE);
        }
        printf("Allocated %zu bytes at %p\n", size, ptr);
        free(ptr);
    }
}

void allocate_memory_realloc() {
    void *ptr = malloc(1);
    for (int n = 1; n <= 64; n++) {
        size_t size = 1 << n;
        ptr = realloc(ptr, size);
        if (ptr == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        printf("Reallocated to %zu bytes at %p\n", size, ptr);
    }
    free(ptr);
}

void allocate_memory_alloca() {
    for (int n = 1; n <= 64; n++) {
        size_t size = 1 << n;
        void *ptr = alloca(size);
        printf("Allocated %zu bytes on stack at %p\n", size, ptr);
    }
}

int main() {
    printf("Initial ");
    check_program_break();

    printf("\nTesting malloc(0):\n");
    void *ptr = malloc(0);
    printf("malloc(0) returned %p\n", ptr);
    check_program_break();

    printf("\nAllocating memory with malloc:\n");
    allocate_memory();
    check_program_break();

    printf("\nAllocating memory with calloc:\n");
    allocate_memory_calloc();
    check_program_break();

    printf("\nAllocating memory with realloc:\n");
    allocate_memory_realloc();
    check_program_break();

    printf("\nAllocating memory with alloca:\n");
    allocate_memory_alloca();
    check_program_break();

    return 0;
}

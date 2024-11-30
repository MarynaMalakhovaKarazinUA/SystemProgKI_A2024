#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Для sbrk
#include <stdint.h>
#include <malloc.h> // Для malloc_trim

#define MAX_BLOCKS 10

void track_heap_top(void) {
    printf("Heap top: %p\n", sbrk(0));
}

int main() {
    printf("Initial heap state:\n");
    track_heap_top();

    // Виділення пам’яті за допомогою malloc
    void *blocks[MAX_BLOCKS];
    for (int i = 0; i < MAX_BLOCKS; i++) {
        size_t size = 1 << (i + 1); // Розмір блоку 2^n
        blocks[i] = malloc(size);
        if (blocks[i] == NULL) {
            printf("Failed to allocate %zu bytes.\n", size);
        } else {
            printf("Allocated %zu bytes at %p.\n", size, blocks[i]);
        }
        track_heap_top();
    }

    // Звільнення пам’яті
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i]) {
            free(blocks[i]);
            printf("Freed block at %p.\n", blocks[i]);
            track_heap_top();
        }
    }

    // Повторне використання malloc
    printf("Reallocating memory to check reuse:\n");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        blocks[i] = malloc(1 << (i + 1));
        if (blocks[i]) {
            printf("Reallocated block at %p.\n", blocks[i]);
        }
    }

    // Використання calloc
    printf("Using calloc:\n");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        void *ptr = calloc(1, 1 << (i + 1));
        printf("Calloc'd block of size %zu at %p.\n", (size_t)(1 << (i + 1)), ptr);
        free(ptr);
    }

    // Використання alloca
    printf("Using alloca:\n");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        void *ptr = alloca(1 << (i + 1));
        printf("Allocated on stack: %p (size %zu).\n", ptr, (size_t)(1 << (i + 1)));
    }

    printf("Heap after testing:\n");
    track_heap_top();

    // Чистка купи (heap trimming)
    malloc_trim(0);
    printf("Heap after trimming:\n");
    track_heap_top();

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>

void* allocate_memory(size_t size) {
    void* ptr = malloc(size);
    if (ptr != NULL) {
        printf("Allocated %zu bytes using malloc at %p\n", size, ptr);
    } else {
        printf("malloc failed\n");
    }
    return ptr;
}

void* allocate_cleared_memory(size_t size) {
    void* ptr = calloc(1, size);
    if (ptr != NULL) {
        printf("Allocated %zu bytes using calloc at %p\n", size, ptr);
    } else {
        printf("calloc failed\n");
    }
    return ptr;
}

void* reallocate_memory(void* ptr, size_t new_size) {
    void* new_ptr = realloc(ptr, new_size);
    if (new_ptr != NULL) {
        printf("Reallocated memory to %zu bytes at %p\n", new_size, new_ptr);
    } else {
        printf("realloc failed\n");
    }
    return new_ptr;
}

void* allocate_on_stack(size_t size) {
    void* ptr = alloca(size);
    printf("Allocated %zu bytes on stack at %p\n", size, ptr);
    return ptr;
}

void track_heap_top() {
    struct mallinfo info = mallinfo();
    printf("Heap top (arena): %zu bytes\n", info.arena);
    printf("Free blocks: %zu\n", info.ordblks);
    printf("Allocated space: %zu bytes\n", info.uordblks);
    printf("Free space: %zu bytes\n", info.fordblks);
}

int main() {
    void* ptrs[64];
    size_t i;

    track_heap_top();

    for (i = 1; i <= 64; ++i) {
        size_t block_size = 1UL << i;  // 2^n
        ptrs[i-1] = allocate_memory(block_size);
        track_heap_top();
    }

    for (i = 0; i < 64; ++i) {
        if (ptrs[i] != NULL) {
            free(ptrs[i]);
            printf("Freed memory at %p\n", ptrs[i]);
            track_heap_top();
        }
    }

    void* ptr = allocate_memory(64);
    free(ptr);
    track_heap_top();

    ptr = realloc(ptr, 128);
    if (ptr) {
        printf("Reallocation successful\n");
    } else {
        printf("Reallocation failed\n");
    }
    track_heap_top();

    for (i = 1; i <= 64; ++i) {
        size_t block_size = 1UL << i;
        allocate_on_stack(block_size);
        track_heap_top();
    }

    return 0;
}
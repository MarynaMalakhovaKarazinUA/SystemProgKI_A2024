#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_POW 64

void check_heap() {
    // Використовуємо sbrk() для перевірки верхівки кучі
    void *heap_top = sbrk(0);
    printf("Current heap top: %p\n", heap_top);
}

void allocate_memory() {
    size_t size;
    void *ptr[MAX_POW];
    size_t allocated_size = 0;

    for (int n = 1; n <= MAX_POW; n++) {
        size = 1UL << n; // 2^n
        printf("Allocating %zu bytes\n", size);
        
        // Використання malloc
        ptr[n-1] = malloc(size);
        if (ptr[n-1] == NULL) {
            perror("malloc failed");
            break;
        }
        allocated_size += size;
        
        // Перевірка верхівки кучі
        check_heap();
        
        // Звільнення пам'яті
        free(ptr[n-1]);
        printf("Freed %zu bytes\n", size);
        
        // Перевірка верхівки кучі після звільнення
        check_heap();
    }
}

int main() {
    printf("Memory Allocation and Deallocation Demo\n");
    allocate_memory();
    return 0;
}

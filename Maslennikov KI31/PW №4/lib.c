#include <stdio.h>

int shared_value = 10;

void print_value() {
    printf("Shared value: %d\n", shared_value);
}

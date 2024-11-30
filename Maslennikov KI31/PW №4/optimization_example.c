#include <stdio.h>

int main() {
    int sum = 0;
    for (int i = 0; i < 1000000; i++) {
        if (i % 2 == 0) {
            sum += i;
        }
    }
    printf("Sum: %d\n", sum);
    return 0;
}

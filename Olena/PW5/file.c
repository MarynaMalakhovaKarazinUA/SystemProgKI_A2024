#include <stdio.h>

// Прототипи функцій
void firstFunction();
void secondFunction();
void recursiveFunction(int depth);

int main() {
    printf("Program starts.\n");

    // Виклик першої функції
    firstFunction();

    printf("Program ends.\n");
    return 0;
}

void firstFunction() {
    printf("In firstFunction().\n");

    // Використовуємо goto для стрибка в другу функцію
    goto jumpToSecondFunction;

jumpToSecondFunction:
    secondFunction();
}

void secondFunction() {
    printf("In secondFunction().\n");

    // Використовуємо goto для стрибка до рекурсивної функції
    goto jumpToRecursive;

jumpToRecursive:
    recursiveFunction(3); // Починаємо рекурсивний виклик
}

void recursiveFunction(int depth) {
    printf("In recursiveFunction(), depth: %d.\n", depth);

    if (depth > 1) {
        recursiveFunction(depth - 1); // Рекурсивний виклик
    } else {
        // Використовуємо goto для стрибка до попереднього рекурсивного рівня
        printf("Base case reached, jumping to previous recursive level.\n");
        goto exitRecursive;
    }

exitRecursive:
    printf("Exiting recursiveFunction(), depth: %d.\n", depth);
}

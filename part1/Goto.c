#include <stdio.h>

// Оголошення функцій
void funcA();
void funcB();

int main() {
    printf("У головній функції\n");

    goto funcA_label; // Стрибок до funcA

funcA_label:
    funcA();

funcB_label:
    funcB();

    return 0;
}

void funcA() {
    printf("У функції A\n");

    // Стрибок до мітки у функції funcB
    goto funcB_label;
}

void funcB() {
    printf("У функції B\n");

    // Рекурсивний виклик із використанням "стрибка"
    goto funcA_label;
}

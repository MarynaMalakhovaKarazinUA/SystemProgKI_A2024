#include <stdio.h>

// Прототипи функцій
void functionA();
void functionB();

void functionA() {
    printf("Виклик функції A\n");
    goto callB; // Стрибок до функції B

callB:
    functionB();
}

void functionB() {
    static int count = 0;
    printf("Виклик функції B, лічильник: %d\n", count);
    
    if (count < 3) {
        count++;
        goto callA; // Рекурсивний виклик функції A
    }
    
    return;

callA:
    functionA(); // Рекурсивний виклик
}

int main() {
    functionA(); // Запуск з функції A
    return 0;
}

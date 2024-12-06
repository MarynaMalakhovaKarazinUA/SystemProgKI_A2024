#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10000

// Функція для заповнення масиву випадковими числами
void fill_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1000;
    }
}

// Рекурсивна функція для обчислення чисел Фібоначчі
int fibonacci(int n) {
    if (n <= 1)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Функція для сортування масиву
void bubble_sort(int *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Функція для пошуку найбільшого спільного дільника
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Основна функція для профілювання
int main() {
    srand(time(NULL));

    // Запускаємо обчислення чисел Фібоначчі для великих чисел
    printf("Calculating Fibonacci numbers...\n");
    for (int i = 30; i < 35; i++) {
        printf("Fibonacci(%d) = %d\n", i, fibonacci(i));
    }

    // Заповнення масиву та його сортування
    int *arr = (int *)malloc(SIZE * sizeof(int));
    fill_array(arr, SIZE);

    printf("Sorting array...\n");
    bubble_sort(arr, SIZE);

    // Обчислення GCD для різних пар чисел
    printf("Calculating GCD...\n");
    for (int i = 1; i < SIZE - 1; i++) {
        int result = gcd(arr[i], arr[i + 1]);
        if (i % 1000 == 0) {  // Друкуємо результат кожні 1000 ітерацій для наглядності
            printf("GCD(%d, %d) = %d\n", arr[i], arr[i + 1], result);
        }
    }

    free(arr);
    return 0;
}
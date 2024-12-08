#include <stdio.h>

void function1() {
    printf("function1 - START\n");
    volatile int sum = 0;
    for (int i = 0; i < 500000000; i++) {
        sum += i;
    }
    printf("function1 - END\n");
    function2();
}

void function2() {
    printf("function2 - START\n");
    volatile int sum = 0;
    for (int i = 0; i < 1000000000; i++) {
        sum += i;
    }
    printf("function2 - END\n");
    function3();
}

void function3() {
    printf("function3 - START\n");
    volatile int sum = 0;
    for (int i = 0; i < 1500000000; i++) {
        sum += i;
    }
    printf("function3 - END\n");
    function4();
}

void function4() {
    printf("function4 - START\n");
    volatile int sum = 0;
    for (int i = 0; i < 2000000000; i++) {
        sum += i;
    }
    printf("function4 - END\n");
}

int main() {
    function1();
    function2();
    function3();
    function4();

    return 0;
}
#include <stdio.h>

void function1() {
    volatile int sum = 0;
    for (int i = 0; i < 100000000; i++) {
        sum += i;
    }
}

void function2() {
    volatile int product = 1;
    for (int i = 1; i < 200000000; i++) {
        product *= i;
    }
}


int main() {
    function1();
    function2();
    return 0;
}

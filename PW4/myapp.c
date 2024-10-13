#include <stdio.h>

void function1() {
    for (volatile long i = 0; i < 100000000; i++);
}

void function2() {
    for (volatile long i = 0; i < 200000000; i++);
}

int main() {
    function1();
    function2();
    return 0;
}

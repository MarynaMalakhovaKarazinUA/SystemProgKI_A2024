#include <stdio.h>
#include <setjmp.h>

jmp_buf buf;

void function2();

void function1() {
    printf("In function1\n");
    longjmp(buf, 1); // Стрибок до function2
}

void function2() {
    printf("In function2\n");
    longjmp(buf, 2); // Стрибок назад до main
}

int main() {
    int val = setjmp(buf);
    if (val == 0) {
        printf("Starting main\n");
        function1();
    } else if (val == 1) {
        function2();
    } else if (val == 2) {
        printf("Back in main\n");
    }
    return 0;
}

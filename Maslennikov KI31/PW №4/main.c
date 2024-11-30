#include <stdio.h>

extern int shared_value;
void print_value();

int main() {
    print_value();
    shared_value = 20;
    print_value();
    return 0;
}

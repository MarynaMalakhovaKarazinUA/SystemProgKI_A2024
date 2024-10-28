#include <stdio.h>
#include <unistd.h> // Include for sleep function

void functionB();  // Declaration of function B

void functionA() {
    printf("You are in function A\n");
    
    // Jump to function B
    goto call_functionB;

    // This code will not be executed
    printf("This line will not be executed\n");

call_functionB:
    functionB();  // Call function B
}

void functionB() {
    printf("You are in function B\n");

    // Introduce a delay
    sleep(1); // Pause for 1 second

    // Jump back to function A
    goto return_to_functionA;
    
    // This code will not be executed
    printf("This line will not be executed in function B\n");

return_to_functionA:
    functionA();  // Recursive call to function A
}

int main() {
    functionA();  // Initial call to function A
    return 0;
}

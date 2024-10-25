#include <stdio.h>

#define PI 3.14159
#define CIRCLE_AREA(radius) (PI * (radius) * (radius))

int main() {
    float radius = 5.0;
    printf("Area of circle: %f\n", CIRCLE_AREA(radius));
    return 0;
}

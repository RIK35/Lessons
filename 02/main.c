//main.c
#include <stdio.h>
#include "math_utils.h"

int main() {
    int a = 5, b = 3;
    printf("Sum: %d\n", add(a, b));
    printf("Product: %d\n", multiply(a, b));
    return 0;
}
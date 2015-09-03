#include <stdlib.h>
#include <stdio.h>

// Method declerations
int factorial(int *n);
int computeBinomial(int *n);

void runProcess1() {
    printf("(n (n-2)) binomial coefficient computations of integers n=2,3,10, status now!\n");
}

void runProcess2() {
    int *numbers[] = { 2, 4, 6, 8, 10};

    for (int i = 0; i < count; i++) {
        printf("%d \n", computeBinomial(numbers[i]);
        sleep(1);
    }
}

void runProcess3() {

}

void runProcess4() {

}

/**
Computes the factorial of a given number
@param n number being passed in
@return the factorial of a given number, -1 otherwise
**/
int factorial(int n) {
    if(*n < 0) {
        return -1;
    }

    int result = 1;
    for (int i = *n; i > 0; i--) {
        result *= i;
    }

    return result;
}

/**
Computers the binomial coefficent for the given some n for n choose 2
@param n a pointer to the int n being passed in
@return the binomial coefficient for n choose 2, returns -1 otherwise
**/
int computeBinomial(int n) {
    // Loop through and compute n-factorial
    if(2 > n) {
        return -1;
    }

    int nFactorial = factorial(n);
    int cFactorial = factorial(2);
    // Unwrap pointer here so we can computer it
    int nMinusKFactorial = factorial(n-2);

    return nFactorial/cFactorial*(nMinusKFactorial);
}
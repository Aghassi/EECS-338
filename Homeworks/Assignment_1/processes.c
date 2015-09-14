#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Method declerations
int factorial(int n);
int computeBinomial(int n);
void printTime(char *name);

void runProcess1() {
    printIDs("Process 1");
    printf("(n (n-2)) binomial coefficient computations of integers n=2,3,10, status now!\n");
    printTime("Process 1");
}

void runProcess2() {
    printIDs("Process 2");
    int numbers[] = { 2, 4, 6, 8, 10};
    int i = 0;

    for (i; i < 5; i++) {
        int result = computeBinomial(numbers[i]);
        printf("%d \n", result);
        sleep(2);
    }
    printTime("Process 2");
}

void runProcess3() {
    printIDs("Process 3");
    int numbers[] = {3, 5, 7, 9};
    int i = 0;

    for (i; i < 4; i++) {
        int result = computeBinomial(numbers[i]);
        printf("%d \n", result);
        sleep(2);
    }
    printTime("Process 3");
}

void runProcess4() {
    printIDs("Process 4");
    execl("/bin/ls", "ls", "-l", NULL);
    printTime("Process 4");
}

/**
Computes the factorial of a given number
@param n number being passed in
@return the factorial of a given number, -1 otherwise
**/
int factorial(int n) {
    if(n < 0) {
        return -1;
    }

    int result = 1;
    int i = n;
    for (i; i > 0; i--) {
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
    int nMinusKFactorial = factorial(n-2);

    return (nFactorial/(cFactorial*nMinusKFactorial));
}

void printTime(char *name) {
    // time function
    int i = 0;
    for (i; i < LOOP_ITERATIONS; i++) {}
    printUserAndSystemTime(name);
}
#include "minimath.h"

double abs_d(double x) {
    return (x >= 0) ? x : -x;
}

int abs_i(int x) {
    return (x >= 0) ? x : -x;
}

// Round halfway cases away from zero (ex: round(-0.5) == -1)
int round(double x) {
    if(x >= 0)
        return (int)(x + 0.5);
    return -round(-x);
}

int floor(double x) {
    if(x >= 0) {
        return (int)x;
    }
    else {
        if(x == (int)x) {
            return (int)x;
        }
        else {
            return (int)(x-1);
        }
    }
}

int ceil(double x) {
    if(x > 0) {
        if(x == (int)x) {
            return (int)x;
        }
        else {
            return (int)(x+1);
        }
    }
    else {
        return (int)x;
    }
}

// Returns the fractional part of x, with the same sign.
double fpart(double x) {
    if (x < 0) {
        return -(1 - (x - floor(x)));
    }
    return x - floor(x);
}

// Returns the complement to the fractional part of x, with the same sign.
// If x is an integer value (such as 1.0 or 2.0), then rfpart(x) returns 1.0.
double rfpart(double x) {
    if (x >= 0) {
        return 1 - fpart(x);
    }
    return -1 - fpart(x);
}

int min_i(int x, int y) {
    return (x > y) ? y : x;
}

unsigned min_u(unsigned x, unsigned y) {
    return (x > y) ? y : x;
}

double min_d(double x, double y) {
    return (x > y) ? y : x;
}

int max_i(int x, int y) {
    return (x < y) ? y : x;
}

unsigned max_u(unsigned x, unsigned y) {
    return (x < y) ? y : x;
}

double max_d(double x, double y) {
    return (x < y) ? y : x;
}

long double pow(double base, int exp) {
    double output = 1;
    for(int i = 0; i < exp; i++) {
        output *= base;
    }
    // printf("%d^%d = %d\n", (int)base, (int)exp, (int)output);
    return output;
}

// factorial() of a negative number is undefined (here, it returns 0)
// A long int can only completely hold a value of factorial(12) or less
long int factorial(int x) {
    // factorial() is only defined for positive numbers
    if(x < 0) {
        return 0;
    }

    int result = 1;
    for (int i = 1; i <= x; i++) {
        result *= i;
    }
    return result;
}

// 5-term Taylor series implementation
// Number of terms limited by the maximum size of factorial()
double sin(double x) {
    if(x < 0) {
        return -sin(-x);
    }

    long double output = 0;
    int sign = 0;
    int final_sign = 1;

    // Reduce x to a equivalent value between 0 and pi
    // Account for the appropriate sign
    if(fpart(x/(2*MATH_PI)) > 0.5) {
        final_sign = -1;
    }
    x = x - MATH_PI*(int)(x/MATH_PI);

    for(int n = 0; n < 5; n++) {
        sign = (n % 2) ? -1 : 1;
        output += sign*pow(x,2*n+1)/factorial(2*n+1);
    }
    return final_sign * output;
}

// Uses sine to calculate cosine
double cos(double x){
    return sin(MATH_PI/2 - x);
}

// Finds the greatest common denominator using Euclid's algorithm iteratively
int gcd(int a, int b){
    while (b != 0){
        int t = b;
        b = a % b;
        a = t;
    } 
    return a;
}

//finds the minimum value in an array
int min_arr(int array[], int length){
    int min = 0;
    for(int i = 0; i < length; i ++){
        min = min_i(min, array[i]);
    }
    return min;
}
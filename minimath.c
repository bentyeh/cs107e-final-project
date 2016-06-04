#include "minimath.h"

#define PI_MATH 3.141596

double abs_d(double x) {
    return (x >= 0) ? x : -x;
}

int abs_i(int x) {
    return (x >= 0) ? x : -x;
}

// Round halfway cases away from zero (ex: round(-0.7) == -1)
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
        if((int)x - x > 0.5) {
            return round(x);
        }
        else {
            return round(x - 0.5);
        }
    }
}

int ceil(double x) {
    if(x > 0) {
        if(x - (int)x > 0.5) {
            return round(x);
        }
        else {
            return round(x + 0.5);
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

int pow(int base, int exp) {
    for(int i = 0; i < exp; i++) {
        base *= base;
    }
    return base;
}

int factorial(int x) {
    if(x < 0) return 0;
    int result = 1;
    for (int i = 1; i <= x; i++) {
        result *= i;
    }
    return result;
}

// Taylor Series Implementation
double sin(double x) {
    double output;
    int sign;

    output = x;
    for(int n = 0; n < 5; n++) {
        sign = (n % 2) ? 1 : -1;
        output += sign*pow(x,2*n+1)/factorial(2*n+1);
    }
    return output;
}

// Uses sine to calculate cosine
double cos(double x){
    return sin((PI_MATH/2) - x);
}
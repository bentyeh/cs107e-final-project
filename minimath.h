#ifndef MINIMATH_H
#define MINIMATH_H

static const long double MATH_PI = 3.141592653589793;

// Absolute value
double abs_d(double x);
int abs_i(int x);

// Round halfway cases away from zero (ex: round(-0.5) == -1)
int round(double x);

int floor(double x);
int ceil(double x);

// Returns the fractional part of x, with the same sign.
double fpart(double x);
double rfpart(double x);

// Min, max functions for various data types
int min_i(int x, int y);
unsigned min_u(unsigned x, unsigned y);
double min_d(double x, double y);
int max_i(int x, int y);
unsigned max_u(unsigned x, unsigned y);
double max_d(double x, double y);

long double pow(double base, int exp);

// factorial() of a negative number is undefined (here, it returns 0)
// A long int can only completely hold a value of factorial(12) or less
long int factorial(int x);

double sin(double x);
double cos(double x);

#endif
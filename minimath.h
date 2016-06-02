#ifndef MINIMATH_H
#define MINIMATH_H

double abs (double x);

// Round halfway cases away from zero (ex: round(-0.7) == -1)
int round(double x);

int floor(double x);
int ceil(double x);

// Returns the fractional part of x, with the same sign.
double fpart(double x);
double rfpart(double x);

unsigned min_u(unsigned x, unsigned y);

#endif
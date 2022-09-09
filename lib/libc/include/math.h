#pragma once

#define M_E 2.71828
#define M_LOG2E 1.4427
#define M_LOG10E 0.434294
#define M_LN2 0.693147
#define M_LN10 2.30259
#define M_PI 3.14159
#define M_PI_2 1.5708
#define M_PI_4 0.785398
#define M_1_PI 0.31831
#define M_2_PI 0.63662
#define M_2_SQRTPI 1.12838
#define M_SQRT2 1.41421
#define M_SQRT1_2 0.707107
#define HUGE 3.40282e+38
#define MAXFLOAT 3.40282e+38
#define HUGE_VAL 1.0e300

double sin(double x);
double cos(double x);
double tan(double x);
double asin(double x);
double acos(double x);
double atan(double x);
double atan2(double y, double x);
double sinh(double x);
double cosh(double x);
double tanh(double x);
double exp(double x);
double log(double x);
double log10(double x);
double pow(double x, double y);
double sqrt(double x);
double ceil(double x);
double floor(double x);
double fabs(double x);
double ldexp(double x, int n);
double frexp(double x, int* exp);
double modf(double x, double* ip);
double fmod(double x, double y);
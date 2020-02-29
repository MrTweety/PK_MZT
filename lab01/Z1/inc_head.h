///////////////////////////////////////////////////////////////
//   inc_head.h

#pragma once

#include <ctime>

#define YES_AVX
#define YES_FMA

void getcurtime(clock_t start);
void check(double *y, double *z, int n);
void prepare(double *a, double *x, int n);
void prepare_block(double *a, double *x, int n, int lb);

void matvec_orgin(double* a, double* x, double* y, int n);
void matvec_opt_1(double* a, double* x, double* y, int n);
void matvec_opt_2(double* a, double* x, double* y, int n);
void matvec_opt_3(double* a, double* x, double* y, int n, int lb);
void matvec_opt_4(double* a, double* x, double* y, int n, int lb);
#ifdef YES_AVX
void matvec_opt_5(double* a, double* x, double* y, int n, int lb);
#endif

#ifdef YES_FMA
void matvec_opt_6(double* a, double* x, double* y, int n, int lb);
#endif

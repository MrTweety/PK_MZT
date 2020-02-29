///////////////////////////////////////////////////////////////////////////
//   mvec.h

#ifndef MVEC_HEADER_INCLUDE__H
#define MVEC_HEADER_INCLUDE__H

#define YES_AVX
#define YES_FMA

void check     (double *y, double *z, int n);
void prepare   (double *a, double *x, int n);
void mult_naive(double *a, double *x, double *y, int n);
void matvec_XMM(double* a, double* x, double* y, int n, int lb);

#ifdef YES_AVX
void matvec_YMM(double* a, double* x, double* y, int n, int lb);
#endif

#ifdef YES_FMA
void matvec_FMA(double* a, double* x, double* y, int n, int lb);
#endif

#endif
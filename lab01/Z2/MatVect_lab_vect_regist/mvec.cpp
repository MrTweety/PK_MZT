////////////////////////////////////////////////////////////////////////////////////////////////////
//   mvec.cpp

#include "stdafx.h"
#include <iostream>
#include "mvec.h"
#include <emmintrin.h>

using namespace std;

void mult_naive(double *a, double *x, double *y, int n)
{
	int i, j, ij;
	double register reg;

	for(i=0, ij=0; i<n; ++i)
	{
		reg = 0;

		for(j=0; j<n; ++j, ++ij)
		{
			reg += a[ij]*x[j];
		}

		y[i] = reg;
	}
}

void matvec_XMM(double* a, double* x, double* y, int n, int lb)
{
	//napisz swoj kod
}

#ifdef YES_AVX
void matvec_YMM(double* a, double* x, double* y, int n, int lb)
{
	//napisz swoj kod
}
#endif

#ifdef YES_FMA
void matvec_FMA(double* a, double* x, double* y, int n, int lb)
{

}
#endif

//////////////////////////////////////////////////////////////////////////////////
//   funthrd.cpp
//   thread function implementations

#include "stdafx.h"
#include <iostream>
#include "timethrd.h"
#include <cmath>
#include "windows.h"
#include <emmintrin.h>
#include <intrin.h>

using namespace std;


DWORD WINAPI ThreadFunc1(LPVOID lpParam ) 
{
	THREAD_DATA *ptrDat = (THREAD_DATA *)(lpParam);

	//Wpisz swoj kod tu
	double dot;
	int i, it, loc_N, ntimes;
	double *X, *Y;
	loc_N = ptrDat->loc_N;
	ntimes = ptrDat->ntimes;
	X = ptrDat->X;
	Y = ptrDat->Y;

	for (it = 0; it < ntimes; ++it) {
		dot = 0;
		for (i = 0; i < loc_N; ++i) {
			dot += X[i] * Y[i];

		}
	}
	ptrDat->res = dot;
	ptrDat->ret = 0;
	return 0; 
}

DWORD WINAPI ThreadFunc2(LPVOID lpParam)
{
	THREAD_DATA* ptrDat = (THREAD_DATA*)(lpParam);

	//Wpisz swoj kod tu
	int i, it, loc_N, ntimes;
	double* X, * Y;
	loc_N = ptrDat->loc_N;
	ntimes = ptrDat->ntimes;
	X = ptrDat->X;
	Y = ptrDat->Y;

	for (it = 0; it < ntimes; ++it) {
		ptrDat->res = 0;
		for (i = 0; i < loc_N; ++i) {
			ptrDat->res += X[i] * Y[i];

		}
	}
	ptrDat->ret = 0;
	return 0;
}

DWORD WINAPI ThreadFuncFMA(LPVOID lpParam)
{
	THREAD_DATA* ptrDat = (THREAD_DATA*)(lpParam);

	//Wpisz swoj kod tu
#ifdef _WIN64

	__m256d x1, x2, x3, x0, y1, y2, y3, y0, r0, r1, r2, r3;
	int it, i, loc_N, ntimes;
	double* X_loc, * Y_loc;
	double register rrr;
	loc_N = ptrDat->loc_N;
	ntimes = ptrDat->ntimes;


	__declspec(align(32)) double res[4];

	const int inner_unwinding = 16;

	int rest = loc_N % inner_unwinding;

	for (int it = 0; it < ntimes; ++it) {
		X_loc = ptrDat->X;
		Y_loc = ptrDat->Y;
		r0 = r1 = r2 = r3 = _mm256_setzero_pd();
		rrr = 0;
		for ( i = 0; i < loc_N-rest; i += inner_unwinding) {

			_mm_prefetch(( char*)(X_loc + 16), _MM_HINT_T0);
			_mm_prefetch(( char*)(X_loc + 24), _MM_HINT_T0);
			
			_mm_prefetch(( char*)(Y_loc + 16), _MM_HINT_T0);
			_mm_prefetch(( char*)(Y_loc + 24), _MM_HINT_T0);

			x0 = _mm256_loadu_pd(X_loc);
			x1 = _mm256_loadu_pd(X_loc + 4);
			x2 = _mm256_loadu_pd(X_loc + 8);
			x3 = _mm256_loadu_pd(X_loc + 12);
			y0 = _mm256_loadu_pd(Y_loc);
			y1 = _mm256_loadu_pd(Y_loc + 4);
			y2 = _mm256_loadu_pd(Y_loc + 8);
			y3 = _mm256_loadu_pd(Y_loc + 12);

			X_loc += inner_unwinding;
			Y_loc += inner_unwinding;

			r0 = _mm256_fmadd_pd(x0, y0, r0);
			r1 = _mm256_fmadd_pd(x1, y1, r1);
			r2 = _mm256_fmadd_pd(x2, y2, r2);
			r3 = _mm256_fmadd_pd(x3, y3, r3);

		}
		r0 = _mm256_add_pd(r0, r1);
		r2 = _mm256_add_pd(r2, r3);
		r0 = _mm256_add_pd(r0, r2);
		_mm256_store_pd(res, r0);
		rrr =  res[0] + res[1] + res[2] + res[3];

		for (; i < loc_N; i++) {
			rrr += ptrDat->X[i] * ptrDat->Y[i];
		}

	}
	ptrDat->res = rrr;
	ptrDat->ret = 0;

#else
	cout << "platform ia32 is not supported\n";
#endif



	return 0;
}
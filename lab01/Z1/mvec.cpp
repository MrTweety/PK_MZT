////////////////////////////////////////////////////////////////////////////////
//  mvec.cpp

////////////////////////////////////////////////////////////////////////////////
//  mvec.cpp

#include "stdafx.h"
#include<iostream>
#include <emmintrin.h>
#include <intrin.h>
#include "inc_head.h"

using namespace std;

void matvec_orgin(double* a, double* x, double* y, int n)
{
	int i, j;

	for(i=0;i<n;i++){
	  y[i]=0.0;
	  for(j=0;j<n;j++){
	    y[i]+=a[i+n*j]*x[j];
	  }
	}
}


void matvec_opt_1(double* a, double* x, double* y, int n)
/*=========================================================================
Usuniecie skokow w danych
===========================================================================*/
{
	//napisz swoj kod
	
}

void matvec_opt_2(double* a, double* x, double* y, int n)
/*============================================================================
Rozwijanie petli
=============================================================================*/
{
	//napisz swoj kod

}

void matvec_opt_3(double* a, double* x, double* y, int n, int lb)
{
	int i, j;
	
	memset((void *)y, 0, n*sizeof(double));

	__m128d ry0, ry1, ry2, ry3, ra0, ra1, ra2, ra3, rx0;
	double *ptr_a, *ptr_x, *ptr_y;
	const int mr = 8;
	if(mr != lb)
	{
		cout << "lb != mr\n";
		system("pause");
		exit(1);
	}

	ptr_a = a;

	for(i=0; i<n; i+=mr)
	{
		ry0 = _mm_setzero_pd();
		ry1 = _mm_setzero_pd();
		ry2 = _mm_setzero_pd();
		ry3 = _mm_setzero_pd();
		
		//ptr_a = &a[n*i];
		ptr_y = &y[i];
		ptr_x = x;

		for(j=0; j<n; j++)
		{
			//_mm_prefetch((const char *)(ptr_a+8), _MM_HINT_T0);
			_mm_prefetch((const char *)(ptr_a+8), _MM_HINT_NTA);
			if (j % mr == 0)
				_mm_prefetch((const char *)(ptr_x + 8), _MM_HINT_T0);
			rx0 = _mm_load1_pd(ptr_x);
			ra0 = _mm_load_pd(ptr_a);
			ra1 = _mm_load_pd(ptr_a+2);
			ra2 = _mm_load_pd(ptr_a+4);
			ra3 = _mm_load_pd(ptr_a+6);

			ptr_a += mr;
			ptr_x++;

			ra0 = _mm_mul_pd(ra0, rx0);
			ra1 = _mm_mul_pd(ra1, rx0);
			ra2 = _mm_mul_pd(ra2, rx0);
			ra3 = _mm_mul_pd(ra3, rx0);

			ry0 = _mm_add_pd(ry0, ra0);
			ry1 = _mm_add_pd(ry1, ra1);
			ry2 = _mm_add_pd(ry2, ra2);
			ry3 = _mm_add_pd(ry3, ra3);
		}

		_mm_store_pd(ptr_y, ry0);
		_mm_store_pd(ptr_y+2, ry1);
		_mm_store_pd(ptr_y+4, ry2);
		_mm_store_pd(ptr_y+6, ry3);
	}
}

void matvec_opt_4(double* a, double* x, double* y, int n, int lb)
{
	int i, j;
	
	memset((void *)y, 0, n*sizeof(double));

	__m128d ry0, ry1, ry2, ry3, ra0, ra1, ra2, ra3, rx0;
	double *ptr_a, *ptr_x, *ptr_y;
	const int mr = 8;
	const int nr = 4;
	if(mr != lb || nr != lb/2)
	{
		cout << "lb != mr || lb != nr\n";
		system("pause");
		exit(1);
	}

	ptr_a = a;

	for(i=0; i<n; i+=mr)
	{
		ry0 = _mm_setzero_pd();
		ry1 = _mm_setzero_pd();
		ry2 = _mm_setzero_pd();
		ry3 = _mm_setzero_pd();
				
		//ptr_a = &a[n*i];
		ptr_y = &y[i];
		ptr_x = x;

		for(j=0; j<n; j+=nr)
		{
			_mm_prefetch((const char *)(ptr_a + mr*nr), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 1)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 2)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 3)), _MM_HINT_NTA);

			if(j%nr == 0)
				_mm_prefetch((const char *)(ptr_x + nr), _MM_HINT_T0);
			//--------------------------0
			rx0 = _mm_load1_pd(ptr_x);
			ra0 = _mm_load_pd(ptr_a);
			ra1 = _mm_load_pd(ptr_a+2);
			ra2 = _mm_load_pd(ptr_a+4);
			ra3 = _mm_load_pd(ptr_a+6);

			ra0 = _mm_mul_pd(ra0, rx0);
			ra1 = _mm_mul_pd(ra1, rx0);
			ra2 = _mm_mul_pd(ra2, rx0);
			ra3 = _mm_mul_pd(ra3, rx0);

			ry0 = _mm_add_pd(ry0, ra0);
			ry1 = _mm_add_pd(ry1, ra1);
			ry2 = _mm_add_pd(ry2, ra2);
			ry3 = _mm_add_pd(ry3, ra3);

			//--------------------------1
			rx0 = _mm_load1_pd(ptr_x+1);
			ra0 = _mm_load_pd(ptr_a+8);
			ra1 = _mm_load_pd(ptr_a+10);
			ra2 = _mm_load_pd(ptr_a+12);
			ra3 = _mm_load_pd(ptr_a+14);

			ra0 = _mm_mul_pd(ra0, rx0);
			ra1 = _mm_mul_pd(ra1, rx0);
			ra2 = _mm_mul_pd(ra2, rx0);
			ra3 = _mm_mul_pd(ra3, rx0);

			ry0 = _mm_add_pd(ry0, ra0);
			ry1 = _mm_add_pd(ry1, ra1);
			ry2 = _mm_add_pd(ry2, ra2);
			ry3 = _mm_add_pd(ry3, ra3);

			//--------------------------2
			rx0 = _mm_load1_pd(ptr_x+2);
			ra0 = _mm_load_pd(ptr_a+16);
			ra1 = _mm_load_pd(ptr_a+18);
			ra2 = _mm_load_pd(ptr_a+20);
			ra3 = _mm_load_pd(ptr_a+22);

			ra0 = _mm_mul_pd(ra0, rx0);
			ra1 = _mm_mul_pd(ra1, rx0);
			ra2 = _mm_mul_pd(ra2, rx0);
			ra3 = _mm_mul_pd(ra3, rx0);

			ry0 = _mm_add_pd(ry0, ra0);
			ry1 = _mm_add_pd(ry1, ra1);
			ry2 = _mm_add_pd(ry2, ra2);
			ry3 = _mm_add_pd(ry3, ra3);
			
			//--------------------------3
			rx0 = _mm_load1_pd(ptr_x+3);
			ra0 = _mm_load_pd(ptr_a+24);
			ra1 = _mm_load_pd(ptr_a+26);
			ra2 = _mm_load_pd(ptr_a+28);
			ra3 = _mm_load_pd(ptr_a+30);

			ptr_a += nr*mr;
			ptr_x += nr;

			ra0 = _mm_mul_pd(ra0, rx0);
			ra1 = _mm_mul_pd(ra1, rx0);
			ra2 = _mm_mul_pd(ra2, rx0);
			ra3 = _mm_mul_pd(ra3, rx0);

			ry0 = _mm_add_pd(ry0, ra0);
			ry1 = _mm_add_pd(ry1, ra1);
			ry2 = _mm_add_pd(ry2, ra2);
			ry3 = _mm_add_pd(ry3, ra3);
		}

		_mm_store_pd(ptr_y, ry0);
		_mm_store_pd(ptr_y+2, ry1);
		_mm_store_pd(ptr_y+4, ry2);
		_mm_store_pd(ptr_y+6, ry3);
	}
}

#ifdef YES_AVX
void matvec_opt_5(double* a, double* x, double* y, int n, int lb)
{
	int i, j;

	memset((void *)y, 0, n*sizeof(double));

	__m256d ry0, ry1, ry2, ry3, ra0, ra1, ra2, ra3, rx0, rx1;
	double *ptr_a, *ptr_x, *ptr_y;
	const int mr = 8, nr = 8;
	if (mr != lb)
	{
		cout << "lb != mr\n";
		system("pause");
		exit(1);
	}

	ptr_a = a;

	for (i = 0; i<n; i += mr)
	{
		ry0 = _mm256_setzero_pd();
		ry1 = ry2 = ry3 = ry0;
		
		//ptr_a = &a[n*i];
		ptr_y = &y[i];
		ptr_x = x;

		for (j = 0; j<n; j+=nr)
		{
			_mm_prefetch((const char *)(ptr_a + mr*nr), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 1)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 2)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 3)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 4)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 5)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 6)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 7)), _MM_HINT_NTA);

			_mm_prefetch((const char *)(ptr_x + nr), _MM_HINT_T0);

			//---------------------------------------------------0
			rx0 = _mm256_broadcast_sd(ptr_x);
			rx1 = _mm256_broadcast_sd(ptr_x + 1);
			ra0 = _mm256_load_pd(ptr_a);
			ra1 = _mm256_load_pd(ptr_a + 4);
			ra2 = _mm256_load_pd(ptr_a + mr);
			ra3 = _mm256_load_pd(ptr_a + mr + 4);
			
			ra0 = _mm256_mul_pd(ra0, rx0);
			ra1 = _mm256_mul_pd(ra1, rx0);
			ra2 = _mm256_mul_pd(ra2, rx1);
			ra3 = _mm256_mul_pd(ra3, rx1);
			
			ry0 = _mm256_add_pd(ry0, ra0);
			ry1 = _mm256_add_pd(ry1, ra1);
			ry2 = _mm256_add_pd(ry2, ra2);
			ry3 = _mm256_add_pd(ry3, ra3);

			//---------------------------------------------------1
			
			rx0 = _mm256_broadcast_sd(ptr_x + 2);
			rx1 = _mm256_broadcast_sd(ptr_x + 3);
			ra0 = _mm256_load_pd(ptr_a + 2*mr);
			ra1 = _mm256_load_pd(ptr_a + 2*mr + 4);
			ra2 = _mm256_load_pd(ptr_a + 3 * mr);
			ra3 = _mm256_load_pd(ptr_a + 3 * mr + 4);

			ra0 = _mm256_mul_pd(ra0, rx0);
			ra1 = _mm256_mul_pd(ra1, rx0);
			ra2 = _mm256_mul_pd(ra2, rx1);
			ra3 = _mm256_mul_pd(ra3, rx1);

			ry0 = _mm256_add_pd(ry0, ra0);
			ry1 = _mm256_add_pd(ry1, ra1);
			ry2 = _mm256_add_pd(ry2, ra2);
			ry3 = _mm256_add_pd(ry3, ra3);

			//---------------------------------------------------2
			rx0 = _mm256_broadcast_sd(ptr_x + 4);
			rx1 = _mm256_broadcast_sd(ptr_x + 5);
			ra0 = _mm256_load_pd(ptr_a + 4 * mr);
			ra1 = _mm256_load_pd(ptr_a + 4 * mr + 4);
			ra2 = _mm256_load_pd(ptr_a + 5 * mr);
			ra3 = _mm256_load_pd(ptr_a + 5 * mr + 4);

			ra0 = _mm256_mul_pd(ra0, rx0);
			ra1 = _mm256_mul_pd(ra1, rx0);
			ra2 = _mm256_mul_pd(ra2, rx1);
			ra3 = _mm256_mul_pd(ra3, rx1);

			ry0 = _mm256_add_pd(ry0, ra0);
			ry1 = _mm256_add_pd(ry1, ra1);
			ry2 = _mm256_add_pd(ry2, ra2);
			ry3 = _mm256_add_pd(ry3, ra3);

			//---------------------------------------------------3
			rx0 = _mm256_broadcast_sd(ptr_x + 6);
			rx1 = _mm256_broadcast_sd(ptr_x + 7);
			ra0 = _mm256_load_pd(ptr_a + 6 * mr);
			ra1 = _mm256_load_pd(ptr_a + 6 * mr + 4);
			ra2 = _mm256_load_pd(ptr_a + 7 * mr);
			ra3 = _mm256_load_pd(ptr_a + 7 * mr + 4);

			ptr_a += mr*nr;
			ptr_x += nr;

			ra0 = _mm256_mul_pd(ra0, rx0);
			ra1 = _mm256_mul_pd(ra1, rx0);
			ra2 = _mm256_mul_pd(ra2, rx1);
			ra3 = _mm256_mul_pd(ra3, rx1);

			ry0 = _mm256_add_pd(ry0, ra0);
			ry1 = _mm256_add_pd(ry1, ra1);
			ry2 = _mm256_add_pd(ry2, ra2);
			ry3 = _mm256_add_pd(ry3, ra3);
		}

		ry0 = _mm256_add_pd(ry0, ry2);
		ry1 = _mm256_add_pd(ry1, ry3);
		_mm256_store_pd(ptr_y, ry0);
		_mm256_store_pd(ptr_y + 4, ry1);
	}
}
#endif


#ifdef YES_FMA
void matvec_opt_6(double* a, double* x, double* y, int n, int lb)
{
	int i, j;

	memset((void *)y, 0, n*sizeof(double));

	__m256d ry0, ry1, ry2, ry3, ra0, ra1, ra2, ra3, rx0, rx1;
	double *ptr_a, *ptr_x, *ptr_y;
	const int mr = 8, nr = 8;
	if (mr != lb)
	{
		cout << "lb != mr\n";
		system("pause");
		exit(1);
	}

	ptr_a = a;

	for (i = 0; i<n; i += mr)
	{
		ry0 = _mm256_setzero_pd();
		ry1 = ry2 = ry3 = ry0;

		ptr_y = &y[i];
		ptr_x = x;

		for (j = 0; j<n; j += nr)
		{
			_mm_prefetch((const char *)(ptr_a + mr*nr), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 1)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 2)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 3)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 4)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 5)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 6)), _MM_HINT_NTA);
			_mm_prefetch((const char *)(ptr_a + mr*(nr + 7)), _MM_HINT_NTA);

			_mm_prefetch((const char *)(ptr_x + nr), _MM_HINT_T0);

			//---------------------------------------------------0
			rx0 = _mm256_broadcast_sd(ptr_x);
			rx1 = _mm256_broadcast_sd(ptr_x + 1);
			ra0 = _mm256_load_pd(ptr_a);
			ra1 = _mm256_load_pd(ptr_a + 4);
			ra2 = _mm256_load_pd(ptr_a + mr);
			ra3 = _mm256_load_pd(ptr_a + mr + 4);

			ry0 = _mm256_fmadd_pd(ra0, rx0, ry0);
			ry1 = _mm256_fmadd_pd(ra1, rx0, ry1);
			ry2 = _mm256_fmadd_pd(ra2, rx1, ry2);
			ry3 = _mm256_fmadd_pd(ra3, rx1, ry3);

			//---------------------------------------------------1
			rx0 = _mm256_broadcast_sd(ptr_x + 2);
			rx1 = _mm256_broadcast_sd(ptr_x + 3);
			ra0 = _mm256_load_pd(ptr_a + 2 * mr);
			ra1 = _mm256_load_pd(ptr_a + 2 * mr + 4);
			ra2 = _mm256_load_pd(ptr_a + 3 * mr);
			ra3 = _mm256_load_pd(ptr_a + 3 * mr + 4);

			ry0 = _mm256_fmadd_pd(ra0, rx0, ry0);
			ry1 = _mm256_fmadd_pd(ra1, rx0, ry1);
			ry2 = _mm256_fmadd_pd(ra2, rx1, ry2);
			ry3 = _mm256_fmadd_pd(ra3, rx1, ry3);
			
			//---------------------------------------------------2
			rx0 = _mm256_broadcast_sd(ptr_x + 4);
			rx1 = _mm256_broadcast_sd(ptr_x + 5);
			ra0 = _mm256_load_pd(ptr_a + 4 * mr);
			ra1 = _mm256_load_pd(ptr_a + 4 * mr + 4);
			ra2 = _mm256_load_pd(ptr_a + 5 * mr);
			ra3 = _mm256_load_pd(ptr_a + 5 * mr + 4);

			ry0 = _mm256_fmadd_pd(ra0, rx0, ry0);
			ry1 = _mm256_fmadd_pd(ra1, rx0, ry1);
			ry2 = _mm256_fmadd_pd(ra2, rx1, ry2);
			ry3 = _mm256_fmadd_pd(ra3, rx1, ry3);

			//---------------------------------------------------2
			rx0 = _mm256_broadcast_sd(ptr_x + 6);
			rx1 = _mm256_broadcast_sd(ptr_x + 7);
			ra0 = _mm256_load_pd(ptr_a + 6 * mr);
			ra1 = _mm256_load_pd(ptr_a + 6 * mr + 4);
			ra2 = _mm256_load_pd(ptr_a + 7 * mr);
			ra3 = _mm256_load_pd(ptr_a + 7 * mr + 4);

			ptr_a += mr * nr;
			ptr_x += nr;

			ry0 = _mm256_fmadd_pd(ra0, rx0, ry0);
			ry1 = _mm256_fmadd_pd(ra1, rx0, ry1);
			ry2 = _mm256_fmadd_pd(ra2, rx1, ry2);
			ry3 = _mm256_fmadd_pd(ra3, rx1, ry3);
		}
		ry0 = _mm256_add_pd(ry0, ry2);
		ry1 = _mm256_add_pd(ry1, ry3);

		_mm256_store_pd(ptr_y, ry0);
		_mm256_store_pd(ptr_y + 4, ry1);
	}
}
#endif
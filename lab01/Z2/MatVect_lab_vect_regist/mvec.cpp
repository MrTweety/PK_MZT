////////////////////////////////////////////////////////////////////////////////////////////////////
//   mvec.cpp

#include "stdafx.h"
#include <iostream>
#include "mvec.h"
#include <emmintrin.h>
#include <intrin.h>

using namespace std;

void mult_naive(double* a, double* x, double* y, int n)
{
	int i, j, ij;
	double register reg;

	for (i = 0, ij = 0; i < n; ++i)
	{
		reg = 0;

		for (j = 0; j < n; ++j, ++ij)
		{
			reg += a[ij] * x[j];
		}

		y[i] = reg;
		//std::cout << y[i] << endl;

	}
}

// zewnetrzna zeby zabezpieczyæ register’s reuse // ponownego u¿ycia rejestru
// wewnetrzna zeby zabezpieczyæ potokowe przetwarzanie

void matvec_XMM(double* a, double* x, double* y, int n, int lb)
{
	//napisz swoj kod

	//__m128d is a data type that the compiler will hopefully store in a XMM 128 bit register when optimizing 
	// https://stackoverflow.com/a/53757729 - __m128d
	//__m128d mieszcz¹ sie 2 s³owa double (double - 64 bity)
	int i, j;

	memset((void*)y, 0, n * sizeof(double));
	//memset((void*)y, 0, _msize((void*)y));

	__m128d ra0, ra1, ra2, ra3, rx0, ry0, ry1, ry2, ry3;

	double* ptr_a, * ptr_x;
	__declspec(align(16)) double res0[2], res1[2], res2[2], res3[2];

	const int outer_unwinding = 4;
	const int inner_unwinding = 8;

	const int mr = 8;
	if (mr != lb)
	{
		cout << "lb != mr\n";
		system("pause");
		exit(1);
	}
	ptr_a = a;

	for (i = 0; i < n; i+= outer_unwinding) {
		ptr_x = x;
		ry0 =ry1 =ry2 =ry3 = _mm_setzero_pd();

		for (j = 0; j < n; j += inner_unwinding) {
			//r=r+a(ij)*x(j)


			_mm_prefetch((const char*)(ptr_x +  8), _MM_HINT_T0);
			_mm_prefetch((const char*)(ptr_a + 8), _MM_HINT_NTA);
			_mm_prefetch((const char*)(ptr_a + n + 8), _MM_HINT_NTA);
			_mm_prefetch((const char*)(ptr_a + 2 * n +  8), _MM_HINT_NTA);
			_mm_prefetch((const char*)(ptr_a + 3 * n +  8), _MM_HINT_NTA);

			//_mm_prefetch((const char*)(ptr_x + 16), _MM_HINT_T0);
			//_mm_prefetch((const char*)(ptr_a + 16), _MM_HINT_NTA);
			//_mm_prefetch((const char*)(ptr_a + n + 16), _MM_HINT_NTA);
			//_mm_prefetch((const char*)(ptr_a + 2 * n + 16), _MM_HINT_NTA);
			//_mm_prefetch((const char*)(ptr_a + 3 * n + 16), _MM_HINT_NTA);

			// wczytuje 2 s³owa double
			rx0 = _mm_load_pd(ptr_x);
			ra0 = _mm_load_pd(ptr_a);
			ra1 = _mm_load_pd(ptr_a + n);
			ra2 = _mm_load_pd(ptr_a + 2*n);
			ra3 = _mm_load_pd(ptr_a + 3*n);

			// Multiplies packed double-precision (64-bit) floating-point elements in a and b.
			// https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm_mul_pd&expand=3919
			ra0 = _mm_mul_pd(ra0, rx0); // mniej wiecej tak: [ra[0]*rx[0], ra[1]*rx[1]]
			ry0 = _mm_add_pd(ry0, ra0); // mniej wiecej tak: [ry[0]+ra[0], ry[1]+ra[1]]
			ra1 = _mm_mul_pd(ra1, rx0);
			ry1 = _mm_add_pd(ry1, ra1);
			ra2 = _mm_mul_pd(ra2, rx0);
			ry2 = _mm_add_pd(ry2, ra2);
			ra3 = _mm_mul_pd(ra3, rx0);
			ry3 = _mm_add_pd(ry3, ra3);



			/*******************************************************/ 
			rx0 = _mm_load_pd(ptr_x+2);
			ra0 = _mm_load_pd(ptr_a+2);
			ra1 = _mm_load_pd(ptr_a + n+2);
			ra2 = _mm_load_pd(ptr_a + 2 * n+2);
			ra3 = _mm_load_pd(ptr_a + 3 * n+2);

			ra0 = _mm_mul_pd(ra0, rx0); 
			ry0 = _mm_add_pd(ry0, ra0); 
			ra1 = _mm_mul_pd(ra1, rx0);
			ry1 = _mm_add_pd(ry1, ra1);
			ra2 = _mm_mul_pd(ra2, rx0);
			ry2 = _mm_add_pd(ry2, ra2);
			ra3 = _mm_mul_pd(ra3, rx0);
			ry3 = _mm_add_pd(ry3, ra3);

			/*******************************************************/
			rx0 = _mm_load_pd(ptr_x + 4);
			ra0 = _mm_load_pd(ptr_a + 4);
			ra1 = _mm_load_pd(ptr_a + n + 4);
			ra2 = _mm_load_pd(ptr_a + 2 * n + 4);
			ra3 = _mm_load_pd(ptr_a + 3 * n + 4);

			ra0 = _mm_mul_pd(ra0, rx0);
			ry0 = _mm_add_pd(ry0, ra0);
			ra1 = _mm_mul_pd(ra1, rx0);
			ry1 = _mm_add_pd(ry1, ra1);
			ra2 = _mm_mul_pd(ra2, rx0);
			ry2 = _mm_add_pd(ry2, ra2);
			ra3 = _mm_mul_pd(ra3, rx0);
			ry3 = _mm_add_pd(ry3, ra3);

			/*******************************************************/
			rx0 = _mm_load_pd(ptr_x + 6);
			ra0 = _mm_load_pd(ptr_a + 6);
			ra1 = _mm_load_pd(ptr_a + n + 6);
			ra2 = _mm_load_pd(ptr_a + 2 * n + 6);
			ra3 = _mm_load_pd(ptr_a + 3 * n + 6);

			ra0 = _mm_mul_pd(ra0, rx0);
			ry0 = _mm_add_pd(ry0, ra0);
			ra1 = _mm_mul_pd(ra1, rx0);
			ry1 = _mm_add_pd(ry1, ra1);
			ra2 = _mm_mul_pd(ra2, rx0);
			ry2 = _mm_add_pd(ry2, ra2);
			ra3 = _mm_mul_pd(ra3, rx0);
			ry3 = _mm_add_pd(ry3, ra3);




			ptr_a += inner_unwinding;
			ptr_x += inner_unwinding;
		}

		ptr_a += (outer_unwinding - 1) * n; // przeskakujemy obliczone wiersze 


		_mm_store_pd(res0, ry0);
		_mm_store_pd(res1, ry1);
		_mm_store_pd(res2, ry2);
		_mm_store_pd(res3, ry3);


		y[i] = res0[0] + res0[1]; 
		y[i +1 ] = res1[0] + res1[1];
		y[i + 2] = res2[0] + res2[1];
		y[i + 3] = res3[0] + res3[1];

	}

}


#ifdef YES_AVX
void matvec_YMM(double* a, double* x, double* y, int n, int lb)
{
	//napisz swoj kod

	//YMM registers are 256 bits long. -> 4s³owa double

	memset((void*)y, 0, _msize((void*)y));

	__m256d ra0, ra1, ra2, ra3, rx, ry0, ry1, ry2, ry3;
	double* ptr_a, * ptr_x;
	__declspec(align(16)) double res0[4], res1[4], res2[4], res3[4];

	const int outer_unwinding = 4;
	const int inner_unwinding = 16;

	const int mr = 8;

	if (mr != lb)
	{
		cout << "lb != mr\n";
		system("pause");
		exit(1);
	}
	ptr_a = a;

	for (int i = 0; i < n; i += outer_unwinding) {
		ry0 = ry1 = ry2 = ry3 = _mm256_setzero_pd();
		ptr_x = x;

		for (int j = 0; j < n; j += inner_unwinding) {

			_mm_prefetch((const char*)(ptr_a + 16), _MM_HINT_NTA);
			_mm_prefetch((const char*)(ptr_a + 16+8), _MM_HINT_NTA);

			_mm_prefetch((const char*)(ptr_x + 16), _MM_HINT_T0);
			_mm_prefetch((const char*)(ptr_x + 16 + 8), _MM_HINT_T0);

			_mm_prefetch((const char*)(ptr_a + n + 16), _MM_HINT_NTA);
			_mm_prefetch((const char*)(ptr_a + n + 16 + 8), _MM_HINT_NTA);

			_mm_prefetch((const char*)(ptr_a + 2 * n + 16), _MM_HINT_NTA);
			_mm_prefetch((const char*)(ptr_a + 2 * n + 16 + 8), _MM_HINT_NTA);

			_mm_prefetch((const char*)(ptr_a + 3 * n + 16), _MM_HINT_NTA);
			_mm_prefetch((const char*)(ptr_a + 3 * n + 16 + 8), _MM_HINT_NTA);

			// wczytuje 4 s³owa double
			rx = _mm256_load_pd(ptr_x);
			ra0 = _mm256_load_pd(ptr_a);
			ra1 = _mm256_load_pd(ptr_a + n);
			ra2 = _mm256_load_pd(ptr_a + 2 * n);
			ra3 = _mm256_load_pd(ptr_a + 3 * n);

			ra0 = _mm256_mul_pd(ra0, rx);
			ry0 = _mm256_add_pd(ry0, ra0);
			ra1 = _mm256_mul_pd(ra1, rx);
			ry1 = _mm256_add_pd(ry1, ra1);
			ra2 = _mm256_mul_pd(ra2, rx);
			ry2 = _mm256_add_pd(ry2, ra2);
			ra3 = _mm256_mul_pd(ra3, rx);
			ry3 = _mm256_add_pd(ry3, ra3);

			/*------------------------------------------------------------*/

			rx = _mm256_load_pd(ptr_x + 4);
			ra0 = _mm256_load_pd(ptr_a + 4);
			ra1 = _mm256_load_pd(ptr_a + n + 4);
			ra2 = _mm256_load_pd(ptr_a + 2 * n + 4);
			ra3 = _mm256_load_pd(ptr_a + 3 * n + 4);


			ra0 = _mm256_mul_pd(ra0, rx);
			ry0 = _mm256_add_pd(ry0, ra0);
			ra1 = _mm256_mul_pd(ra1, rx);
			ry1 = _mm256_add_pd(ry1, ra1);
			ra2 = _mm256_mul_pd(ra2, rx);
			ry2 = _mm256_add_pd(ry2, ra2);
			ra3 = _mm256_mul_pd(ra3, rx);
			ry3 = _mm256_add_pd(ry3, ra3);

			/*------------------------------------------------------------*/

			rx = _mm256_load_pd(ptr_x + 8);
			ra0 = _mm256_load_pd(ptr_a + 8);
			ra1 = _mm256_load_pd(ptr_a + n + 8);
			ra2 = _mm256_load_pd(ptr_a + 2 * n + 8);
			ra3 = _mm256_load_pd(ptr_a + 3 * n + 8);


			ra0 = _mm256_mul_pd(ra0, rx);
			ry0 = _mm256_add_pd(ry0, ra0);
			ra1 = _mm256_mul_pd(ra1, rx);
			ry1 = _mm256_add_pd(ry1, ra1);
			ra2 = _mm256_mul_pd(ra2, rx);
			ry2 = _mm256_add_pd(ry2, ra2);
			ra3 = _mm256_mul_pd(ra3, rx);
			ry3 = _mm256_add_pd(ry3, ra3);

			/*------------------------------------------------------------*/

			rx = _mm256_load_pd(ptr_x + 12);
			ra0 = _mm256_load_pd(ptr_a + 12);
			ra1 = _mm256_load_pd(ptr_a + n + 12);
			ra2 = _mm256_load_pd(ptr_a + 2 * n + 12);
			ra3 = _mm256_load_pd(ptr_a + 3 * n + 12);


			ra0 = _mm256_mul_pd(ra0, rx);
			ry0 = _mm256_add_pd(ry0, ra0);
			ra1 = _mm256_mul_pd(ra1, rx);
			ry1 = _mm256_add_pd(ry1, ra1);
			ra2 = _mm256_mul_pd(ra2, rx);
			ry2 = _mm256_add_pd(ry2, ra2);
			ra3 = _mm256_mul_pd(ra3, rx);
			ry3 = _mm256_add_pd(ry3, ra3);



			ptr_a += inner_unwinding;
			ptr_x += inner_unwinding;
		}

		ptr_a += (outer_unwinding - 1) * n;

		_mm256_store_pd(res0, ry0);
		_mm256_store_pd(res1, ry1);
		_mm256_store_pd(res2, ry2);
		_mm256_store_pd(res3, ry3);

		y[i] = res0[0] + res0[1] + res0[2] + res0[3];
		y[i + 1] = res1[0] + res1[1] + res1[2] + res1[3];
		y[i + 2] = res2[0] + res2[1] + res2[2] + res2[3];
		y[i + 3] = res3[0] + res3[1] + res3[2] + res3[3];

	}
}
#endif

#ifdef YES_FMA
void matvec_FMA(double* a, double* x, double* y, int n, int lb)
{

	//memset((void*)y, 0, n * sizeof(double));

	memset((void*)y, 0, _msize((void*)y));


	__m256d ra0, ra1, ra2, ra3, rx, ry0, ry1, ry2, ry3;
	double* ptr_a, * ptr_x;
	__declspec(align(16)) double res0[4], res1[4], res2[4], res3[4];

	const int outer_unwinding = 4;
	const int inner_unwinding = 16;

	const int mr = 8;

	if (mr != lb)
	{
		cout << "lb != mr\n";
		system("pause");
		exit(1);
	}
	ptr_a = a;

	for (int i = 0; i < n; i += outer_unwinding) {
		ry0 = ry1 = ry2 = ry3 = _mm256_setzero_pd();
		ptr_x = x;

		for (int j = 0; j < n; j += inner_unwinding) {

			_mm_prefetch((const char*)(ptr_a + 16), _MM_HINT_NTA);
			_mm_prefetch((const char*)(ptr_a + 24), _MM_HINT_NTA);

			_mm_prefetch((const char*)(ptr_x + 16), _MM_HINT_T0);
			_mm_prefetch((const char*)(ptr_x + 24), _MM_HINT_T0);

			_mm_prefetch((const char*)(ptr_a + n + 16), _MM_HINT_NTA);
			_mm_prefetch((const char*)(ptr_a + n + 24), _MM_HINT_NTA);

			_mm_prefetch((const char*)(ptr_a + 2 * n + 16), _MM_HINT_NTA);
			_mm_prefetch((const char*)(ptr_a + 2 * n + 24), _MM_HINT_NTA);

			_mm_prefetch((const char*)(ptr_a + 3 * n + 16), _MM_HINT_NTA);
			_mm_prefetch((const char*)(ptr_a + 3 * n + 24), _MM_HINT_NTA);

			// wczytuje 4 s³owa double
			rx = _mm256_load_pd(ptr_x);
			ra0 = _mm256_load_pd(ptr_a);
			ra1 = _mm256_load_pd(ptr_a + n);
			ra2 = _mm256_load_pd(ptr_a + 2 * n);
			ra3 = _mm256_load_pd(ptr_a + 3 * n);

			ry0 = _mm256_fmadd_pd(ra0, rx, ry0); // ra0 * rx0 +ry0
			ry1 = _mm256_fmadd_pd(ra1, rx, ry1);
			ry2 = _mm256_fmadd_pd(ra2, rx, ry2);
			ry3 = _mm256_fmadd_pd(ra3, rx, ry3);


			/*------------------------------------------------------------*/

			rx = _mm256_load_pd(ptr_x + 4);
			ra0 = _mm256_load_pd(ptr_a + 4);
			ra1 = _mm256_load_pd(ptr_a + n + 4);
			ra2 = _mm256_load_pd(ptr_a + 2 * n + 4);
			ra3 = _mm256_load_pd(ptr_a + 3 * n + 4);


			ry0 = _mm256_fmadd_pd(ra0, rx, ry0); 
			ry1 = _mm256_fmadd_pd(ra1, rx, ry1);
			ry2 = _mm256_fmadd_pd(ra2, rx, ry2);
			ry3 = _mm256_fmadd_pd(ra3, rx, ry3);

			/*------------------------------------------------------------*/

			rx = _mm256_load_pd(ptr_x + 8);
			ra0 = _mm256_load_pd(ptr_a + 8);
			ra1 = _mm256_load_pd(ptr_a + n + 8);
			ra2 = _mm256_load_pd(ptr_a + 2 * n + 8);
			ra3 = _mm256_load_pd(ptr_a + 3 * n + 8);


			ry0 = _mm256_fmadd_pd(ra0, rx, ry0);
			ry1 = _mm256_fmadd_pd(ra1, rx, ry1);
			ry2 = _mm256_fmadd_pd(ra2, rx, ry2);
			ry3 = _mm256_fmadd_pd(ra3, rx, ry3);

			/*------------------------------------------------------------*/

			rx = _mm256_load_pd(ptr_x + 12);
			ra0 = _mm256_load_pd(ptr_a + 12);
			ra1 = _mm256_load_pd(ptr_a + n + 12);
			ra2 = _mm256_load_pd(ptr_a + 2 * n + 12);
			ra3 = _mm256_load_pd(ptr_a + 3 * n + 12);


			ry0 = _mm256_fmadd_pd(ra0, rx, ry0);
			ry1 = _mm256_fmadd_pd(ra1, rx, ry1);
			ry2 = _mm256_fmadd_pd(ra2, rx, ry2);
			ry3 = _mm256_fmadd_pd(ra3, rx, ry3);


			ptr_a += inner_unwinding;
			ptr_x += inner_unwinding;
		}

		ptr_a += (outer_unwinding - 1) * n;

		_mm256_store_pd(res0, ry0);
		_mm256_store_pd(res1, ry1);
		_mm256_store_pd(res2, ry2);
		_mm256_store_pd(res3, ry3);

		y[i] = res0[0] + res0[1] + res0[2] + res0[3];
		y[i + 1] = res1[0] + res1[1] + res1[2] + res1[3];
		y[i + 2] = res2[0] + res2[1] + res2[2] + res2[3];
		y[i + 3] = res3[0] + res3[1] + res3[2] + res3[3];

	}
}
#endif

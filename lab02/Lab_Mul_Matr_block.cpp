// Lab_Mul_Matr_block.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <cmath>
#include "omp.h"
#include "windows.h"
using namespace std;

// https://github.com/miglopst/matrix_multiplication_acceleration/blob/master/dgemm-openmp.cpp
// https://github.com/rehbergT/dgemm/blob/master/cpp/dgemm_avx2.cpp

void dgemm_naiv_HP(int M, int N, int K, double *A, double *B, double *C);
void dgemm_naiv_seq_HP(int M, int N, int K, double *A, double *B, double *C);
void check(int M, int N, double *C, double *CC);

int main(int argc, char* argv[])
{
	cout << "C = A*B, A[M,K], B[k,N], C[N,M] A, B, C are packed row-by-row\n";
#ifdef _DEBUG
	cout << "DEBUG VERSION\n";
#else
	cout << "RELEASE VERSION\n";
#endif

#ifndef __WIN64
	cout << "PLATFORM 32 bit\n";
#else
	cout << "PLATFORM 64 bit\n";
#endif

	int M, N, K, i, j, k, np;
	double *A = NULL, *B = NULL, *C = NULL, *CC = NULL;
	DWORD ts;
	double elapsed, perf, ops;

	cout << "input dimensions M, N, K\n";
	cin >> M >> N >> K;
	ops = 2.0*(double)M*(double)N*double(K);

	cout << "input number of processors\n";
	cin >> np;

	try
	{
		A = new double [M*K];
		B = new double [K*N];
		C = new double [M*N];
		CC = new double [M*N];
		memset((void *)A, 0, M*K*sizeof(double));
		memset((void *)B, 0, K*N*sizeof(double));
		memset((void *)C, 0, M*N*sizeof(double));
		memset((void *)CC, 0, M*N*sizeof(double));
	}
	catch(bad_alloc)
	{
		cout << "memory allocation error\n";
		system("pause");
		exit(1);
	}

	int ik, jk;

	for(i=0, ik=0; i<M; ++i)
	{
		for(k=0; k<K; ++k, ++ik)
		{
			A[ik] = sqrt((double)(i+1)*(double)(k+1));
		}
	}

	for(k=0, jk=0; k<K; ++k)
	{
		for(j=0; j<N; ++j, ++jk)
		{
			B[jk] = (double)j+(double)k+1.0;
		}
	}

	dgemm_naiv_seq_HP(M, N, K, A, B, CC);

	omp_set_num_threads(np);
	ts = GetTickCount();
	dgemm_naiv_HP(M, N, K, A, B, C);
	elapsed = (double)(GetTickCount()-ts)/1000.0;
	perf = ops/elapsed;

	check(M, N, C, CC);

	cout << "naive HP: perf = " << perf/1000.0/1000.0 << " MFLOPS  duration = " << elapsed << " s\n";

	if(A)
		delete [] A;
	A = NULL;

	if(B)
		delete [] B;
	B = NULL;

	if(C)
		delete [] C;
	C = NULL;

	if(CC)
		delete [] CC;
	CC = NULL;

	system("pause");
	return 0;
}

void dgemm_naiv_seq_HP(int M, int N, int K, double *A, double *B, double *C)
/*===========================================================================
 sequential algorithm
============================================================================*/
{
	int i, j, k;
	register double r;

	memset((void *)C, 0, M*N*sizeof(double));

	for(i=0; i<M; ++i)
	{
		for(k=0; k<K; ++k)
		{
			r = A[K*i+k];
			for(j=0; j<N; ++j)
			{
				C[N*i+j] += r*B[N*k+j];
			}
		}
	}
}

void dgemm_naiv_HP(int M, int N, int K, double *A, double *B, double *C)
/*===========================================================================
 parallel algorithm
============================================================================*/
{
	int i, j, k;
	register double r;

	memset((void *)C, 0, M*N*sizeof(double));

	for(i=0; i<M; ++i)
	{
		for(k=0; k<K; ++k)
		{
			r = A[K*i+k];
			for(j=0; j<N; ++j)
			{
				C[N*i+j] += r*B[N*k+j];
			}
		}
	}
}

void check(int M, int N, double *C, double *CC) 
{
	int i, j, ij;
	double div = 0;

	for(j=0, ij=0; j<N; ++j)
	{
		for(i=0; i<M; ++i, ++ij)
		{
			if(fabs(CC[ij]) > div)
				div = fabs(CC[ij]);
		}
	}

	for(j=0, ij=0; j<N; ++j)
	{
		for(i=0; i<M; ++i, ++ij)
		{
			if(fabs((CC[ij]-C[ij])/div) > 1e-9)
			{
				cout << "error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
				return;
			}
		}
	}
}
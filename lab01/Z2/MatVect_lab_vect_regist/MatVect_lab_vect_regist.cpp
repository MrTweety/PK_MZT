// MatVect_lab_vect_regist.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include "windows.h"
#include "mvec.h"

using namespace std;


int main(int argc, char* argv[])
{
	double *a, *x, *y, *z;
	int n;
	DWORD tstart;
	const int lb = 8;
	double elaps_time;
	cout << "Program Mat_Vect: performance y = y +A*x\n";

#ifdef _DEBUG
	cout << "DEBUG version\n";
#else
	cout << "RELEASE version\n";
#endif

	cout << "Input dimension\n";
	cin >> n;

	n = n/lb;
	n = lb*n;

	try
	{
		a = new double [n*n];
		x = new double [n+1];
		y = new double [n];
		z = new double [n];
	}
	catch(bad_alloc aa)
	{
		cout << "memory allocation error" << endl;
		system("pause");
		exit(1);
	}

	memset((void *)a, 0, _msize((void *)a));
	memset((void *)x, 0, _msize((void *)x));
	memset((void *)y, 0, _msize((void *)y));

	cout << "start\n";

	prepare(a, x, n);

	//-------------------------naive algorithm-----------------------//
	cout << "naive algorithm: \n";
	tstart = GetTickCount();
	mult_naive(a, x, z, n);
	elaps_time = (double)(GetTickCount()-tstart)/1000.0;
	cout << "naive algorithm: " << elaps_time << " sec" << endl;
	
	//-------------------------algorithm which uses XMM registers-----------------------//
	delete [] a;
	delete [] x;
	a = (double *)_aligned_malloc(n*n*sizeof(double), 32);
	x = (double *)_aligned_malloc(n*sizeof(double), 32);
	if(!a || !x)
	{
		cout << "memory allocation error" << endl;
		system("pause");
		exit(1);
	}
	cout << "\n";
	prepare(a, x, n);
	tstart = GetTickCount();
	matvec_XMM(a, x, y, n, lb);
	elaps_time = (double)(GetTickCount()-tstart)/1000.0;
	check(y, z, n);
	cout << "algorithm which uses XMM: " << elaps_time << " sec" << endl;

	//-------------------------algorithm which uses AVX-----------------------//
#ifdef YES_AVX
	cout << "\n";
	tstart = GetTickCount();
	matvec_YMM(a, x, y, n, lb);
	elaps_time = (double)(GetTickCount() - tstart) / 1000.0;
	check(y, z, n);
	cout << "algorithm which uses YMM: " << elaps_time << " sec" << endl;
#endif

	//-------------------------algorithm which uses FMA-----------------------//
#ifdef YES_FMA
	cout << "\n";
	tstart = GetTickCount();
	matvec_FMA(a, x, y, n, lb);
	elaps_time = (double)(GetTickCount() - tstart) / 1000.0;
	check(y, z, n);
	cout << "algorithm which uses FMA: " << elaps_time << " sec" << endl;
#endif
	
	delete [] y;
	delete [] z;
	_aligned_free(a);
	_aligned_free(x);
	
	system("pause");
	return 0;
}


void check(double *y, double *z, int n)
{
	int i;
	for(i=0; i<n; i++)
	{
		if(fabs(z[i] - y[i]) > 1.0e-9)
		{
			cout << "error\n";
			return;
		}
	}

	cout << "OK\n";
}

void prepare(double *a, double *x, int n)
{
	int i, j, ij;

	for(i=0, ij=0; i<n; i++)
	{
		for(j=0; j<n; j++, ij++)
		{
			if(i == j)
				a[ij] = 10.0;
			else
				a[ij] = (double)(i+1);
		}

		x[i] = 1.0;
	}
}




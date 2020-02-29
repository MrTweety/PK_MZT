// MZT_L1.cpp : Defines the entry point for the console application.
//

// L1_block.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include "inc_head.h"

using namespace std;


int main(int argc, char* argv[])
{
	double *a, *x, *y, *z;
	int n;
	clock_t start;
	const int lb = 8;

	cout << "Program Mat_Vect: performance y = y +A*x\n";

#ifdef VERSION_x64
	cout << "x64 plarform\n";
#else
	cout << "ia32 platform\n";
	cout << "this program is available only on x64 platform\n";
#endif

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
		x = new double [n];
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

	//non-optimal code
	cout << "non-optimized code: \n";
	start = clock();
	matvec_orgin(a, x, z, n);
	getcurtime(start);

	//optim_1
	cout << "optim_1: \n";
	start = clock();
	matvec_opt_1(a, x, y, n);
	getcurtime(start);
	check(y, z, n);

	//optim_2
	cout << "optim_2: \n";
	start = clock();
	matvec_opt_2(a, x, y, n);
	getcurtime(start);
	check(y, z, n);

	//optim_3
	cout << "optim_3: \n";
	delete [] a;
	delete [] y;
	a = (double *)_aligned_malloc(n*n*sizeof(double), 32);
	y = (double *)_aligned_malloc(n*sizeof(double), 32);
	if(!a || !y)
	{
		cout << "memory allocation error" << endl;
		system("pause");
		exit(1);
	}
	prepare_block(a, x, n, lb);
	start = clock();
	matvec_opt_3(a, x, y, n, lb);
	getcurtime(start);
	check(y, z, n);

	//optim_4
	cout << "optim_4: \n";
	start = clock();
	matvec_opt_4(a, x, y, n, lb);
	getcurtime(start);
	check(y, z, n);

	//optim_5
#ifdef YES_AVX
	cout << "optim_5: AVX\n";
	start = clock();
	matvec_opt_5(a, x, y, n, lb);
	getcurtime(start);
	check(y, z, n);
#endif

	//optim_6
#ifdef YES_FMA
	cout << "optim_6: FEMA\n";
	start = clock();
	matvec_opt_6(a, x, y, n, lb);
	getcurtime(start);
	check(y, z, n);
#endif
	
	delete [] x;
	delete [] z;
	_aligned_free(a);
	_aligned_free(y);

	system("pause");
	return 0;
}


void getcurtime(clock_t start)
{
	clock_t end = clock();
	double duration = (double)(end-start)/CLOCKS_PER_SEC;
	cout << "CPU time: " << duration << " s" << endl;
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

	for(j=0, ij=0; j<n; j++)
	{
		for(i=0; i<n; i++, ij++)
		{
			if(i == j)
				a[ij] = 10.0;
			else
				a[ij] = (double)(i+1);
		}

		x[j] = 1.0;
	}
}

void prepare_block(double *a, double *x, int n, int lb)
{
	int ii, i, j, ij = 0;

	for(ii=0; ii<n; ii+=lb)
	{
		for(j=0; j<n; j++)
		{
			for(i=ii; i<ii+lb; i++)
			{
				if(i == j)
					a[ij] = 10.0;
				else
					a[ij] = (double)(i+1);
				ij++;
			}
		}
	}
	for(j=0; j<n; j++)
		x[j] = 1.0;
}

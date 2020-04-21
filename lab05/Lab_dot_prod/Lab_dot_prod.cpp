// Lab_dot_prod.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include "timethrd.h"
#include "windows.h"

using namespace std;

#define NUMB_THREADS 4

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hThread[NUMB_THREADS];
	DWORD ThreadID[NUMB_THREADS];
	THREAD_DATA tDat[NUMB_THREADS];
	double *X = NULL, *Y = NULL;
	double dotProd;
	

	FUN_THRD FunThreads[] = { ThreadFunc1,ThreadFunc2,ThreadFuncFMA };

	char* str_title_meth[] = {
		"native r += X*Y",
		"native ptrDat -> res += X*Y",
		"FMA:  X*Y"
	};


	int N, ip, np, loc_N, imath, nometh = sizeof(FunThreads)/sizeof(void *);
	int ntimes;


#ifdef _DEBUG
	cout << "START dotProd: DEBUG VERSION\n";
#else
	cout << "START dotProd: RELEASE VERSION\n";
#endif
	cout << "Get N\n";
	cin >> N;
	//cout << "Get number of processors\n";
	//cin >> np;
	cout << "Get number of repetitions\n";
	cin  >> ntimes;



	//cout << "actual N = " << N << endl;
	//cout << "np = " << np << endl;  

	for (imath = 0; imath < nometh; ++imath) {
		cout << " \nZadanie:  " << str_title_meth[imath] << endl << endl;

		for (np = 1; np <= NUMB_THREADS; ++np) {
			loc_N = N / np;
			N = loc_N * np;
			cout << "np = " << np << "--------------" << endl;
			try
			{
				X = new double[N];
				Y = new double[N];
			}
			catch (bad_alloc aa)
			{
				cout << "memory allocation error\n";
				system("pause");
				exit(1);
			}

			//preparation of X, Y
			int i;
			for (i = 0; i < N; i++)
			{
				X[i] = (double)(i + 1);
				Y[i] = 1.0;
			}



			DWORD time_st = GetTickCount();

			for (ip = 0; ip < np; ip++)
			{
				//wypelnic dane dla potoku ip
				tDat[ip].loc_N = loc_N;
				tDat[ip].ntimes = ntimes;
				tDat[ip].X = X + ip * loc_N;
				tDat[ip].Y = Y + ip * loc_N;

				//stworzyc potok ip
				if ((hThread[ip] = CreateThread(NULL, 0, FunThreads[imath], (LPVOID)&tDat[ip], 0, &ThreadID[ip])) == NULL) {
					cout << "create thread error\n";
					system("pause");
					exit(1);
				}
			}

			//zawiesic pierwotny potok dokad potoki liczace nie skonca prace
			DWORD retv;
			if ((retv = WaitForMultipleObjects(np, hThread, TRUE, INFINITE)) == WAIT_FAILED) {
				cout << "wait for... error\n";
				system("pause");
				exit(1);
			}
			//Check
			bool IsOK = true;
			for (ip = 0; ip < np; ip++)
			{
				if (tDat[ip].ret != 0)
				{
					cout << " Thread # " << ip + 1 << " failed" << endl;
					IsOK = false;
				}
				CloseHandle(hThread[ip]);
			}

			if (!IsOK)
			{
				system("pause");
				exit(1);
			}

			//reduction: sumowanie 
			dotProd = 0.0;

			for (ip = 0; ip < np; ip++)
			{
				dotProd += tDat[ip].res;
			}

			cout << "WTime = " << GetTickCount() - time_st << " ms\n";

			//check
			double sum = 0.0;
			for (i = 0; i < N; i++)
			{
				sum += X[i] * Y[i];
			}

			if ((fabs(sum - dotProd)) > 1.0e-9)
			{
				cout << "error\n";
				cout << "!!!!!!!!!!!\n";
				cout <<"sum:"<< sum<<"dotProd: "<< dotProd <<"\n";

			}



		}
		cout << "END MV\n";
	}
	system("pause");

	delete [] X;
	delete [] Y;

	return 0;
}




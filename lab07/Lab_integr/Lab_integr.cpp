// OMP_2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include "timethrd.h"
//#include <omp.h>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	cout << "DEBUG VERSION\n";
#else
	cout << "RELEASE VERSION\n";
#endif

	int ndiv, ip, np;
	double a, b = 0.01, c, dx;
	double elaps;
	double s=0.0, s_exact, err;
	clock_t start, end;

	HANDLE hThread[NUMB_THREADS];
	DWORD ThreadID[NUMB_THREADS];
	CThreadTime thrd_timer[NUMB_THREADS];
	THREAD_DATA tDat[NUMB_THREADS];

	cout << "Compute numerically: Integral[0, a]{exp(-b*x)*cos(c*x)*dx}\n";
	cout << " [0, a] - limit of integration\n";

	cout << "input a,  c, ndiv\n";
	cin >> a >> c >> ndiv;
	cout << "input number of threads np\n";
	cin >> np;
	
	start = clock();

	ip = ndiv/np;
	ndiv = ip*np;
	dx = a/ndiv;

	cout << "Input data:\n";
	cout << " a    = " << a << endl;
	cout << " b    = " << b << endl;
	cout << " c    = " << c << endl;
	cout << " ndiv = " << ndiv << endl;
	cout << " np   = " << np << endl;

	//stworzyc np potokow potomnych i przekazac im dane
	//zawiesic potok pierwotny dokad potoki robocze nie skancza obliczen

	//Check
	bool IsOK = true;
	for(ip=0; ip<np; ip++)
	{
		if(tDat[ip].ret != 0)
		{
			cout << " Thread # " << ip+1 << " failed" << endl;
			IsOK = false;
		}
	}

	if(!IsOK)
	{
		system("pause");
		exit(1);
	}

	//ThreadTime
	ThreadTimeInfo(np, thrd_timer);
	
	//reduction
	//napisac kod : podsumowac dzialalnosc kazdego potoku
	//i przypisac ten wynik do zmiennej s
	
	end = clock();
	elaps = (double)(end-start)/CLOCKS_PER_SEC;
	cout << "Commercial time: " << elaps << " s" << endl;

	//sprawdzenie dokladnosci: s_exact - wynik dokladny
	//                         s - wynik calkowania numerycznego
	s_exact = exact(a, b, c);
	err = (s-s_exact)/s_exact;
	err = sqrt(err*err);
	cout << "dx/a = " << dx/a << "  err = " << err << endl;
	if(err > 1.0e-6)
		cout << "too large error!!!!!!!\n";
	system("pause");

	return 0;
}

double fun(double x, double b, double c)
{
	return exp(-b*x)*cos(c*x);
}

double exact(double a, double b, double c)
{
	double ret = exp(-a*b)*(-b*cos(a*c)+c*sin(a*c))/(b*b+c*c)+b/(b*b+c*c);
	return ret;
}


//////////////////////////////////////////////////////////////////////////////////
//   funthrd.cpp
//   thread function implementations

#include "stdafx.h"
#include <iostream>
#include "timethrd.h"

using namespace std;

DWORD WINAPI ThreadFunc1(LPVOID lpParam ) 
{
	THREAD_DATA *ptrDat = (THREAD_DATA *)(lpParam);

	CThreadTime *thrd_timer = ptrDat->thrd_timer;
	thrd_timer->begin();
	
	ptrDat->ret = 0;
	//pobrac dane, przekazywane potokowi
	//.....

	int i;
	
	double x_b,  //Wspolrzedna x poczatku odcinku dx 
		   x_e,  //Wspolrzedna x konca odcinku dx 
		   f_b,  //wartosc funkcji podintegralnej na poczatku odcinku dx  
		   f_e,  //wartosc funkcji podintegralnej na koncu odcinku dx  
		   s;    //suma = suma + (f_b+f_e)*dx/2 - metoda trapezu

	s = 0.0;
	//napisac kod calkowania numerycznego metoda trapezow
	x_b = (ptrDat->ndiv * ptrDat->dx) * ptrDat->ip;
	f_b = ptrDat->p_fun(x_b, ptrDat->b, ptrDat->c);
	for (i = 0; i < ptrDat->ndiv; i++) {
		//x_e = x_b + ptrDat->dx;
		x_e = (i + 1 + (ptrDat->ndiv * ptrDat->ip)) * ptrDat->dx;

		f_e = ptrDat->p_fun(x_e, ptrDat->b, ptrDat->c);

		s += f_b + f_e;

		x_b = x_e;
		f_b = f_e;
	}
	
	ptrDat->s = s * ptrDat->dx /2;
	thrd_timer->getelapstime();
	ptrDat->ret = 0;
	
	return 0; 
}
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define NUMB_THREADS 10

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#include "windows.h"

class CThreadTime;

struct THREAD_DATA
{
	int ip;    
	int np;
	int ndiv;
	int ret;
	double a;
	double b;
	double c;
	double dx;
	double (*p_fun)(double x, double b, double c);
	double s;
	CThreadTime *thrd_timer;
};

DWORD WINAPI ThreadFunc1(LPVOID lpParam );
double fun(double x, double b, double c);
double exact(double a, double b, double c);
double (*p_fun)(double x, double b, double c);



// TODO: reference additional headers your program requires here

////////////////////////////////////////////////////////////////////////////////////
//      timethrd.h
//      GetThreadTime service

#pragma once

#include "windows.h"

struct THREAD_DATA
{
	int threadNo;    //thread number   
	int NoThreads;   //number of threads
	int N;           //dimension of vector
	int loc_N;       //dimension of subvector
	int ntimes;      //number of repititions
	int ret;         //return value from thread
	double *X;       //vector X - read only
	double *Y;       //vector Y - read only
	double res;      //dot product for subvectors
};

DWORD WINAPI ThreadFunc1(LPVOID lpParam );
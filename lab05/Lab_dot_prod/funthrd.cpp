//////////////////////////////////////////////////////////////////////////////////
//   funthrd.cpp
//   thread function implementations

#include "stdafx.h"
#include <iostream>
#include "timethrd.h"
#include<cmath>
using namespace std;


DWORD WINAPI ThreadFunc1(LPVOID lpParam ) 
{
	THREAD_DATA *ptrDat = (THREAD_DATA *)(lpParam);

	//Wpisz swoj kod tu
	
	ptrDat->ret = 0;
	return 0; 
}
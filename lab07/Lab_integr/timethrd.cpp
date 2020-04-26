///////////////////////////////////////////////////////////////////////////////////////////
//    timethrd.cpp

#include "stdafx.h"
#include <iostream>
#include "timethrd.h"

using namespace std;

void CThreadTime::begin()
{
	GetThreadTimes(GetCurrentThread(), &ftDummy, &ftDummy, &ftKernalTimeStart, &ftUserTimeStart);
}

void CThreadTime::getelapstime()
{
	GetThreadTimes(GetCurrentThread(), &ftCreation, &ftDummy, &ftKernalTimeEnd, &ftUserTimeEnd);
	qwKernalTimeElapsed = FileTimeToQuadWord(&ftKernalTimeEnd)-FileTimeToQuadWord(&ftKernalTimeStart);
	qwUserTimeElapsed = FileTimeToQuadWord(&ftUserTimeEnd)-FileTimeToQuadWord(&ftUserTimeStart);
	qwTotalTimeElapsed = qwKernalTimeElapsed+qwUserTimeElapsed; 
	qwTimeStart = FileTimeToQuadWord(&ftCreation);
	qwTimeEnd = qwTimeStart+qwTotalTimeElapsed;
}


void ThreadTimeInfo(int np, CThreadTime *thrd_timer)
{
	int ip;
	cout << "Thread time information\n";
	for(ip=1; ip<=np; ip++)
	{
		cout << "Thread " << ip+1;
		cout << " KernalTime " << thrd_timer[ip].get_kernaltime()/10 << " mks ";
		cout << " UserTime "   << thrd_timer[ip].get_usertime()/10   << " mks ";
		cout << " TotalTime "  << thrd_timer[ip].get_totaltime()/10  << " mks " << endl;
	}

	__int64 MinTimeStart = thrd_timer[1].get_timestart();
	__int64 MaxTimeEnd   = thrd_timer[1].get_timeend();
	for(ip=2; ip<=np; ip++)
	{
		if(MinTimeStart > thrd_timer[ip].get_timestart())
			MinTimeStart = thrd_timer[ip].get_timestart();
		if(MaxTimeEnd < thrd_timer[ip].get_timeend())
			MaxTimeEnd = thrd_timer[ip].get_timeend();
	}

	cout << endl;
	cout << "Total Thread time :" << (MaxTimeEnd-MinTimeStart)/10 << " mks" << endl;
	
}
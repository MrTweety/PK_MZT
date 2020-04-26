////////////////////////////////////////////////////////////////////////////////////
//      timethrd.h
//      GetThreadTime service

#pragma once

#include "windows.h"
#include <sys\timeb.h>

class CThreadTime
{
	FILETIME ftKernalTimeStart, ftKernalTimeEnd;
	FILETIME ftUserTimeStart, ftUserTimeEnd;
	FILETIME ftDummy, ftCreation;
	__int64 qwKernalTimeElapsed, qwUserTimeElapsed, qwTotalTimeElapsed;
	__int64 qwTimeStart, qwTimeEnd;

public:
	void begin();
	void getelapstime();
	__int64 get_kernaltime() { return qwKernalTimeElapsed;}
	__int64 get_timestart()  { return qwTimeStart;}
	__int64 get_timeend()    { return qwTimeEnd;}
	__int64 get_usertime()   { return qwUserTimeElapsed;}
	__int64 get_totaltime()  { return qwTotalTimeElapsed;}
	__int64 FileTimeToQuadWord(PFILETIME pft) 
	    {return(Int64ShllMod32(pft->dwHighDateTime, 32) | pft->dwLowDateTime);}
};

void ThreadTimeInfo(int np, CThreadTime *thrd_timer);
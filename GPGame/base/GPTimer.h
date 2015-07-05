#ifndef _TIMER_H_
#define _TIMER_H_

#include <time.h>

typedef clock_t time_type;

time_type getTimeClock()
{
	return clock();
}

float getDeltaTimeInSec(time_type start, time_type end)
{
	return (float)(end - start) / CLOCKS_PER_SEC;
}

#endif
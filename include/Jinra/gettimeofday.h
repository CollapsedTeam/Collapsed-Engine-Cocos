#pragma once 

#if defined(_WIN32) && !defined(__GNUC__)  &&!defined(__GCCXML__)
#include <time.h>
struct timezone
{
	s32 tz_minuteswest; /* minutes W of Greenwich */
	s32 tz_dsttime;     /* type of dst correction */
};

s32 gettimeofday(struct timeval* tv, struct timezone* tz);

#else
// Uncomment this if you need to
/*
// http://www.halcode.com/archives/2008/08/26/retrieving-system-time-gettimeofday/
struct timezone
{
  int tz_minuteswest;
  int tz_dsttime;
};
 
#ifdef	__cplusplus
 
void  GetSystemTimeAsFileTime(FILETIME*);
 
inline int gettimeofday(struct timeval* p, void* tz )
{
	union {
	    long long ns100; // time since 1 Jan 1601 in 100ns units
		FILETIME ft;
	} now;
 
    GetSystemTimeAsFileTime( &(now.ft) );
    p->tv_usec=(long)((now.ns100 / 10LL) % 1000000LL );
    p->tv_sec= (long)((now.ns100-(116444736000000000LL))/10000000LL);
	return 0;
}
 
#else
	int gettimeofday(struct timeval* p, void* tz );
#endif
*/

#endif


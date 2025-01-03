#ifndef KENKLOCK_H
#define KENKLOCK_H

#ifdef __WATCOMC__
	__int64 rdtsc64 ();
	#pragma aux rdtsc64 = "rdtsc" value [edx eax] modify nomemory parm nomemory;
#endif

#ifdef _MSC_VER
	static __forceinline __int64 rdtsc64 () { _asm rdtsc }
#endif

static __int64 pertimbase, rdtimbase, nextimstep;
static double perfrq, klockmul, klockadd;

static 
void initklock ()
{
	__int64 q;
	QueryPerformanceFrequency((LARGE_INTEGER *)&q);
	perfrq = (double)q;
	rdtimbase = rdtsc64();
	QueryPerformanceCounter((LARGE_INTEGER *)&pertimbase);
	nextimstep = 4194304; klockmul = 0.000000001; klockadd = 0.0;
}

static 
void readklock (double *tim)
{
	__int64 q = rdtsc64()-rdtimbase;
	if (q > nextimstep)
	{
		__int64 p;
		double d;
		QueryPerformanceCounter((LARGE_INTEGER *)&p);
		d = klockmul; klockmul = ((double)(p-pertimbase))/(((double)q)*perfrq);
		klockadd += (d-klockmul)*((double)q);
		do { nextimstep <<= 1; } while (q > nextimstep);
	}
	(*tim) = ((double)q)*klockmul + klockadd;
}

#endif /* KENKLOCK_H */

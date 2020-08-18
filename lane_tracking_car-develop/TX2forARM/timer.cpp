#include <iostream>
#include <string>
#include "timer.h"
#if _WIN32 || _WIN64
    #include <Windows.h>
#endif
#if __GNUC__
    #include <sys/time.h>
#endif
using namespace std;

#if _WIN32 || _WIN64
    
long long Timer::milliseconds_now() {
	static LARGE_INTEGER s_frequency;
	static bool s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	} else {
		return GetTickCount();
	}
}

#endif

#if __GNUC__

long long Timer::milliseconds_now() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time, the timezone parameter is ignored
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

#endif

void Timer::start() {
	tic = milliseconds_now();
}

long long Timer::stop() {
	toc = milliseconds_now();
    return toc - tic;   // ms
}

void Timer::display(string str) {
	toc = milliseconds_now();
	cout << "Elapsed time(" << str << "): " << toc - tic << "ms.\n";
}

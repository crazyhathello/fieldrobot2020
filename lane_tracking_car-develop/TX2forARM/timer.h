#ifndef TIMER_H
#define TIMER_H

#include <string>
using namespace std;

class Timer{
public:
	Timer() {}
	~Timer() {}
	void start();
	long long stop();
	void display(string str);
private:
	long long tic;
	long long toc;
	long long milliseconds_now();
};


#endif
#include <iostream>
#include <windows.h>
#include "chrono.h"

Chrono::Chrono():_freq(),_counter(),_numSamples(),_acc()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cout << "QueryPerformanceFrequency failed!\n";

	_freq = double(li.QuadPart)/1000.0;
	std::cout<<"Frequency: "<<_freq<<'\n';
}

Chrono::~Chrono()
{
	std::cout<<"Average time: "<<Result()<<" ms ("<<_numSamples<<" Samples)"<<'\n';
}

void Chrono::Start()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	_counter=li.QuadPart;
	_numSamples++;
}

void Chrono::Stop()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double t=double(li.QuadPart-_counter)/_freq;
	_acc+=t;
	//std::cout<<t<<'\n';
}

double Chrono::Result()
{
	std::cout<<_acc<<" "<<_numSamples<<'\n';
	double r=_acc/_numSamples;
	_acc=0.0;
	_numSamples=0;
	return r;
}

#ifndef CHRONO_H
#define CHRONO_H

#include <cstdint>

class Chrono
{
public:
	Chrono();
	~Chrono();
	void Start();
	void Stop();
	double Result();
protected:
private:
	double _freq;
	int64_t _counter;
	int _numSamples;
	double _acc;
};

#endif // CHRONO_H

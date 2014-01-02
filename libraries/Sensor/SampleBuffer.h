#ifndef __SAMPLEBUFFER_H__
#define __SAMPLEBUFFER_H__

class SampleBuffer
{
public:
	SampleBuffer();
	void addSample(float sample);
	float getAverage();
	void clear();

private:
	static const int NUM_SAMPLES = 11;
	float samples[11];
};

#endif
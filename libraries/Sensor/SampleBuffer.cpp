#include "SampleBuffer.h"
#include <math.h>

SampleBuffer::SampleBuffer()
{
	clear();
}

void SampleBuffer::addSample(float sample)
{
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		if (isnan(samples[i]))
		{
			samples[i] = sample;
			samples[(i + 1) % NUM_SAMPLES] = NAN;
			break;
		}
	}
}

float SampleBuffer::getAverage()
{
	float accumulator = 0.f;
	int numSamples = 0;
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		if (!isnan(samples[i]))
		{
			accumulator += samples[i];
			numSamples++;
		}
	}
	return accumulator / numSamples;
}

void SampleBuffer::clear()
{
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		samples[i] = NAN;
	}
}
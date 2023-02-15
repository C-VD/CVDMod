#pragma once

#include <math.h>
#include "CutOffFilter.h"

#define Z_BUF_SIZE 20
#define PI_4 3.1416
#define TWO_PI 3.1416 * 2
class ModProcessor
{
public:
	void process(double* in, double* out, int nsamples);
	inline void setDepth(double pDepth) { mLfoDepth = pDepth; };
	inline void setLfoRate(double pLfoDepth) { mLfoDepth = pLfoDepth; };
	inline void setLowFreq(double pLowFreq) { mLowFreq = pLowFreq; };
	inline void setHighFreq(double pHighFreq) { mHighFreq = pHighFreq; };
	inline void setSampleRate(int pSampleRate) { mSampleRate = pSampleRate; };
	inline void setWet(double pWet) { mWetAtt = pWet; mDryAtt = 1 - pWet; };
	void setBandPassFreq(double pCutFreq);
	CutOffFilter highCutOffFilter;
	CutOffFilter lowCutOffFilter;
private:
	int lfoStep();
	int mSampleRate;
	int mLfoRate;
	double mLfoDepth;
	double mLfoPhase = 0;
	int mLowFreq;
	int mHighFreq;
	double mDryAtt;
	double mWetAtt;
	double zRingBuffer[Z_BUF_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double zBandPassBuffer[8] = { 0,0,0,0,0,0,0,0 };
	int zRingBufferWritePtr = 0;
	int zRingBufferReadPtr = 0;
	int nZDelay = 1;

};
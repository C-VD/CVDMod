#include "ModProcessor.h"

void ModProcessor::process(double* in, double* out, int nsamples) {
	for (int i = 0; i < nsamples; i++) {
		// bandpass
		double bpout;
		highCutOffFilter.lpass(in, &bpout);
		lowCutOffFilter.hpass(&bpout, &bpout);
		nZDelay = lfoStep();
		if (zRingBufferWritePtr >= Z_BUF_SIZE) zRingBufferWritePtr -= Z_BUF_SIZE;
		zRingBufferReadPtr = zRingBufferWritePtr - nZDelay;
		if (zRingBufferReadPtr < 0) zRingBufferReadPtr += Z_BUF_SIZE;
		*out = zRingBuffer[zRingBufferReadPtr];
		zRingBuffer[zRingBufferWritePtr] = bpout;
		*out = *out * mWetAtt + -*in * mDryAtt;
		in++;
		out++;
		zRingBufferReadPtr++;
		zRingBufferWritePtr++;
	}
}

int ModProcessor::lfoStep() {
	double x;
	while (mLfoPhase >= TWO_PI)
	{
		mLfoPhase -= TWO_PI;
	}
	x = sin(mLfoPhase);
	mLfoPhase += (mLfoRate * TWO_PI / mSampleRate);

	return (x + 1) * (mLfoDepth * (Z_BUF_SIZE - 1) / 2) + 1;
}
	
void ModProcessor::setHighFreq(double pCutFreq) {
	double nc = (pCutFreq / mSampleRate);
	highCutOffFilter.setNc(nc);
	double c = (tan(PI_4 * nc) - 1) / (tan(PI_4 * nc) + 1);
	highCutOffFilter.setC(c);
}
void ModProcessor::setLowFreq(double pCutFreq) {
	double nc = (pCutFreq / mSampleRate);
	lowCutOffFilter.setNc(nc);
	double c = (tan(PI_4 * nc) - 1) / (tan(PI_4 * nc) + 1);
	lowCutOffFilter.setC(c);
}



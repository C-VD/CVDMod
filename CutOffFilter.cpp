#include "CutOffFilter.h"

void CutOffFilter::hpass(double *in, double *out) {
	*out = *in - apf(*in);
}

void CutOffFilter::lpass(double *in, double *out) {
	*out = (*in + apf(*in)) / 2;
}

// All-pass filter.
double CutOffFilter::apf(double in) {
	zStack[0] = in * mC + zStack[1] + zStack[2] * -mC;
	zStack[2] = zStack[0];
	zStack[1] = in;
	return zStack[0];
}
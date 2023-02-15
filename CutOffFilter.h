#pragma once
class CutOffFilter
{
public:
	inline void setNc(double pNc) { mNc = pNc; }
	inline void setC(double pC) { mC = pC; }
	void hpass(double *in, double *out);
	void lpass(double *in, double *out);
private:
	double mC;
	double mNc;
	double apf(double in);
	double zStack[3]{ 0, 0, 0 };
};

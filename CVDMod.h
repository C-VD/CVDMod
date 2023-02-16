#ifndef __CVDMOD__
#define __CVDMOD__

#include "IPlug_include_in_plug_hdr.h"
#include "ModProcessor.h"

class CVDMod : public IPlug
{
public:
  CVDMod(IPlugInstanceInfo instanceInfo);
  ~CVDMod();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

  ModProcessor mModProcessorL;
  ModProcessor mModProcessorR;

private:
  double mWet;
  double mLastChengedValue;
};

#endif

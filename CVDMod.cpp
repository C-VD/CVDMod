#include "CVDMod.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 1;

enum EParams
{
  kRate = 0,
  kDepth,
  kHighFreq,
  kLowFreq,
  kWet,
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kRateX = 245,
  kRateY = 100,
  kDepthX = 60,
  kDepthY = 100,
  kHighFreqX = 245,
  kHighFreqY = 15,
  kLowFreqX = 60,
  kLowFreqY = 15,
  kKnobFrames = 60
};

CVDMod::CVDMod(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mWet(1.)
{
  TRACE;

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kRate)->InitDouble("Rate", 5., 0., 20.0, 0.01, "Hz");
  GetParam(kRate)->SetShape(2.);
  GetParam(kDepth)->InitDouble("Depth", 50., 0., 100.0, 1.00, "%");
  GetParam(kDepth)->SetShape(2.);
  GetParam(kHighFreq)->InitDouble("HighFreq", 1000., 30., 20000.0, 1.00, "Hz");
  GetParam(kHighFreq)->SetShape(2.);
  GetParam(kLowFreq)->InitDouble("LowFreq", 1000., 30., 20000.0, 1.00, "Hz");
  GetParam(kLowFreq)->SetShape(2.);

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachBackground(BG_ID, BG_FN);

  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);

  pGraphics->AttachControl(new IKnobMultiControl(this, kRateX, kRateY, kRate, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kDepthX, kDepthY, kDepth, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kHighFreqX, kHighFreqY, kHighFreq, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kLowFreqX, kLowFreqY, kLowFreq, &knob));

  AttachGraphics(pGraphics);

  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);

  mModProcessorL.setSampleRate(GetSampleRate());
  mModProcessorR.setSampleRate(GetSampleRate());


}

CVDMod::~CVDMod() {}

void CVDMod::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.

  double* in1 = inputs[0];
  double* in2 = inputs[1];
  double* out1 = outputs[0];
  double* out2 = outputs[1];

  mModProcessorL.process(in1, out1, nFrames);
  mModProcessorR.process(in2, out2, nFrames);

//  for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
//  {
//    *out1 = *in1 * mGain;
//    *out2 = *in2 * mGain;
//  }
}

void CVDMod::Reset()
{
  TRACE;
  IMutexLock lock(this);
}

void CVDMod::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    case kWet:
      mLastChengedValue = GetParam(kWet)->Value() / 200.;
//      mModProcessorL.setWet(mLastChengedValue);
//      mModProcessorR.setWet(mLastChengedValue);
    case kRate:
      mLastChengedValue = GetParam(kRate)->Value();
      mModProcessorL.setRate(mLastChengedValue);
      mModProcessorR.setRate(mLastChengedValue);
      break;
    case kDepth:
      mLastChengedValue = GetParam(kDepth)->Value() / 100;
      mModProcessorL.setDepth(mLastChengedValue);
      mModProcessorR.setDepth(mLastChengedValue);
      break;
    case kHighFreq:
      mLastChengedValue = GetParam(kHighFreq)->Value();
      mModProcessorL.setHighFreq(mLastChengedValue);
      mModProcessorR.setHighFreq(mLastChengedValue);
      break;
    case kLowFreq:
      mLastChengedValue = GetParam(kLowFreq)->Value();
      mModProcessorL.setLowFreq(mLastChengedValue);
      mModProcessorR.setLowFreq(mLastChengedValue);
      break;
    default:
      break;
  }
}

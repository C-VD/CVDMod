#include "CVDMod.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 1;

enum EParams
{
  kWet = 0,
  kDepth,
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kWetX = 100,
  kWetY = 100,
  kDepthX = 200,
  kDepthY = 100,
  kKnobFrames = 60
};

CVDMod::CVDMod(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mWet(1.)
{
  TRACE;

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kWet)->InitDouble("Dry/Wet", 50., 0., 100.0, 0.01, "%");
  GetParam(kWet)->SetShape(1.);
  GetParam(kDepth)->InitDouble("Depth", 50., 0., 100.0, 1.00, "%");
  GetParam(kDepth)->SetShape(2.);

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachPanelBackground(&COLOR_RED);

  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);

  pGraphics->AttachControl(new IKnobMultiControl(this, kWetX, kWetY, kWet, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, kDepthX, kDepthY, kDepth, &knob));

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
      mWet = GetParam(kWet)->Value() / 200.;
      mModProcessorL.setWet(mWet);
      mModProcessorR.setWet(mWet);
    case kDepth:
      mDepth = GetParam(kDepth)->Value() / 100;
      mModProcessorL.setDepth(mDepth);
      mModProcessorR.setDepth(mDepth);
      break;

    default:
      break;
  }
}

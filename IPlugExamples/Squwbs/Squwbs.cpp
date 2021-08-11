#include "Squwbs.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstddef>

const int kNumPrograms = 1;

const double f1=150.0;
enum EParams
{
  kGain = 0,
  kNumParams
};

double LP6::process(double inputValue) {
  buf0 += cutoff * (inputValue - buf0);
  return buf0;
}

enum ELayout
{
  kWidth = 175,
  kHeight = 175,

  kGainX = 12,
  kGainY = 12,
//  kKnobFrames = 60
  kKnobFrames = 128
};

Squwbs::Squwbs(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mGain(1.)
{
  TRACE;

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kGain)->InitDouble("Dry/Wet", 50., 0., 100.0, 0.01, "%");
  GetParam(kGain)->SetShape(2.);

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachPanelBackground(&COLOR_WHITE);

  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);

  pGraphics->AttachControl(new IKnobMultiControl(this, kGainX, kGainY, kGain, &knob));

  AttachGraphics(pGraphics);

  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);
}

Squwbs::~Squwbs() {}

void Squwbs::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.

  double* in1 = inputs[0];
  double* in2 = inputs[1];
  double* out1 = outputs[0];
  double* out2 = outputs[1];
  
  
  for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
  {
    
//    *out1 = filter1.process(*in1) * mGain + *in1*(1.0-mGain);
//    *out2 = filter2.process(*in2) * mGain + *in2*(1.0-mGain);
    
    
    
//    float* sample=filter3.match(*in1,*in2);
//    *out1 = sample[0]*mGain+*in1*(1.0-mGain);
//    *out2= sample[1]*mGain+*in2*(1.0-mGain);
    
    double increment = (mGain-prevGain)/double(nFrames);
    
    if(mGain==prevGain){
      float* sample=filter3.match(*in1,*in2);
      *out1 = sample[0]*toLinear(18.0)*mGain+*in1*(1.0-mGain);
      *out2= sample[1]*toLinear(18.0)*mGain+*in2*(1.0-mGain);
    }
    else{
      prevGain=prevGain+increment;
      float* sample=filter3.match(*in1,*in2);
      *out1 = sample[0]*12.0*prevGain+*in1*(1.0-prevGain);
      *out2= sample[1]*12.0*prevGain+*in2*(1.0-prevGain);
    }
  }
}

void Squwbs::Reset()
{
  TRACE;
  sr1=GetSampleRate();
  fq1=2*sin((PI)*f1/sr1);
  filter1.set(fq1);
  filter2.set(fq1);
  IMutexLock lock(this);
  filter3.setSampleRate(sr1);
  prevGain=1.0;
}

void Squwbs::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    case kGain:
      mGain = GetParam(kGain)->Value() / 100.;
      break;

    default:
      break;
  }
}

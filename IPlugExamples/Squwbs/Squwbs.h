#ifndef __SQUWBS__
#define __SQUWBS__
#define SR (44100.f);
//#define F_PI(3.14159f);
#include "IPlug_include_in_plug_hdr.h"
#include <math.h>
class LP6 {
public:
  LP6() :
  cutoff(0.1),
  resonance(0.0),
  buf0(0.0)
  {
    calculateFeedbackAmount();
  };
  double process(double inputValue);
  inline void set(double newCutoff) { cutoff = newCutoff; calculateFeedbackAmount(); };
  inline void setResonance(double newResonance) { resonance = newResonance; calculateFeedbackAmount(); };
private:
  double cutoff;
  double resonance;
  double feedbackAmount;
  inline void calculateFeedbackAmount() { feedbackAmount = resonance + resonance/(1.0 - cutoff); }
  double buf0;
};
class LP12
{
public:
  LP12():
  cutoff(750.0),
  resonance(0.0),
  buf0(0.0),
  buf1(0.0),
  sampleRate(44100.0)
  {
    calculateFeedbackAmount();
  }
  
  ~LP12()
  {
  }
  
  
  float process(float inputValue){
    buf0 += cutoff * (inputValue - buf0);
    buf1 += cutoff * (buf0 - buf1);
    return buf1;
  };
  
  void set(float newCutoff){
    cutoff = 2*sin((PI)*newCutoff/sampleRate);
    calculateFeedbackAmount();
  };
  void setResonance(float newResonance){
    resonance = newResonance; calculateFeedbackAmount();
  };
  void setSampleRate(float sr){
    sampleRate=sr;
  };
private:
  float cutoff;
  float resonance;
  float feedbackAmount;
  void calculateFeedbackAmount() {
    feedbackAmount = resonance + resonance/(1.0 - cutoff);
  };
  float buf0;
  float buf1;
  float sampleRate;
};

class LP24
{
public:
  LP24():
  cutoff(750.0),
  resonance(0.0),
  buf0(0.0),
  buf1(0.0),
  buf2(0.0),
  buf3(0.0)
  {
    calculateFeedbackAmount();
  }
  
  ~LP24()
  {
  }
  
  float process(float inputValue){
    buf0 += cutoff * (inputValue - buf0);
    buf1 += cutoff * (buf0 - buf1);
    buf2 += cutoff * (buf1 - buf2);
    buf3 += cutoff * (buf2 - buf3);
    return buf3;
  };
  void set(float newCutoff) {
    
    cutoff = 2*sin((PI)*newCutoff/sampleRate);
    calculateFeedbackAmount();
  };
  void setResonance(float newResonance){
    resonance = newResonance;
    calculateFeedbackAmount();
  };
  void setSampleRate(float sr){
    sampleRate=sr;
  };
private:
  float cutoff;
  float resonance;
  float feedbackAmount;
  void calculateFeedbackAmount() {
    feedbackAmount = resonance + resonance/(1.0 - cutoff);
  };
  float buf0;
  float buf1;
  float buf2;
  float buf3;
  float sampleRate;
};

class HP12
{
public:
  HP12():
  cutoff(750.0),
  resonance(0.0),
  buf0(0.0),
  buf1(0.0),
  sampleRate(44100.0)
  {
    calculateFeedbackAmount();
  }
  
  ~HP12()
  {
  }
  
  float process(float inputValue){
    buf0 += cutoff * (inputValue - buf0 + feedbackAmount * (buf0 - buf1));
    //buf0 += cutoff * (inputValue - buf0);
    buf1 += cutoff * (buf0 - buf1);
    return inputValue - buf1;
  }
  void set(float newCutoff) {
    
    cutoff = 2*sin((PI)*newCutoff/sampleRate);
    calculateFeedbackAmount();
  }
  void setResonance(float newResonance){
    resonance = newResonance;
    calculateFeedbackAmount();
  }
  void setSampleRate(float sr){
    sampleRate=sr;
  }
private:
  float cutoff;
  float resonance;
  float feedbackAmount;
  void calculateFeedbackAmount() {
    feedbackAmount = resonance + resonance/(1.0 - cutoff);
  }
  float sampleRate;
  float buf0;
  float buf1;
};

class Clipper
{
public:
  Clipper()
  {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
  }
  
  ~Clipper()
  {
  }
  
  
  float process(float inputValue) {
    k=atanf(inputValue*0.5)/atanf(1.0);
    float i = atanf(powf(abs(inputValue), 100));
    //k = sign(inputValue)*powf(i, (1.0 / 100.0));
    return k;
  }
  
private:
  float k;
  float sign(float a) {
    if (a < 0) {
      return (-1.0);
    }
    else {
      return (1.0);
    }
  };
};

class Gate
{
public:
  Gate():
  releaseTime(0.2),
  sr(44100),
  threshold(0.015),
  outputValue(0.0),
  gain(1.0),
  holdTime(1.0),
  attackTime(0.01)
  {
    set(44100.0);
  }
  
  ~Gate()
  {
  }
  
  
  double process(double inputValue){
    if(tick>int(hold)){
      if(threshold>(inputValue*inputValue)){
        gain*=release;
      }
      else{
        tick=0;
        gain*=attack;
      }
    }
    else{
      tick+=1;
      gain=1.0;
    }
    outputValue=inputValue*gain;
    return outputValue;
  }
  void set(float sampleRate){
    sr=sampleRate;
    releaseTime=0.3;
    attackTime=0.03;
    threshold=0.0000001;
    release=1.0-exp(-1.0/(releaseTime*sr));
    hold=holdTime*sr;
    //attack=1.0-exp(-1.0/(attackTime*sr));
    attack=1.0-exp(-1.0/1.0);
  }
  void setThreshold(double newThreshold){
    threshold=newThreshold;
  }
private:
  double threshold;
  double releaseTime;
  double sr;
  double release;
  double outputValue;
  double gain;
  int tick;
  double hold;
  double holdTime;
  double attack;
  double attackTime;
};

double toLinear(double dB){
  //return powf(10.0, dB);
  return powf(powf(2.0,1/6),(dB-6.0));
}
class ThreeBandEQ
{
public:
  ThreeBandEQ()
  {
    setSampleRate(44100);
    setGains(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
    //setRatios(0.75, 0.75, 0.5, 0.5, 0.25, 0.25),
    setRatios(1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
    setMidHP(20.0);
    setSideHP(200.0);
    setMidLMC(175.0);
    setSideLMC(175.0);
    setMidMHC(2750.0);
    setSideMHC(2750.0);
    setMidLP(2250.0);
    setSideLP(6000.0);
    resetMatch();
    //setGoals(0.5,0.5,0.5,0.5,0.5,0.5,0.25,0.25,0.25,0.25,0.25,0.25);
    
    reset();
    //below is the data from eot
    setGoals(0.068,0.031,0.039,0.043,0.035,0.038,0.031,0.031,0.031,0.063,0.063,0.063);
    //below is the data from starbucks jongro(rms/160000.0)
    /*
     gate1.setThreshold(0.00000026875);
     gate2.setThreshold(0.00000021875);
     gate3.setThreshold(0.0000002375);
     gate4.setThreshold(0.000000425);
     gate5.setThreshold(0.00000019375);
     gate6.setThreshold(0.00000024375);
     gate7.setThreshold(0.00000026875);
     gate8.setThreshold(0.00000021875);
     gate9.setThreshold(0.0000002375);
     */
    
  }
  
  ~ThreeBandEQ()
  {
  }
  void setMidHP(float freq) {
    midHP.set(freq);
  }
  void setSideHP(float freq) {
    leftHP.set(freq);
    rightHP.set(freq);
  }
  void setMidLP(float freq) {
    midLP.set(freq);
  }
  void setSideLP(float freq) {
    leftLP.set(freq);
    rightLP.set(freq);
  }
  void setMidLMC(float freq){
    midLPL.set(freq);
    midHPM.set(freq);
  }
  void setSideLMC(float freq){
    leftLPL.set(freq);
    rightLPL.set(freq);
    leftHPM.set(freq);
    rightHPM.set(freq);
  }
  
  void setMidMHC(float freq){
    midLPM.set(freq);
    midHPH.set(freq);
  }
  void setSideMHC(float freq){
    leftLPM1.set(freq);
    leftLPM2.set(freq);
    rightLPM1.set(freq);
    rightLPM2.set(freq);
    leftHPH.set(freq);
    rightHPH.set(freq);
  }
  
  float mid(float left, float right){
    return (left+right)/2.0;
  }
  
  float leftOnly(float left, float right){
    return left-mid(left,right);
  }
  
  float rightOnly(float left, float right){
    return right-mid(left,right);
  }
  void reset() {
    leftlowtmp = 0;
    leftmidtmp = 0;
    lefthightmp = 0;
    midlowtmp = 0;
    midmidtmp = 0;
    midhightmp = 0;
    rightlowtmp = 0;
    rightmidtmp = 0;
    righthightmp = 0;
    sidelowrms = 0;
    sidemidrms = 0;
    sidehighrms = 0;
    midlowrms = 0;
    midmidrms = 0;
    midhighrms = 0;
    sidelowsd = 0;
    sidemidsd = 0;
    sidehighsd = 0;
    m=0;
    l = 0;
    r = 0;
    midh = 0;
    midm = 0;
    midl = 0;
    lefth = 0;
    leftm = 0;
    leftl = 0;
    righth = 0;
    rightm = 0;
    rightl = 0;
    plhrms = 0;
    plmrms = 0;
    pllrms = 0;
    pmhrms = 0;
    pmmrms = 0;
    pmlrms = 0;
    prhrms = 0;
    prmrms = 0;
    prlrms = 0;
    plhsd = 0;
    plmsd = 0;
    pllsd = 0;
    pmhsd = 0;
    pmmsd = 0;
    pmlsd = 0;
    prhsd = 0;
    prmsd = 0;
    prlsd = 0;
    for (int i = 0; i < 8; i++) {
      processlefthigh[i] = 0.0;
      processleftmid[i] = 0.0;
      processleftlow[i] = 0.0;
      processmidhigh[i] = 0.0;
      processmidmid[i] = 0.0;
      processmidlow[i] = 0.0;
      processrighthigh[i] = 0.0;
      processrightmid[i] = 0.0;
      processrightlow[i] = 0.0;
    };
    index = 0;
  }
  void setGains(float m, float s,float hm,float mm, float lm, float hs, float ms, float ls){
    midGain=m;
    sideGain=s;
    highMidGain=hm;
    midMidGain=mm;
    lowMidGain=lm;
    highSideGain=hs;
    midSideGain=ms;
    lowSideGain=ls;
  }
  void setRatios(float hm, float mm, float lm, float hs, float ms, float ls) {
    highMidRatio=1.0;
    highSideRatio=1.0;
    midMidRatio=1.0;
    midSideRatio=1.0;
    lowMidRatio=1.0;
    lowSideRatio=1.0;
  }
  
  
  void setSampleRate(float sr){
    leftLPL.setSampleRate(sr);
    rightLPL.setSampleRate(sr);
    midLPL.setSampleRate(sr);
    
    leftLPM1.setSampleRate(sr);
    leftLPM2.setSampleRate(sr);
    rightLPM1.setSampleRate(sr);
    rightLPM2.setSampleRate(sr);
    midLPM.setSampleRate(sr);
    
    leftLP.setSampleRate(sr);
    rightLP.setSampleRate(sr);
    midLP.setSampleRate(sr);
    
    leftHPM.setSampleRate(sr);
    rightHPM.setSampleRate(sr);
    midHPM.setSampleRate(sr);
    
    leftHPH.setSampleRate(sr);
    rightHPH.setSampleRate(sr);
    midHPH.setSampleRate(sr);
    
//    phaser1.Rate(0.002, sr);
//    phaser2.Rate(0.002, sr);
    gate1.set(sr);
    gate2.set(sr);
    gate3.set(sr);
    //gate4.set(sr);
    //gate5.set(sr);
    //gate6.set(sr);
    //gate7.set(sr);
    //gate8.set(sr);
    //gate9.set(sr);
    
  }
  
  /*
   float* process(float left, float right){
   
   float m=midHP.process(midLP.process(mid(phaser1.Update(gate1.process(left)), phaser1.Update(gate2.process(right)))))*midGain;
   float l=leftHP.process(leftLP.process(leftOnly(phaser1.Update(gate1.process(left)), phaser1.Update(gate2.process(right)))))*sideGain;
   float r=rightHP.process(rightLP.process(rightOnly(phaser1.Update(gate1.process(left)), phaser1.Update(gate2.process(right)))))*sideGain;
   float midh=midHPH.process(m)*highMidGain;
   float midm=midHPM.process(midLPM.process(m))*midMidGain;
   float midl=midLPL.process(midLPM.process(m))*lowMidGain;
   float lefth=leftHPH.process(l)*highSideGain;
   float leftm=leftHPM.process(midLPM.process(l))*midSideGain;
   float leftl=leftLPL.process(midLPM.process(l))*lowSideGain;
   float righth=rightHPH.process(r)*highSideGain;
   float rightm=rightHPM.process(midLPM.process(r))*midSideGain;
   float rightl=rightLPL.process(midLPM.process(r))*lowSideGain;
   
   
   output[0] = clipper1.process(sin(float_Pi*(highMidRatio*midh + midMidRatio*midm + lowMidRatio*midl + highSideRatio*lefth + midSideRatio*leftm + lowSideRatio*leftl)/2.0));
   output[1] = clipper2.process(sin(float_Pi*(highMidRatio*midh + midMidRatio*midm + lowMidRatio*midl + highSideRatio*righth + midSideRatio*rightm + lowSideRatio*rightl)/2.0));
   
   return output;
   }
   */
  void runStats(float* left, float*right, int length) {
    for (int i=0; i<length; i++){
      m = midHP.process(midLP.process(mid(left[i], right[i])));
      l = leftHP.process(leftOnly(left[i], right[i]));
      r = rightHP.process(rightOnly(left[i], right[i]));
      
      midh = midHPH.process(m);
      midm = midHPM.process(midLPM.process(m));
      midl = midLPL.process((m));
      
      lefth = leftHPH.process(l);
      leftm = leftHPM.process(leftLPM1.process(l));
      leftl = leftLPL.process((l));
      righth = rightHPH.process(r);
      rightm = rightHPM.process(rightLPM1.process(r));
      rightl = rightLPL.process((r));
      
      sidehighrms += (abs(lefth)+abs(righth)) / (2.0*length);
      sidemidrms += (abs(leftm)+abs(rightm)) / (2.0*length);
      sidelowrms += (abs(leftl)+abs(rightl)) / (2.0*length);
      
      midhighrms += (abs(midh) / length);
      midmidrms += (abs(midm) / length);
      midlowrms += (abs(midl) / length);
      maximum = fmax(fabs(left[i]),fabs(right[i]));
    }
    for (int i = 0; i < length; i++) {
      
      sidehighsd += abs(abs(lefth) + abs(righth) - (2 * sidehighrms)) / (2.0*length);
      sidemidsd += abs(abs(leftm) + abs(rightm) - (2 * sidemidrms)) / (2.0*length);
      sidelowsd += abs(abs(leftl) + abs(rightl) - (2 * sidelowrms)) / (2.0*length);
      midhighsd += abs(abs(midh) - sidehighrms) / length;
      midmidsd += abs(abs(midm) - midmidrms) / length;
      midlowsd += abs(abs(midl) - midlowrms) / length;
      
    };
    
    
    
    
    
  };
  void setGoals(float gmhrms, float gmmrms, float gmlrms, float gshrms, float gsmrms, float gslrms, float gmhsd, float gmmsd, float gmlsd, float gshsd, float gsmsd, float gslsd ) {
    
    midhighrms = gmhrms;
    midmidrms = gmmrms;
    midlowrms = gmlrms;
    sidehighrms = gshrms;
    sidemidrms = gsmrms;
    sidelowrms = gslrms;
    midhighsd = gmhsd;
    midmidsd = gmmsd;
    midlowsd = gmlsd;
    sidehighsd = gshsd;
    sidemidsd = gsmsd;
    sidelowsd = gslsd;
    
  }
  float* match(float left, float right) {
    /*
     m = midHP.process(midLP.process(mid(phaser1.Update(gate1.process(left)), phaser1.Update(gate2.process(right)))))*midGain;
     l = leftHP.process(leftLP.process(leftOnly(phaser1.Update(gate1.process(left)), phaser1.Update(gate2.process(right)))))*sideGain;
     r = rightHP.process(rightLP.process(rightOnly(phaser1.Update(gate1.process(left)), phaser1.Update(gate2.process(right)))))*sideGain;
     */
    
    /*
     m = midHP.process(midLP.process(mid(gate1.process(left), gate2.process(right))));
     l = leftHP.process(leftLP.process(leftOnly(gate1.process(left), gate2.process(right))));
     r = rightHP.process(rightLP.process(rightOnly(gate1.process(left), gate2.process(right))));
     */
    /*
     l = leftHP.process(leftOnly(left, right));
     m = midHP.process(midLP.process(mid(left, right)));
     r = rightHP.process(rightOnly(left, right));
     lefth = gate1.process(leftHPH.process(l));
     leftm = gate2.process(leftHPM.process(leftLPM1.process(l)));
     leftl = gate3.process(leftLPL.process((l)));
     midh = gate4.process(midHPH.process(m));
     midm = gate5.process(midHPM.process(midLPM.process(m)));
     midl = gate6.process(midLPL.process((m)));
     righth = gate7.process(rightHPH.process(r));
     rightm = gate8.process(rightHPM.process(rightLPM1.process(r)));
     rightl = gate9.process(rightLPL.process((r)));
     */
    /*
     l = leftHP.process(gate1.process(leftOnly(left, right)));
     m = midHP.process(gate2.process(midLP.process(mid(left, right))));
     r = rightHP.process(gate3.process(rightOnly(left, right)));
     */
    l = leftHP.process(leftOnly(left, right));
    m = midHP.process(midLP.process(mid(left, right)));
    r = rightHP.process(rightOnly(left, right));
    
    lefth = leftHPH.process(l);
    leftm = leftHPM.process(leftLPM1.process(l));
    leftl = leftLPL.process(l);
    midh = midHPH.process(m);
    midm = midHPM.process(midLPM.process(m));
    midl = midLPL.process(m);
    righth = rightHPH.process(r);
    rightm = rightHPM.process(rightLPM1.process(r));
    rightl = rightLPL.process(r);
    
    if(index<8){
      processlefthigh[index] = lefth;
      processleftmid[index] = leftm;
      processleftlow[index] = leftl;
      processmidhigh[index] = midh;
      processmidmid[index] = midm;
      processmidlow[index] = midl;
      processrighthigh[index] = righth;
      processrightmid[index] = rightm;
      processrightlow[index] = rightl;
      plhrms += abs(lefth) / 8.0;
      plmrms += abs(leftm) / 8.0;
      pllrms += abs(leftl) / 8.0;
      pmhrms += abs(midh) / 8.0;
      pmmrms += abs(midm) / 8.0;
      pmlrms += abs(midl) / 8.0;
      prhrms += abs(righth) / 8.0;
      prmrms += abs(rightm) / 8.0;
      prlrms += abs(rightl) / 8.0;
      
      index++;
    }
    else {
      
      for (int i = 0; i < 8; i++) {
        plhsd += abs(processlefthigh[i] - plhrms) / 8.0;
        plmsd += abs(processleftmid[i] - plmrms) / 8.0;
        pllsd += abs(processleftlow[i] - pllrms) / 8.0;
        pmhsd += abs(processmidhigh[i] - pmhrms) / 8.0;
        pmmsd += abs(processmidmid[i] - pmmrms) / 8.0;
        pmlsd += abs(processmidlow[i] - pmlrms) / 8.0;
        prhsd += abs(processrighthigh[i] - prhrms) / 8.0;
        prmsd += abs(processrightmid[i] - prmrms) / 8.0;
        prlsd += abs(processrightlow[i] - prlrms) / 8.0;
      }
    }
    setGains(1.0, 0.25, midhighsd / pmhsd, midmidsd / pmmsd, midlowsd / pmlsd, sidehighsd / (plhsd / 2.0 + prhsd / 2.0), sidemidsd / (plmsd / 2.0 + prmsd / 2.0), sidelowsd / (pllsd / 2.0 + prlsd / 2.0));
    
    midh = matchBox(midh, pmhrms, midhighrms, midhighsd, pmhsd);
    midm = matchBox(midm, pmmrms, midmidrms, midmidsd, pmmsd);
    midl = matchBox(midl, pmlrms, midlowrms, midlowsd, pmlsd);
    lefth = matchBox(lefth, plhrms, sidehighrms, sidehighsd, plhsd);
    leftm = matchBox(leftm, plmrms, sidemidrms, sidemidsd, plmsd);
    leftl = matchBox(leftl, pllrms, sidelowrms, sidelowsd, pllsd);
    righth = matchBox(righth, prhrms, sidehighrms, sidehighsd, prhsd);
    rightm = matchBox(rightm, prmrms, sidemidrms, sidemidsd, prmsd);
    rightl = matchBox(rightl, prlrms, sidelowrms, sidelowsd, prlsd);
    //std::cout<< std::to_string(sin(float_Pi/2.0*(highMidGain*(midh) + midMidGain * (midm) + lowMidGain * (midl))*midGain + (highSideGain * (lefth) + midSideGain * (leftm) + lowSideGain * (leftl))*sideGain))<<std::endl;
    resetMatch();
    
    
    output[0] = sin(PI/2.0*(highMidGain*(midh) + midMidGain * (midm) + lowMidGain * (midl))*midGain + (highSideGain * (lefth) + midSideGain * (leftm) + lowSideGain * (leftl))*sideGain);
    output[1] = sin(PI/2.0*(highMidGain*(midh) + midMidGain * (midm) + lowMidGain * (midl))*midGain + (highSideGain * (righth) + midSideGain * (rightm) + lowSideGain * (rightl))*sideGain);
    
    return output;
  };
  void resetMatch() {
    for (int i=0; i<8; i++){
      processlefthigh[i] = 0.0;
      processleftmid[i] = 0.0;
      processleftlow[i] = 0.0;
      processmidhigh[i] = 0.0;
      processmidmid[i] = 0.0;
      processmidlow[i] = 0.0;
      processrighthigh[i] = 0.0;
      processrightmid[i] = 0.0;
      processrightlow[i] = 0.0;
    }
    
    plhrms = 0.5;
    plmrms = 0.5;
    pllrms = 0.5;
    pmhrms = 0.5;
    pmmrms = 0.5;
    pmlrms = 0.5;
    prhrms = 0.5;
    prmrms = 0.5;
    prlrms = 0.5;
    plhsd = 0.25;
    plmsd = 0.25;
    pllsd = 0.25;
    pmhsd = 0.25;
    pmmsd = 0.25;
    pmlsd = 0.25;
    prhsd = 0.25;
    prmsd = 0.25;
    prlsd = 0.25;
    index = 0;
  }
  float matchBox(float input, float originalRms, float targetRms, float sd1, float sd2) {
    /*
     if (input ==0.0 ){
     return input;
     }
     else {
     return input - originalRms + targetRms;
     }
     */
    
    /*
     if (input > targetRms) {
     return input - originalRms + targetRms;
     }
     else if (input < -1.0*0.0001) {
     return input + originalRms - targetRms;
     }
     else {
     return input;
     }
     */
    if(targetRms>originalRms){
      if(input>0.0){
        return input-originalRms*sd1/sd2+targetRms;
      }
      else if(input<0.0){
        return input+originalRms*sd1/sd2-targetRms;
      }
      
      else{
        return input;
      }
    }
    else if(targetRms<originalRms){
      if(input>originalRms){
        return input-originalRms*sd1/sd2+targetRms;
      }
      else if(input<(-1.0)*(originalRms)){
        return input+originalRms*sd1/sd2-targetRms;
      }
      else{
        return input;
      }
    }
    else{
      return input;
    }
    
  }
  
  float midGain;
  float sideGain;
  float midLMC;
  float sideLMC;
  float midMHC;
  float sideMHC;
  float midLPF;
  float sideLPF;
  float m;
  float l;
  float r;
  float midh;
  float midm;
  float midl;
  float lefth;
  float leftm;
  float leftl;
  float righth;
  float rightm;
  float rightl;
  float highMidGain;
  float highSideGain;
  float midMidGain;
  float midSideGain;
  float lowMidGain;
  float lowSideGain;
  float highMidRatio;
  float highSideRatio;
  float midMidRatio;
  float midSideRatio;
  float lowMidRatio;
  float lowSideRatio;
  float output[2];
  float leftlowtmp;
  float leftmidtmp;
  float lefthightmp;
  float midlowtmp;
  float midmidtmp;
  float midhightmp;
  float rightlowtmp;
  float rightmidtmp;
  float righthightmp;
  float sidelowrms;
  float sidemidrms;
  float sidehighrms;
  float midlowrms;
  float midmidrms;
  float midhighrms;
  float sidelowsd;
  float sidemidsd;
  float sidehighsd;
  float midlowsd;
  float midmidsd;
  float midhighsd;
  float processlefthigh[8];
  float processleftmid[8];
  float processleftlow[8];
  float processmidhigh[8];
  float processmidmid[8];
  float processmidlow[8];
  float processrighthigh[8];
  float processrightmid[8];
  float processrightlow[8];
  float plhrms;
  float plmrms;
  float pllrms;
  float pmhrms;
  float pmmrms;
  float pmlrms;
  float prhrms;
  float prmrms;
  float prlrms;
  float plhsd;
  float plmsd;
  float pllsd;
  float pmhsd;
  float pmmsd;
  float pmlsd;
  float prhsd;
  float prmsd;
  float prlsd;
  float maximum;
  int index;
private:
  
  LP12 leftLPL;
  LP12 rightLPL;
  LP12 midLPL;
  
  LP12 leftLPM1;
  LP12 leftLPM2;
  LP12 rightLPM1;
  LP12 rightLPM2;
  LP12 midLPM;
  
  LP12 leftLP;
  LP12 rightLP;
  LP24 midLP;
  
  HP12 midHP;
  HP12 leftHP;
  HP12 rightHP;
  
  HP12 leftHPM;
  HP12 rightHPM;
  HP12 midHPM;
  
  HP12 leftHPH;
  HP12 rightHPH;
  HP12 midHPH;
  Gate gate1;
  Gate gate2;
  Gate gate3;
  //Gate gate4;
  //Gate gate5;
  //Gate gate6;
  //Gate gate7;
  //Gate gate8;
  //Gate gate9;
//  Phaser phaser1;
//  Phaser phaser2;
  Clipper clipper1;
  Clipper clipper2;
  //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThreeBandEQ)
};
class Squwbs : public IPlug
{
public:
  Squwbs(IPlugInstanceInfo instanceInfo);
  ~Squwbs();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
  double sr1;
  double fq1;
  double mGain;
  double prevGain;
  LP12 filter1;
  LP12 filter2;
  ThreeBandEQ filter3;
};

#endif

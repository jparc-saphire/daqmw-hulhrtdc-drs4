#include <iostream>
#include <fstream>
#include <algorithm>

#include <TLine.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TVirtualPad.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <THttpServer.h>
#include <TMemFile.h>
#include <TDirectory.h>
#include <TSystem.h>

#include "cpp11_make_unique.hh"
#include "datadrs.hh"

#include "TdcDrawer.hh"
#include "IntegralAna.hh"
#include "WaveformDrawer.hh"
#include "DrsDraw.hh"


TdcDrawer tdc;
IntegralAna iana;
WaveformDrawer wf;

//______________________________________________________________________________
DrsDraw::DrsDraw(const char* name)
  : fName(name)
{
}

//______________________________________________________________________________
void DrsDraw::Fill(const dataDrs& cont)
{
  tdc.fill(cont);
  iana.fill(cont);

  std::vector<double> peakXList(NofChModule);
  std::vector<double> baselineList(NofChModule);
  std::vector<double> amplitudeList(NofChModule);
  std::vector<double> integralList(NofChModule);

  for (int i=0; i<NofChModule; ++i) {
    peakXList[i]     = iana.getPeakX(i);
    baselineList[i]  = iana.getBaseLine(i);
    amplitudeList[i] = iana.getAmplitude(i);
    integralList[i]  = iana.getIntegral(i);
  }
  wf.fill(cont, peakXList, baselineList);
}

//______________________________________________________________________________
void DrsDraw::Initialize(THttpServer* p)
{
  tdc.initialize(p, fName);
  iana.initialize(p, fName);
  wf.initialize(p, fName);

  p->ls();
  gDirectory->ls();
  
}

//______________________________________________________________________________
void DrsDraw::Reset()
{
  tdc.reset();
  iana.reset();
  wf.reset();


}

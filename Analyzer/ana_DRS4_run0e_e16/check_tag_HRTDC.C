#define HIST
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TMath.h>
#include <TProfile.h>
#include <TString.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <fstream>
using namespace std;

Bool_t E88_trig = false;

int main(int argc,char *argv[]) {


  //  const Double_t tdc_min = 1900.;
  //    const Double_t tdc_max = 1980.;

  const Double_t tdc_min = 2000.;
  const Double_t tdc_max = 3000.;
  //  const Double_t tdc_min = 0.;
  //  const Double_t tdc_max = 40960.;

  cout << "argc = " << argc << endl;
  if (argc<=1) {
    cerr << "Usage check_tag_trig run" << endl;
    exit(-1);
  }

  Int_t run = atoi(argv[1]);

  TFile *file0 = new TFile(Form("./ana/check_drs4_run0e_e16/check_drs4_run0e_m_run%05d.root", run), "read");
  TTree *htree = (TTree*)file0 -> Get("htree");

  //  htree->Scan("spillID:event_tag:lt_trig1[2][0]","lt_trig1[2][0]>0");

  const Int_t nch_trig1 = 5;
  const Int_t max_nh = 20;
    
  Int_t ievt, spillID, spillID_tag,  event_tag;
  Double_t hlt_trig1[nch_trig1][max_nh];

  htree->SetBranchAddress("ievt",&ievt);
  htree->SetBranchAddress("spillID",&spillID);
  htree->SetBranchAddress("spillID_tag",&spillID_tag);
  htree->SetBranchAddress("event_tag",&event_tag);
  htree->SetBranchAddress("lt_trig1",hlt_trig1);//trig1(L1/L1RAW/MRPC/TSC/AC)

  ofstream ofs;
  ofstream ofs2;

  ofs.open(Form("./ana/trig_data/check_tag_HRTDC_run%d.dat",run));
  ofs2.open(Form("./ana/trig_data/check_tag_HRTDC_detail_run%d.dat",run));
  cout << "count" << " " << "event" << " " << "spill" << " " << "trigL1raw" << endl;
  ofs2 << "count" << " " << "event" << " " << "spill" << " " << "trigL1raw" << endl;
  Int_t nentries = (Int_t)htree->GetEntries();
  Int_t ic = 0;
  for (Int_t i=0; i<nentries; i++) {
    if ((i%10000) == 0) {
      cout << "event " << i << endl;
    }

    htree->GetEntry(i);
    Double_t trig_L1raw= hlt_trig1[1][0];
    Double_t trig_MRPC = hlt_trig1[2][0];
    Double_t trig_TSC = hlt_trig1[3][0];
    Double_t trig_AC = hlt_trig1[4][0];
    
    //    if ((trig_MRPC>=0)||(trig_TSC>=0)) {
    //    if (trig_L1raw>=0) {

      //      if (((tdc_min<=trig_MRPC)&&(trig_MRPC<=tdc_max))
      //	  ||((tdc_min<=trig_TSC)&&(trig_TSC<=tdc_max))) {
      //	ofs << event_tag << " " << spillID_tag << endl;
      //      }

    if ((!E88_trig)||(E88_trig &&(tdc_min<=trig_L1raw)&&(trig_L1raw<=tdc_max))) {
      ofs << event_tag << " " << spillID_tag << endl;
      
	cout << ic << " " << event_tag << " " << spillID_tag << " " << trig_L1raw << endl;
	ofs2 << ic << " " << event_tag << " " << spillID_tag << " " << trig_L1raw << endl;
      }

      ic++;
      //  }
  }
  ofs.close();
  ofs2.close();
  
}


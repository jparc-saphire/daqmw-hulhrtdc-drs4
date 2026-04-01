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

  cout << "argc = " << argc << endl;
  if (argc<=1) {
    cerr << "Usage check_tag_trig run" << endl;
    exit(-1);
  }

  Int_t run = atoi(argv[1]);

  TFile *file0 = new TFile(Form("/disk1/mrpcdaq/trig_data/run%06d_trg.root", run), "read");
  TTree *tree0 = (TTree*)file0 -> Get("tree0");

  const Int_t nch_trig1 = 5;
  const Int_t max_nh = 20;
    
  Int_t ievt;
  UShort_t tagSpill;
  UInt_t tagEvent;
  UChar_t mainTrgType;

  tree0->SetBranchAddress("ievt",&ievt);
  tree0->SetBranchAddress("tagSpill",&tagSpill);
  tree0->SetBranchAddress("tagEvent",&tagEvent);
  tree0->SetBranchAddress("mainTrgType",&mainTrgType);

  ofstream ofs;
  ofstream ofs2;

  ofs.open(Form("./ana/trig_data/check_tag_trig_run%d.dat",run));
  ofs2.open(Form("./ana/trig_data/check_tag_trig_detail_run%d.dat",run));

  cout << "count" << " " << "event" << " " << "spill" << " " << "mainTrgType" << endl;
  ofs2 << "count" << " " << "event" << " " << "spill" << " " << "mainTrgType" << endl;
  
  Int_t nentries = (Int_t)tree0->GetEntries();
  Int_t ic = 0;
  for (Int_t i=0; i<nentries; i++) {
    if ((i%10000) == 0) {
      cout << "event " << i << endl;
    }

    tree0->GetEntry(i);
    if ((!E88_trig)||(E88_trig && (mainTrgType==5))) {
      //      cout << tagEvent << " " << tagSpill << endl;
      ofs << tagEvent << " " << tagSpill << endl;

      cout << ic << " " <<  tagEvent << " " << tagSpill << " " << (Int_t)(mainTrgType) << endl;
      ofs2 << ic << " " <<  tagEvent << " " << tagSpill << " " << (Int_t)(mainTrgType) << endl;
      
      ic++;
    }
  }
  ofs.close();
  ofs2.close();
  
}


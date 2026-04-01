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

const Bool_t debug = true;

int main(int argc,char *argv[]) {

  cout << "argc = " << argc << endl;
  if (argc<=1) {
    cerr << "Usage merge_mrpc_trig run" << endl;
    exit(-1);
  }

  Int_t run = atoi(argv[1]);

  TFile *file0 = new TFile(Form("/disk1/mrpcdaq/trig_data/run%06d_trg.root", run), "read");
  TTree *tree0 = (TTree*)file0 -> Get("tree0");

  TFile *file1 = new TFile(Form("./ana/check_drs4_run0e_e16/check_drs4_run0e_run%05d.root", run), "read");
  TTree *htree = (TTree*)file1 -> Get("htree");

  TFile *fout = new TFile(Form("ana/check_drs4_run0e_e16/check_drs4_run0e_m_run%05d.root", run),"RECREATE");

  //  const Int_t nch_trig1 = 6;
  Int_t hev=0;
  const Int_t max_nh = 20;
  Int_t hrun=0;
    
  Int_t ievt;
  UShort_t tagSpill;
  UInt_t tagEvent;
  UChar_t mainTrgType;

  Int_t itagSpill;
  Int_t itagEvent;
  Int_t imainTrgType;

  tree0->SetBranchAddress("ievt",&ievt);
  tree0->SetBranchAddress("tagSpill",&tagSpill);
  tree0->SetBranchAddress("tagEvent",&tagEvent);
  tree0->SetBranchAddress("mainTrgType",&mainTrgType);
  
  const Int_t nstrip_rpc = 8;
  
  //TSC0,TSC1,TSC2
  const Int_t nstrip_tsc1 = 25;
  const Int_t nstrip_tsc2 = 8;
  const Int_t nstrip_tsc3 = 12;
  
  //AC
  const Int_t n_ac = 8;
  //RPCR
  const Int_t n_ref = 5;
  //    const Int_t nstrip_rpcr = 4;
  
  const Int_t nt_b = 2;//top/bottom
  const Int_t nch_trig1 = 5;
  
  //  TTree *htree = new TTree("htree","all hits");
  //Drs4
  Double_t hamp_ac[n_ac];
  Double_t hadc_ac[n_ac];
  Double_t hbl_ac[n_ac];
  
  Int_t hnh_rpc0[nstrip_rpc][nt_b];
  Int_t hnh_rpc1[nstrip_rpc][nt_b];
  Int_t hnh_rpc2[nstrip_rpc][nt_b];
  Int_t hnh_tsc1[nstrip_tsc1][nt_b];
  Int_t hnh_tsc2[nstrip_tsc2][nt_b];
  Int_t hnh_tsc3[nstrip_tsc3][nt_b];
  Int_t hnh_ac[n_ac];
  Int_t hnh_ref[n_ref][nt_b];
  Int_t hnh_trig1[nch_trig1];
  
  Double_t hlt_rpc0[nstrip_rpc][nt_b][max_nh];
  Double_t htt_rpc0[nstrip_rpc][nt_b][max_nh];
  Double_t hlt_rpc1[nstrip_rpc][nt_b][max_nh];
  Double_t htt_rpc1[nstrip_rpc][nt_b][max_nh];
  Double_t hlt_rpc2[nstrip_rpc][nt_b][max_nh];
  Double_t htt_rpc2[nstrip_rpc][nt_b][max_nh];
  Double_t hlt_tsc1[nstrip_tsc1][nt_b][max_nh];
  Double_t htt_tsc1[nstrip_tsc1][nt_b][max_nh];
  Double_t hlt_tsc2[nstrip_tsc2][nt_b][max_nh];
  Double_t htt_tsc2[nstrip_tsc2][nt_b][max_nh];
  Double_t hlt_tsc3[nstrip_tsc3][nt_b][max_nh];
  Double_t htt_tsc3[nstrip_tsc3][nt_b][max_nh];
  Double_t hlt_ac[n_ac][max_nh];
  Double_t htt_ac[n_ac][max_nh];
  Double_t hlt_ref[n_ref][nt_b][max_nh];
  Double_t htt_ref[n_ref][nt_b][max_nh];
  
  Double_t hlt_trig1[nch_trig1][max_nh];
  Double_t htt_trig1[nch_trig1][max_nh];
  
  Int_t spillID;
  Int_t event_number;
  Int_t MZN_event_number[3];
  Int_t nh_all;
  Int_t event_tag;
  Int_t spillID_tag;

  
  htree->SetBranchAddress("spillID",&spillID);
  htree->SetBranchAddress("spillID_tag",&spillID_tag);
  htree->SetBranchAddress("event_number",&event_number);
  htree->SetBranchAddress("MZN_event_number",MZN_event_number);
  htree->SetBranchAddress("event_tag",&event_tag);
  //  htree->SetBranchAddress("nh_all",&nh_all);
  htree->SetBranchAddress("nh_allr",&nh_all);
  
  htree->SetBranchAddress("irun",&hrun);//2021/7/1
  
  htree->SetBranchAddress("ievt",&hev);
  htree->SetBranchAddress("nh_rpc0",hnh_rpc0);//[strip][up_down]
  htree->SetBranchAddress("nh_rpc1",hnh_rpc1);//[strip][up_down]
  htree->SetBranchAddress("nh_rpc2",hnh_rpc2);//[strip][up_down]
  htree->SetBranchAddress("nh_tsc1",hnh_tsc1);//[strip][up_down]
  htree->SetBranchAddress("nh_tsc2",hnh_tsc2);//[strip][up_down]
  htree->SetBranchAddress("nh_tsc3",hnh_tsc3);//[strip][up_down]
  htree->SetBranchAddress("nh_ac",hnh_ac);//[det]
  htree->SetBranchAddress("nh_ref",hnh_ref);//[det][strip]
  htree->SetBranchAddress("nh_trig1",hnh_trig1);//[ch]
  
  htree->SetBranchAddress("lt_rpc0",hlt_rpc0);//[strip][up_down][max_hit]
  htree->SetBranchAddress("lt_rpc1",hlt_rpc1);
  htree->SetBranchAddress("lt_rpc2",hlt_rpc2);
  htree->SetBranchAddress("lt_tsc1",hlt_tsc1);
  htree->SetBranchAddress("lt_tsc2",hlt_tsc2);
  htree->SetBranchAddress("lt_tsc3",hlt_tsc3);
  htree->SetBranchAddress("lt_ac",hlt_ac);
  htree->SetBranchAddress("lt_ref",hlt_ref);
  htree->SetBranchAddress("lt_trig1",hlt_trig1);//trig1(MRPC/TSC/AC/RPCREF/L1/L1SD)
  
  htree->SetBranchAddress("tt_rpc0",htt_rpc0);
  htree->SetBranchAddress("tt_rpc1",htt_rpc1);
  htree->SetBranchAddress("tt_rpc2",htt_rpc2);
  htree->SetBranchAddress("tt_tsc1",htt_tsc1);
  htree->SetBranchAddress("tt_tsc2",htt_tsc2);
  htree->SetBranchAddress("tt_tsc3",htt_tsc3);
  htree->SetBranchAddress("tt_ac",htt_ac);
  htree->SetBranchAddress("tt_ref",htt_ref);
  htree->SetBranchAddress("tt_trig1",htt_trig1);//trig1(MRPC/TSC/AC/RPCREF/L1/L1SD)
  
  htree->SetBranchAddress("amp_ac",&hamp_ac);
  htree->SetBranchAddress("adc_ac",&hadc_ac);
  htree->SetBranchAddress("adc_ac",&hbl_ac);
    

  TTree *mtree = new TTree("htree","all hits");
  mtree->Branch("spillID",&spillID,"spillID/I");
  mtree->Branch("spillID_tag",&spillID_tag,"spillID_tag/I");
  mtree->Branch("event_number",&event_number,"event_number/I");
  mtree->Branch("MZN_event_number",MZN_event_number,"MZN_event_number[3]/I");
  mtree->Branch("event_tag",&event_tag,"event_tag/I");
  mtree->Branch("nh_allr",&nh_all,"nh_all/I");
  
  
  mtree->Branch("ievt",&hev, "ev/I");
  mtree->Branch("nh_rpc0",hnh_rpc0,"nh_rpc0[8][2]/I");//[strip][up_down]
  mtree->Branch("nh_rpc1",hnh_rpc1,"nh_rpc1[8][2]/I");//[strip][up_down]
  mtree->Branch("nh_rpc2",hnh_rpc2,"nh_rpc2[8][2]/I");//[strip][up_down]
  mtree->Branch("nh_tsc1",hnh_tsc1,"nh_tsc1[25][2]/I");//[strip][up_down]
  mtree->Branch("nh_tsc2",hnh_tsc2,"nh_tsc2[8][2]/I");//[strip][up_down]
  mtree->Branch("nh_tsc3",hnh_tsc3,"nh_tsc3[12][2]/I");//[strip][up_down]
  mtree->Branch("nh_ac",hnh_ac,"nh_ac[8]/I");//[det]
  mtree->Branch("nh_ref",hnh_ref,"nh_ref[5][2]/I");//[det][strip]
  mtree->Branch("nh_trig1",hnh_trig1,"nh_trig1[5]/I");//[ch]
  
  mtree->Branch("lt_rpc0",hlt_rpc0,"lt_rpc0[8][2][20]/D");//[strip][up_down][max_hit]
  mtree->Branch("lt_rpc1",hlt_rpc1,"lt_rpc1[8][2][20]/D");
  mtree->Branch("lt_rpc2",hlt_rpc2,"lt_rpc2[8][2][20]/D");
  mtree->Branch("lt_tsc1",hlt_tsc1,"lt_tsc1[25][2][20]/D");
  mtree->Branch("lt_tsc2",hlt_tsc2,"lt_tsc2[8][2][20]/D");
  mtree->Branch("lt_tsc3",hlt_tsc3,"lt_tsc3[12][2][20]/D");
  mtree->Branch("lt_ac",hlt_ac,"lt_ac[8][20]/D");
  mtree->Branch("lt_ref",hlt_ref,"lt_ref[5][2][20]/D");
  mtree->Branch("lt_trig1",hlt_trig1,"lt_trig1[5][20]/D");//trig1(MRPC/TSC/AC/RPCREF/L1/L1SD)
  
  mtree->Branch("tt_rpc0",htt_rpc0,"tt_rpc0[8][2][20]/D");
  mtree->Branch("tt_rpc1",htt_rpc1,"tt_rpc1[8][2][20]/D");
  mtree->Branch("tt_rpc2",htt_rpc2,"tt_rpc2[8][2][20]/D");
  mtree->Branch("tt_tsc1",htt_tsc1,"tt_tsc1[25][2][20]/D");
  mtree->Branch("tt_tsc2",htt_tsc2,"tt_tsc2[8][2][20]/D");
  mtree->Branch("tt_tsc3",htt_tsc3,"tt_tsc3[12][2][20]/D");
  mtree->Branch("tt_ac",htt_ac,"tt_ac[8][20]/D");
  mtree->Branch("tt_ref",htt_ref,"tt_ref[5][2][20]/D");
  mtree->Branch("tt_trig1",htt_trig1,"tt_trig1[5][20]/D");//trig1(MRPC/TSC/AC/RPCREF/L1/L1SD)
      
  mtree->Branch("tagSpill",&itagSpill);
  mtree->Branch("tagEvent",&itagEvent);
  mtree->Branch("mainTrgType",&imainTrgType);
    
  mtree->Branch("amp_ac",&hamp_ac,"amp[8]/D");
  mtree->Branch("adc_ac",&hadc_ac,"adc[8]/D");
  mtree->Branch("adc_ac",&hbl_ac,"bl[8]/D");


  Int_t nentries_trig = (Int_t)tree0->GetEntries();
  cout << "nentries_trig = " << nentries_trig << endl;
  Int_t nentries = (Int_t)htree->GetEntries();
  Int_t it = 0;
  for (Int_t i=0; i<nentries; i++) {
    if ((i%10000) == 0) {
      cout << "event " << i << endl;
    }

    htree->GetEntry(i);
    tree0->GetEntry(it);

    if (debug) {
      cout << "tagEvent(E16), event_tag(HUL) = " << tagEvent << " " << event_tag << endl;
    }
    
    if (tagEvent==event_tag) {
      //merge
      itagSpill = (int)tagSpill;
      itagEvent = (int)tagEvent;
      imainTrgType = (int)mainTrgType;
      it++;
    } else if (tagEvent<event_tag) {
      cerr << "tagEvent " << tagEvent << " < event_tag " << event_tag << endl;
      //search for same trigger event
      if (debug) {
	cout << "++look for sync event" << endl;
      }
      do {
	it++;
	tree0->GetEntry(it);
	cout << "loop, it, tagEvent, event_tag = " << it << " " << tagEvent << " " << event_tag << endl;
      }
      while ((tagEvent<event_tag)&&(it<(nentries_trig-1)));

      //should be same event
      if (tagEvent==event_tag) {
	cerr << "Fixed : tagEvent " << tagEvent << " == event_tag " << event_tag << endl;
	//merge
	itagSpill = (int)tagSpill;
	itagEvent = (int)tagEvent;
	imainTrgType = (int)mainTrgType;
	it++;
      } else { //tagEvent>event_tag... no same trig event found
	//Fill dummy data
	cerr << "out of loop: tagEvent " << tagEvent << " > event_tag " << event_tag << " no trig event found" << endl;
	cerr << "it, nentries_trig = " << it << " " << nentries_trig << endl;
	ievt = -1;
	itagSpill = -1;
	itagEvent = -1;
	imainTrgType = -1;
      }
      
    } else {//tagEvent>event_tag
      //no increment of it. Fill dummy data
      cerr << "tagEvent " << tagEvent << " > event_tag " << event_tag << " no trig event found" << endl;
      ievt = -1;
      itagSpill = -1;
      itagEvent = -1;
      imainTrgType = -1;
    }
    
    //    if (mainTrgType==5) {
      //      ofs << tagEvent << " " << tagSpill << endl;

      //      cout << ic << " " <<  tagEvent << " " << tagSpill << " " << (Int_t)(mainTrgType) << endl;
      //      ofs2 << ic << " " <<  tagEvent << " " << tagSpill << " " << (Int_t)(mainTrgType) << endl;
      
      //      ic++;
    //    }
    mtree->Fill();
  }
  //  ofs.close();
  //  ofs2.close();


  mtree->Write();
  fout->Close();
  
}


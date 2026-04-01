#define HIST
#define NO_E16DAQ
#define  check_drs4_run0e_cc
#include "check_drs4_run0e.h"
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

static const Bool_t debug=false;
static const Bool_t hist =false;

static Int_t maxevent=0;

int main(int argc,char *argv[]) {

  cout << "argc = " << argc << endl;
  if (argc<=1) {
    cerr << "Usage check_drs4_run0e run [subrun] [maxevent]" << endl;
    exit(-1);
  }

  Int_t run = atoi(argv[1]);

  Int_t subrun=0;
  if (argc>=3) {
    subrun = atoi(argv[2]);
  }

  if (argc>=4) {
    maxevent = atoi(argv[3]);
  }

    TString basedir = "./data";

  check_drs4_run0e(run, subrun, basedir);
}
double check_drs4_run0e::call(vector<vector<double> > *vec, Int_t arg1, Int_t arg2){

    double val;
    try{
        val = vec -> at(arg1).at(arg2);
    }
    catch(...){
        val = -999;
    }

    return val;
}

void check_drs4_run0e::check(Int_t run, Int_t subrun){


  char runtag[256];

  if (subrun==0) {
    sprintf(runtag,"%d", run);
  } else {
    sprintf(runtag,"%d_%d",run,subrun);
  }
  const Int_t nplane=12;
  const Bool_t sort_TDC = true;
  const Bool_t pairing_TDC = true;
  const Bool_t detail = false;
  gStyle->SetOptFit(1111);
  gStyle->SetPadTopMargin(0.12);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.12);
  
  Double_t t_min_AC = 198;
  Double_t t_max_AC = 238;

  Double_t t_min_AC1 = 214;
  Double_t t_max_AC1 = 254;

  Double_t t_min_AC2 = 215;
  Double_t t_max_AC2 = 255;

  Double_t t_min_AC3 = 215;
  Double_t t_max_AC3 = 255;

  Double_t t_min_AC4 = 215;
  Double_t t_max_AC4 = 255;


  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);

  Int_t nt = 5000;
  Int_t maxt = 500;
  Int_t maxam = 600;
  Int_t maxad = 300;

    //AC
  const Int_t n_ac = 8;//AC4-1,AC4-2,AC4-3,AC4-4,AC4-5,AC1,AC2,AC3
  TH1D *hlt_AC[n_ac];
  TH1D *hlt_AC_g[n_ac];
  TH1D *hnh_AC[n_ac];
  TH1D *hwt_AC[n_ac];
  TH2D *hlt_wt_AC[n_ac];
  TH2D *hlt_wt_AC_g[n_ac];
  TH1D *hamp_AC[n_ac];
  TH1D *hadc_AC[n_ac];
  TH1D *hbl_AC[n_ac];
  TH2D *hamp_wt_AC[n_ac];
  TH2D *hadc_wt_AC[n_ac];
  for (Int_t iac=0;iac<n_ac;iac++) {
    hlt_AC[iac] = new TH1D(Form("hlt_AC%d",iac),Form("hlt_AC%d",iac),nt,0,maxt);
    hlt_AC_g[iac] = new TH1D(Form("hlt_AC_g%d",iac),Form("hlt_AC_g%d",iac),nt,0,maxt);
    hnh_AC[iac] = new TH1D(Form("hnh_AC%d",iac),Form("hnh_AC%d",iac),100,-0.5,99.5);
    hwt_AC[iac] = new TH1D(Form("hwt_AC%d",iac),Form("hwt_AC%d",iac),400,0,100);
    hlt_wt_AC[iac] = new TH2D(Form("hlt_wt_AC%d",iac),Form("hlt_wt_AC%d",iac),400,0,100,nt,0,maxt);
    hlt_wt_AC_g[iac] = new TH2D(Form("hlt_wt_AC_g%d",iac),Form("hlt_wt_AC_g%d",iac),400,0,100,nt,0,maxt);
    hamp_AC[iac] = new TH1D(Form("hamp_AC%d",iac),Form("hamp_AC%d",iac),nt,0,maxam);
    hadc_AC[iac] = new TH1D(Form("hadc_AC%d",iac),Form("hadc_AC%d",iac),nt,0,maxad);
    hbl_AC[iac] = new TH1D(Form("hbl_AC%d",iac),Form("hbl_AC%d",iac),nt,0,maxam);
    hamp_wt_AC[iac] = new TH2D(Form("hamp_wt_AC%d",iac),Form("hamp_wt_AC%d",iac),400,0,100,nt,0,maxam);
    hadc_wt_AC[iac] = new TH2D(Form("hadc_wt_AC%d",iac),Form("hadc_wt_AC%d",iac),400,0,100,nt,0,maxad);
  }
  TH1D *hlt_AC12 = new TH1D("hlt_AC12","hlt_AC12",nt,0,maxt);

  //amp = peak, adc = integral
  
  TH1D *hamp_AC12 = new TH1D("hamp_AC12","hamp_AC12",nt,0,maxam);
  TH1D *hadc_AC12 = new TH1D("hadc_AC12","hadc_AC12",nt,0,maxad);
  TH1D *hbl_AC12 = new TH1D("hbl_AC12","hbl_AC12",nt,0,maxam);
  TH2D *hamp_wt_AC12 = new TH2D("hamp_wt_AC12","hamp_wt_AC12",1200,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC12 = new TH2D("hadc_wt_AC12","hadc_wt_AC12",1200,0,100,nt,0,maxad);
  TH2D *hlt_wt_AC12 = new TH2D("hlt_wt_AC12","hlt_wt_AC12",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC12_g = new TH2D("hlt_wt_AC12_g","hlt_wt_AC12_g",400,0,100,nt,0,maxt);

#ifdef HIST   
  TH1D *h_le1[ntdc][nch];
  TH1D *h_le2[ntdc][nch];
  TH1D *h_le3[ntdc][nch];
  TH1D *h_tr1[ntdc][nch];
  TH1D *h_tr2[ntdc][nch];
  TH1D *h_tr3[ntdc][nch];
  TH1D *h_wi1[ntdc][nch];
  TH1D *h_wi2[ntdc][nch];
  TH1D *h_wi3[ntdc][nch];
  
  TH2D *h_lw1[ntdc][nch];
  TH2D *h_lw2[ntdc][nch];
  TH2D *h_lw3[ntdc][nch];
  
  TH2D *h_lt1[ntdc][nch];
  TH2D *h_lt2[ntdc][nch];
  TH2D *h_lt3[ntdc][nch];
  TH2D *h_nh[ntdc][nch];
  TH1D *h_multil[nplane];//multiplicity for each plane (4)
  TH1D *h_multit[nplane];//multiplicity for each plane (4)
  TH1D *h_lch[nplane];//channel dist
  TH1D *h_tch[nplane];//channel dist

  for (Int_t itdc = 0; itdc < ntdc; itdc++){
    Int_t nch_tdc = 64;
    for(Int_t ch = 0; ch < nch_tdc; ch++){
      Int_t iplane = ch/16+itdc*4;
      if (ch%16==0) {
	h_multil[iplane] = new TH1D(Form("h_multil_%d", iplane), "no. of leading hits per plane (16ch)", 17,-0.5,16.5);
	h_multit[iplane] = new TH1D(Form("h_multit_%d", iplane), "no. of trailing hits per plane (16ch)", 17,-0.5,16.5);
	h_lch[iplane] = new TH1D(Form("h_lch_%d", iplane), "ch dist (leading)", 16,-0.5,15.5);
	h_tch[iplane] = new TH1D(Form("h_tch_%d", iplane), "ch dist (trailing)", 16,-0.5,15.5);
      }


      h_le1[itdc][ch] = new TH1D(Form("h_le1_%d_%d", itdc, ch), "leading;(ns)",  500, -0.5, 2999.5);
      h_le2[itdc][ch] = new TH1D(Form("h_le2_%d_%d", itdc, ch), "leading;(ns)",  500, -0.5, 2999.5);
      h_le3[itdc][ch] = new TH1D(Form("h_le3_%d_%d", itdc, ch), "leading;(ns)",  500, -0.5, 2999.5);

        h_tr1[itdc][ch] = new TH1D(Form("h_tr1_%d_%d", itdc, ch), "trailing;(ns)", 500, -0.5, 499.5);
        h_tr2[itdc][ch] = new TH1D(Form("h_tr2_%d_%d", itdc, ch), "trailing;(ns)", 500, -0.5, 499.5);
        h_tr3[itdc][ch] = new TH1D(Form("h_tr3_%d_%d", itdc, ch), "trailing;(ns)", 500, -0.5, 499.5);

        h_wi1[itdc][ch] = new TH1D(Form("h_wi1_%d_%d", itdc, ch), "width;(ns)", 3200, -400., 400.);
        h_wi2[itdc][ch] = new TH1D(Form("h_wi2_%d_%d", itdc, ch), "width;(ns)", 3200, -400., 400.);
        h_wi3[itdc][ch] = new TH1D(Form("h_wi3_%d_%d", itdc, ch), "width;(ns)", 3200, -400., 400.);

	h_lw1[itdc][ch] = new TH2D(Form("h_lw1_%d_%d", itdc, ch), "leading vs width;(ns)", 800, -100., 100.,600,0.,150.);
        h_lw2[itdc][ch] = new TH2D(Form("h_lw2_%d_%d", itdc, ch), "leading vs width;(ns)", 800, -100., 100.,600,0.,150.);
        h_lw3[itdc][ch] = new TH2D(Form("h_lw3_%d_%d", itdc, ch), "leading vs width;(ns)", 800, -100., 100.,600,0.,150.);

        h_lt1[itdc][ch] = new TH2D(Form("h_lt1_%d_%d", itdc, ch), "leading vs trailing(ns)", 600, 0., 150.,600,0.,150.);
        h_lt2[itdc][ch] = new TH2D(Form("h_lt2_%d_%d", itdc, ch), "leading vs trailing(ns)", 600, 0., 150.,600,0.,150.);
        h_lt3[itdc][ch] = new TH2D(Form("h_lt3_%d_%d", itdc, ch), "leading vs trailing(ns)", 600, 0., 150.,600,0.,150.);


        h_nh[itdc][ch] = new TH2D(Form("h_nh_%d_%d", itdc, ch), "leading vs trailing no. of hits", 20, -0.5,19.5,20,-0.5,19.5);
      
    }

  }
#endif


  TFile *fout = new TFile(Form("ana/check_drs4_run0e/check_drs4_run0e_run%s.root", runtag),"RECREATE");
    
  Int_t hev=0;
  Int_t hrun=run;

  const Int_t max_nh = 20;
  const Int_t nstrip_rpc = 8;
  
  //TSC1,TSC2,TSC3
  //run0d    const Int_t nstrip_tsc1 = 24;
  //run0e
  const Int_t nstrip_tsc1 = 25;
  const Int_t nstrip_tsc2 = 8;
  const Int_t nstrip_tsc3 = 12;
  
  //ref counters
  const Int_t n_ref = 5;//ref1, ref2-1, ref1-2, ref3-1, ref3-2
  const Int_t nt_b = 2;//top/bottom
  const Int_t nch_trig1 = 5;
  
  TTree *htree = new TTree("htree","all hits");

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
  //    Double_t hlt_rpc3[nstrip_rpc][nt_b][max_nh];
  //    Double_t htt_rpc3[nstrip_rpc][nt_b][max_nh];
  Double_t hlt_tsc1[nstrip_tsc1][nt_b][max_nh];
  Double_t htt_tsc1[nstrip_tsc1][nt_b][max_nh];
  Double_t hlt_tsc2[nstrip_tsc2][nt_b][max_nh];
  Double_t htt_tsc2[nstrip_tsc2][nt_b][max_nh];
  Double_t hlt_tsc3[nstrip_tsc3][nt_b][max_nh];
  Double_t htt_tsc3[nstrip_tsc3][nt_b][max_nh];
  Double_t hlt_ac[n_ac][max_nh];
  Double_t htt_ac[n_ac][max_nh];
  //    Double_t hlt_rpcr[n_rpcr][nstrip_rpcr][max_nh];
  //    Double_t htt_rpcr[n_rpcr][nstrip_rpcr][max_nh];
  Double_t hlt_ref[n_ref][nt_b][max_nh];
  Double_t htt_ref[n_ref][nt_b][max_nh];
  
  Double_t hlt_trig1[nch_trig1][max_nh];
  Double_t htt_trig1[nch_trig1][max_nh];
  
  //2024/4/19
#ifdef E16DAQ
  Int_t event_tag = 0;
  Int_t spillID_tag = 1;
  htree->Branch("spillID",&spillID,"spillID/I");
  htree->Branch("spillID_tag",&spillID_tag,"spillID_tag/I");
  htree->Branch("event_number",&event_number,"event_number/I");
  htree->Branch("MZN_event_number",MZN_event_number,"MZN_event_number[3]/I");
  htree->Branch("event_tag",&event_tag,"event_tag/I");
  htree->Branch("nh_allr",&nh_all,"nh_all/I");
#endif
  
  htree->Branch("ievt",&hev, "ev/I");
  htree->Branch("nh_rpc0",hnh_rpc0,"nh_rpc0[8][2]/I");//[strip][up_down]
  htree->Branch("nh_rpc1",hnh_rpc1,"nh_rpc1[8][2]/I");//[strip][up_down]
  htree->Branch("nh_rpc2",hnh_rpc2,"nh_rpc2[8][2]/I");//[strip][up_down]
  htree->Branch("nh_tsc1",hnh_tsc1,"nh_tsc1[25][2]/I");//[strip][up_down]
  htree->Branch("nh_tsc2",hnh_tsc2,"nh_tsc2[8][2]/I");//[strip][up_down]
  htree->Branch("nh_tsc3",hnh_tsc3,"nh_tsc3[12][2]/I");//[strip][up_down]
  htree->Branch("nh_ac",hnh_ac,"nh_ac[8]/I");//[det]
  //    htree->Branch("nh_rpcr",hnh_rpcr,"nh_rpcr[2][4]/I");//[det][strip]
  htree->Branch("nh_ref",hnh_ref,"nh_ref[5][2]/I");//[det][strip]
  htree->Branch("nh_trig1",hnh_trig1,"nh_trig1[5]/I");//[ch]
  
  htree->Branch("lt_rpc0",hlt_rpc0,"lt_rpc0[8][2][20]/D");//[strip][up_down][max_hit]
  htree->Branch("lt_rpc1",hlt_rpc1,"lt_rpc1[8][2][20]/D");
  htree->Branch("lt_rpc2",hlt_rpc2,"lt_rpc2[8][2][20]/D");
  //    htree->Branch("lt_rpc3",hlt_rpc3,"lt_rpc3[8][2][20]/D");
  htree->Branch("lt_tsc1",hlt_tsc1,"lt_tsc1[25][2][20]/D");
  htree->Branch("lt_tsc2",hlt_tsc2,"lt_tsc2[8][2][20]/D");
  htree->Branch("lt_tsc3",hlt_tsc3,"lt_tsc3[12][2][20]/D");
  htree->Branch("lt_ac",hlt_ac,"lt_ac[8][20]/D");
  //    htree->Branch("lt_rpcr",hlt_rpcr,"lt_rpcr[2][4][20]/D");
  htree->Branch("lt_ref",hlt_ref,"lt_ref[5][2][20]/D");
  //    htree->Branch("lt_trig1",hlt_trig1,"lt_trig1[7][20]/D");//trig1(MRPC/TSC/AC/RPCREF/L1/L1SD)
  htree->Branch("lt_trig1",hlt_trig1,"lt_trig1[5][20]/D");//trig1(MRPC/TSC/AC/RPCREF/L1/L1SD)
  
  htree->Branch("tt_rpc0",htt_rpc0,"tt_rpc0[8][2][20]/D");
  htree->Branch("tt_rpc1",htt_rpc1,"tt_rpc1[8][2][20]/D");
  htree->Branch("tt_rpc2",htt_rpc2,"tt_rpc2[8][2][20]/D");
  htree->Branch("tt_tsc1",htt_tsc1,"tt_tsc1[25][2][20]/D");
  htree->Branch("tt_tsc2",htt_tsc2,"tt_tsc2[8][2][20]/D");
  htree->Branch("tt_tsc3",htt_tsc3,"tt_tsc3[12][2][20]/D");
  htree->Branch("tt_ac",htt_ac,"tt_ac[8][20]/D");
  //    htree->Branch("tt_rpcr",htt_rpcr,"tt_rpcr[2][4][20]/D");
  htree->Branch("tt_ref",htt_ref,"tt_ref[5][2][20]/D");
  //    htree->Branch("tt_trig1",htt_trig1,"tt_trig1[7][20]/D");//trig1(MRPC/TSC/AC/RPCREF/L1/L1SD)
  htree->Branch("tt_trig1",htt_trig1,"tt_trig1[5][20]/D");//trig1(MRPC/TSC/AC/RPCREF/L1/L1SD)
  

  //DRS4
  
  htree->Branch("amp_ac",&hamp_ac,"amp[8]/D");
  htree->Branch("adc_ac",&hadc_ac,"adc[8]/D");
  htree->Branch("adc_ac",&hbl_ac,"bl[8]/D");

  
  
  Int_t nentries = tree0 -> GetEntriesFast();

  cout << "Event : " << nentries << endl;
  if (maxevent>0) {
    nentries = maxevent;
  }


  for(Int_t event = 0; event < nentries; event++){
    hev = event;
    for (Int_t iac=0;iac<n_ac;iac++) {
      hadc_ac[iac] = -1;
    }
    //iac=0-4: AC4, iac=5(AC1), 6(AC2), 7(AC3)
    if ((event%10000) == 0) {
      cout << "event : " << event << endl;
    }
    Int_t ientry = tree0 -> LoadTree(event);
    tree0 -> GetEntry(ientry);

      //def
      const Int_t max_pair = 16;
      //temp sako
      //      const Int_t max_pair = 64;
      Double_t gltdcs0[nch][max_pair];
      Double_t gltdcs1[nch][max_pair];
      Double_t gltdcs2[nch][max_pair];
      Double_t gttdcs0[nch][max_pair];
      Double_t gttdcs1[nch][max_pair];
      Double_t gttdcs2[nch][max_pair];
      Int_t mg0[nch];//keep no. of hits
      Int_t mg1[nch];//keep no. of hits
      Int_t mg2[nch];//keep no. of hits
      
      for (Int_t ich=0;ich<nch;ich++) {
	mg0[ich] = 0;
	for (Int_t ip=0;ip<max_pair;ip++) {
	  gltdcs0[ich][ip]=0;
	  gttdcs0[ich][ip]=0;
	}
      }  
      for (Int_t ich=0;ich<nch;ich++) {
	mg1[ich] = 0;
	for (Int_t ip=0;ip<max_pair;ip++) {
	  gltdcs1[ich][ip]=0;
	  gttdcs1[ich][ip]=0;
	}
      }
      for (Int_t ich=0;ich<nch;ich++) {
	mg2[ich] = 0;
	for (Int_t ip=0;ip<max_pair;ip++) {
	  gltdcs2[ich][ip]=0;
	  gttdcs2[ich][ip]=0;
	}
      }
  
		
      //check leading(L) > trailing(T) > leading(L) > trailing(T)... order
      //if LL or TT reject earlier (larger) one
      //order leading and trailing in decending order
      //Secure LT pairs and reject LLTTLLLT -> select  xLTxxxLT
      //Select only LT pairs
      
      for (Int_t ich = 0; ich < nstrip_rpc; ich++){
	for (Int_t itb = 0; itb < nt_b; itb++){
	  hnh_rpc0[ich][itb] = 0;
	  hnh_rpc1[ich][itb] = 0;
	  hnh_rpc2[ich][itb] = 0;
	  for (Int_t ih=0;ih<max_nh;ih++) {
	    hlt_rpc0[ich][itb][ih]=-1;
	    hlt_rpc1[ich][itb][ih]=-1;
	    hlt_rpc2[ich][itb][ih]=-1;
	    htt_rpc0[ich][itb][ih]=-1;
	    htt_rpc1[ich][itb][ih]=-1;
	    htt_rpc2[ich][itb][ih]=-1;
	  }
	}
      }
      for (Int_t ich = 0; ich < nstrip_tsc1; ich++){
	for (Int_t itb = 0; itb < nt_b; itb++){
	  hnh_tsc1[ich][itb] = 0;
	  for (Int_t ih=0;ih<max_nh;ih++) {
	    hlt_tsc1[ich][itb][ih]=-1;
	    htt_tsc1[ich][itb][ih]=-1;
	  }
	}
      }
      for (Int_t ich = 0; ich < nstrip_tsc2; ich++){
	for (Int_t itb = 0; itb < nt_b; itb++){
	  hnh_tsc2[ich][itb] = 0;
	  for (Int_t ih=0;ih<max_nh;ih++) {
	    hlt_tsc2[ich][itb][ih]=-1;
	    htt_tsc2[ich][itb][ih]=-1;
	  }
	}
      }
      for (Int_t ich = 0; ich < nstrip_tsc3; ich++){
	for (Int_t itb = 0; itb < nt_b; itb++){
	  hnh_tsc3[ich][itb] = 0;
	  for (Int_t ih=0;ih<max_nh;ih++) {
	    hlt_tsc3[ich][itb][ih]=-1;
	    htt_tsc3[ich][itb][ih]=-1;
	  }
	}
      }
      for (Int_t ich = 0; ich < n_ac; ich++){
	hnh_ac[ich] = 0;
	for (Int_t ih=0;ih<max_nh;ih++) {
	  hlt_ac[ich][ih]=-1;
	  htt_ac[ich][ih]=-1;
	}
      }
      for (Int_t ich = 0; ich < n_ref; ich++){
	for (Int_t itb = 0; itb < nt_b; itb++){
	  hnh_ref[ich][itb] = 0;
	  for (Int_t ih=0;ih<max_nh;ih++) {
	    hlt_ref[ich][itb][ih]=-1;
	    htt_ref[ich][itb][ih]=-1;
	  }
	}
      }
	
      for (Int_t ich = 0; ich < nch_trig1; ich++){
	hnh_trig1[ich] = 0;
	for (Int_t ih=0;ih<max_nh;ih++) {
	  hlt_trig1[ich][ih]=-1;
	  htt_trig1[ich][ih]=-1;
	}
      }

    
      Int_t nhitl_channel[ntdc][nch]={0};//leading multiplicity per plane
      Int_t nhitt_channel[ntdc][nch]={0};//trailing multiplicity per plane

      for(Int_t itdc=0;itdc<ntdc;itdc++){
	Int_t nch_tdc = 64;
	
	//----------leading time----------

        for(Int_t ch = 0; ch < nch_tdc; ch++){
	  Int_t plane = ch/16+itdc*4;
	  Int_t channel = ch%16;
	  Int_t nhitl = 0;
	  Int_t nhitt = 0;
	  if (itdc==0) {
	    nhitl = ltdc0->at(ch).size();
	    nhitt = ttdc0->at(ch).size();
	  } else if (itdc==1) {
	    nhitl = ltdc1->at(ch).size();
	    nhitt = ttdc1->at(ch).size();
	  } else {
	    nhitl = ltdc2->at(ch).size();
	    nhitt = ttdc2->at(ch).size();
	  }

	  //sorting with vector in descending order
	  if (sort_TDC) {
	    if (itdc==0) {
	      vector<double> &v = ltdc0->at(ch);
	      if (debug) {
		cout << "before sort v " << endl;
		for (Int_t iv=0;iv<(int)v.size();iv++) {
		  cout << "v["<<iv<<"]= " << v[iv] << endl;
		}
	      }
	      sort(v.begin(), v.end(), greater<double>());
	      if (debug) {
		cout << "after sort v  " << endl;
		for (Int_t iv=0;iv<(int)v.size();iv++) {
		  cout << "v["<<iv<<"]= " << v[iv] << endl;
		}
	      }
	    } else if (itdc==1){
	      vector<double> &v = ltdc1->at(ch);
	      sort(v.begin(), v.end(), greater<double>());
	    } else {
	      vector<double> &v = ltdc2->at(ch);
	      sort(v.begin(), v.end(), greater<double>());	      
	    }
	  }
	  
	  nhitl_channel[itdc][ch] = nhitl;
	  nhitt_channel[itdc][ch] = nhitt;
	  Int_t nhit = nhitl;

	  if ((channel<0)||(channel>=16)||(plane<0)||(plane>=nplane)) {
	    if (detail) cerr << " Strange channel, plane = " << channel << " " << plane << endl;
	    exit(-1);
	  }
	  if (nhitl != nhitt) {
	    if (detail) cerr << "Inconsistent nhits> plane rchannel nhitl nhitt " << plane << " " << channel << " " << nhitl << " " << nhitt << endl;
	    if (nhitl<=nhitt) {
	      nhit = nhitl;
	    } else {
	      nhit = nhitt;
	    }
	  } else {

	  }

	  if (hist) cout << "h_nh[itdc][ch]->Fill(nhitl,nhitt)" << endl;
#ifdef HIST	  
	  h_nh[itdc][ch]->Fill(nhitl,nhitt);
#endif
	  
	  //---- leading time------
	  if (debug) cout << "nhitl = " << nhitl << endl;
	  
	  for(Int_t element = 0; element < nhitl; element++){
	    Double_t lt = 0;
	    if (itdc==0) {
	      lt = ltdc0->at(ch).at(element);
	    } else if (itdc==1) {
	      lt = ltdc1->at(ch).at(element);
	    } else {
	      lt = ltdc2->at(ch).at(element);
	    }
	    if (debug) cout << "@ itdc, ch, element, lt " << itdc << " " << ch << " " << element << " " << lt << endl;
#ifdef HIST
	    if (hist) cout << "h_le1[itdc][ch]->Fill" << endl;
	    if (element == 0) {
	      h_le1[itdc][ch] -> Fill(lt);
	    }
	    else if(element == 1) h_le2[itdc][ch] -> Fill(lt);
	    else h_le3[itdc][ch] -> Fill(lt);
#endif
	  }
	  
	  //----------trailing time----------
	  if (debug) cout << "nhitt = " << nhitt << endl;
	  if (debug) {
	    if ((itdc==0)&&(ttdc0->at(ch).size()>0)) {
	      cout << "@ should be empty ttdcs["<<itdc<<"]["<<ch<<"].size() = " << ttdc0->at(ch).size() << endl;
	    }
	    if ((itdc==1)&&(ttdc1->at(ch).size()>0)) {
	      cout << "@ should be empty ttdcs["<<itdc<<"]["<<ch<<"].size() = " << ttdc1->at(ch).size() << endl;
	    }
	    if ((itdc==2)&&(ttdc2->at(ch).size()>0)) {
	      cout << "@ should be empty ttdcs["<<itdc<<"]["<<ch<<"].size() = " << ttdc2->at(ch).size() << endl;
	    }
	  }
       
	  for(Int_t element = 0; element < nhitt; element++){
	    Double_t tt = 0;
	    if (itdc==0) {
	      tt = ttdc0->at(ch).at(element);
	    } else if (itdc==1) {
	      tt = ttdc1->at(ch).at(element);
	    } else  {
	      tt = ttdc2->at(ch).at(element);
	    }
	    if (debug) cout << "@ itdc, ch, element, tt " << itdc << " " << ch << " " << element << " " << tt << endl;
#ifdef HIST
	    if (hist) cout << "h_tr1[itdc][ch]->Fill" << endl;
	    if (element == 0) h_tr1[itdc][ch] -> Fill(tt);
	    else if(element == 1) h_tr2[itdc][ch] -> Fill(tt);
	    else h_tr3[itdc][ch] -> Fill(tt);
#endif
	  }
	   

	  if (debug) {
	    if (itdc==0) {
	      cout << "before paring, ltdcs[itdc][ch].size() = "
		   << ltdc0->at(ch).size() << endl;
	      
	      cout << "before paring, ttdcs[itdc][ch].size() = "
		   << ttdc0->at(ch).size() << endl;
	    } else if (itdc==1) {
	      cout << "before paring, ltdcs[itdc][ch].size() = "
		   << ltdc1->at(ch).size() << endl;
	      
	      cout << "before paring, ttdcs[itdc][ch].size() = "
		   << ttdc1->at(ch).size() << endl;
	    } else {
	      cout << "before paring, ltdcs[itdc][ch].size() = "
		   << ltdc2->at(ch).size() << endl;
	      
	      cout << "before paring, ttdcs[itdc][ch].size() = "
		   << ttdc2->at(ch).size() << endl;
	    }
	  }
	  
	  if (debug) cout << "=== going to paring, itdc, ch = " << itdc << " " << ch << endl;
	  Int_t nltdcs = 0;
		    
	  if (itdc==0) {
	    nltdcs = ltdc0->at(ch).size();
	  } else if (itdc==1) {
	    nltdcs = ltdc1->at(ch).size();
	  } else {
	    nltdcs = ltdc2->at(ch).size();
	  }
	  for (Int_t ie=0;ie<nltdcs;ie++) {
	    if (debug) cout << "ie = " << ie << endl;
	    Double_t lt = 0;
	    if (itdc==0) {
	      lt = ltdc0->at(ch).at(ie);
	    } else if (itdc==1) {
	      lt = ltdc1->at(ch).at(ie);
	    } else {
	      lt = ltdc2->at(ch).at(ie);
	    }
	    Int_t nttdcs = 0;
	    if (itdc==0) {
	      nttdcs = ttdc0->at(ch).size();
	    } else if (itdc==1) {
	      nttdcs = ttdc1->at(ch).size();
	    } else {
	      nttdcs = ttdc2->at(ch).size();
	    }
	    
	    for (Int_t je=0;je<nttdcs;je++) {
	      if (debug) cout << "je = " << je << endl;
	      Double_t tt = 0;
	      if (itdc==0) {
		tt = ttdc0->at(ch).at(je);
	      } else if (itdc==1) {
		tt = ttdc1->at(ch).at(je);
	      } else {
		tt = ttdc2->at(ch).at(je);
	      }
	      if (pairing_TDC) {
		if (lt>tt) {		
		  Double_t tt_prev = lt+1.;// if (je==0) ok
		  if (je>=1) {
		    if (itdc==0) {
		      tt_prev = ttdc0->at(ch).at(je-1);
		    } else if (itdc==1) {
		      tt_prev = ttdc1->at(ch).at(je-1);
		    } else  {
		      tt_prev = ttdc2->at(ch).at(je-1);
		    }
		  }
		  if (tt_prev>lt) {//ok
		    Double_t lt_next = tt-1.;// if (ie==ltdcs[itdc][ch].size()-1)
		    Int_t nnltdcs = 0;
		    if (ie<=(nltdcs-2)) {
		      if (itdc==0) {
			lt_next = ltdc0->at(ch).at(ie+1);
		      } else if (itdc==1) {
			lt_next = ltdc1->at(ch).at(ie+1);
		      } else  {
			lt_next = ltdc2->at(ch).at(ie+1);
		      }
		    }
		    if (tt>lt_next) {//good
		      if (debug) {
			cout << "!!!! found good pair" << endl;
			cout << "tt_prev, lt, tt, lt_next = " << tt_prev << " " << lt << " " << tt << " " << lt_next << endl;
			cout << "gltdcs, gttdcs.push_back, itdc, ch = " << itdc << " " << ch << endl;
		      }
		      if (itdc==0) {
			if ((0<=ch)&&(ch<nch_tdc)&&(mg0[ch]<max_pair)) {
			  gltdcs0[ch][mg0[ch]]=lt;
			  gttdcs0[ch][mg0[ch]]=tt;
			  mg0[ch]++;
			} else {
			  //			cout << "+++ wrong ch, mg0[ch] = " << ch << " " << mg0[ch] << endl;
			  //			cout << "+++ ch, nch, max_pair = " << ch << " " << nch << " " << max_pair << endl;
			  //			exit(-1);
			}
		      } else if (itdc==1) {
			if ((0<=ch)&&(ch<nch_tdc)&&(mg1[ch]<max_pair)) {
			  gltdcs1[ch][mg1[ch]]=lt;
			  gttdcs1[ch][mg1[ch]]=tt;
			  mg1[ch]++;
			} else {
			  //			cout << "+++ wrong ch, mg1[ch] = " << ch << " " << mg1[ch] << endl;
			  //			cout << "+++ ch, nch, max_pair = " << ch << " " << nch << " " << max_pair << endl;
			  //			exit(-1);
			  
			}
		      } else  {
			if ((0<=ch)&&(ch<nch_tdc)&&(mg2[ch]<max_pair)) {
			  gltdcs2[ch][mg2[ch]]=lt;
			  gttdcs2[ch][mg2[ch]]=tt;
			  mg2[ch]++;
			} else {
			  //			cout << "+++ wrong ch, mg1[ch] = " << ch << " " << mg1[ch] << endl;
			  //			cout << "+++ ch, nch, max_pair = " << ch << " " << nch << " " << max_pair << endl;
			  //			exit(-1);
			  
			}
		      }
		      break;
		    }
		  }// if (tt_prev>lt)
		} // if (lt>tt)
	      } else {//if (pairing_TDC)
		if ((ie==je)&&(je<nltdcs)) {
		  if (itdc==0) {
		    if ((0<=ch)&&(ch<nch_tdc)&&(mg0[ch]<max_pair)) {
		      gltdcs0[ch][mg0[ch]]=lt;
		      gttdcs0[ch][mg0[ch]]=tt;
		      mg0[ch]++;
		    } 
		  } else if (itdc==1) {
		    if ((0<=ch)&&(ch<nch_tdc)&&(mg1[ch]<max_pair)) {
		      gltdcs1[ch][mg1[ch]]=lt;
		      gttdcs1[ch][mg1[ch]]=tt;
		      mg1[ch]++;
		    } 
		  } else{ 
		    if ((0<=ch)&&(ch<nch_tdc)&&(mg2[ch]<max_pair)) {
		      gltdcs2[ch][mg2[ch]]=lt;
		      gttdcs2[ch][mg2[ch]]=tt;
		      mg2[ch]++;
		    } 
		  }
		}
	      }//if (paring_TDC)
		      
	    }
	  }
	
	
	  //check paring algorithm
	  if (debug) cout << "--- Before pairing, leading" << endl;
	  Int_t mltdcs = 0;
	  if (itdc==0) {
	    mltdcs = ltdc0->at(ch).size();
	  } else if (itdc==1) {
	    mltdcs = ltdc1->at(ch).size();
	  } else {
	    mltdcs = ltdc2->at(ch).size();
	  }

	  if (debug) {
	    for (Int_t ie=0;ie<mltdcs;ie++) {
	      if (itdc==0) {
		cout << "- itdc, ch, element, lt " << itdc << " " << ch << " " << ie << " " << ltdc0->at(ch).at(ie) << endl;
	      } else if (itdc==1) {
		cout << "- itdc, ch, element, lt " << itdc << " " << ch << " " << ie << " " << ltdc1->at(ch).at(ie) << endl;
	      } else {
		cout << "- itdc, ch, element, lt " << itdc << " " << ch << " " << ie << " " << ltdc2->at(ch).at(ie) << endl;
	      }
	    }
	  }

	  Int_t mttdcs = 0;
	  if (itdc==0) {
	    mttdcs = ttdc0->at(ch).size();
	  } else if (itdc==1) {
	    mttdcs = ttdc1->at(ch).size();
	  } else {
	    mttdcs = ttdc2->at(ch).size();
	  }
	  			
	  if (debug) {
	    cout << "--- Before pairing, trailing" << endl;
	    for (Int_t ie=0;ie<mttdcs;ie++) {
	      if (itdc==0) {
		cout << "- itdc, ch, element, tt " << itdc << " " << ch << " " << ie << " " << ttdc0->at(ch).at(ie) << endl;
	      } else if (itdc==1) {
		cout << "- itdc, ch, element, tt " << itdc << " " << ch << " " << ie << " " << ttdc1->at(ch).at(ie) << endl;
	      } else {
		cout << "- itdc, ch, element, tt " << itdc << " " << ch << " " << ie << " " << ttdc2->at(ch).at(ie) << endl;
	      }
	    }
	  }
	
	  if (debug) cout << "--- After pairing, leading, trailing" << endl;

	  Int_t ngltdcs = 0;
	  if (itdc==0) {
	    ngltdcs = mg0[ch];
	  } else if (itdc==1) {
	    ngltdcs = mg1[ch];
	  } else {
	    ngltdcs = mg2[ch];
	  }
	  if (debug)cout << "itdc, ngltdcs = " << itdc << " " << ngltdcs << endl;

	  if (debug) {
	    for (Int_t ie=0;ie<ngltdcs;ie++) {
	      if (itdc==0) {
		cout << "+ itdc, ch, element, lt, tt " << itdc << " " << ch << " " << ie << " " << gltdcs0[ch][ie] << " " << gttdcs0[ch][ie] << endl;
	      } else if (itdc==1) {
		cout << "+ itdc, ch, element, lt, tt " << itdc << " " << ch << " " << ie << " " << gltdcs1[ch][ie] << " " << gttdcs1[ch][ie] << endl;
	      } else {
		cout << "+ itdc, ch, element, lt, tt " << itdc << " " << ch << " " << ie << " " << gltdcs2[ch][ie] << " " << gttdcs2[ch][ie] << endl;
	      }
	    }
	  }
	
	
	  // width (sako)
	  for(Int_t element = 0; element < ngltdcs; element++){
	    Double_t lt = 0;
	    if (itdc==0) {
	      lt = gltdcs0[ch][element];
	    } else if (itdc==1) {
	      lt = gltdcs1[ch][element];
	    } else {
	      lt = gltdcs2[ch][element];
	    }
	    Double_t tt = 0;
	    if (itdc==0) {
	      tt = gttdcs0[ch][element];
	    } else if (itdc==1) {
	      tt = gttdcs1[ch][element];
	    } else {
	      tt = gttdcs2[ch][element];
	    }
	    
#ifdef HIST	      
	    if(element == 0)      {
	      if (hist) cout << "h_wi1[itdc][ch]->Fill" << endl;
	      h_wi1[itdc][ch] -> Fill(lt-tt);
	      h_lw1[itdc][ch] -> Fill(lt-tt,lt);
	      h_lt1[itdc][ch] -> Fill(lt,tt);
	    } else if(element == 1) {
	      if (hist) cout << "h_wi2[itdc][ch]->Fill" << endl;
	      h_wi2[itdc][ch] -> Fill(lt-tt);
	      h_lw2[itdc][ch] -> Fill(lt-tt,lt);
	      h_lt2[itdc][ch] -> Fill(lt,tt);
	    } else {
	      if (hist) cout << "h_wi3[itdc][ch]->Fill" << endl;
	      h_wi3[itdc][ch] -> Fill(lt-tt);
	      h_lw3[itdc][ch] -> Fill(lt-tt,lt);
	      h_lt3[itdc][ch] -> Fill(lt,tt);
	    }
#endif	      
	  }
	  //new correct	
	} //ch loop
      }//itdc loop

				      
    
	//calc multiplicity
	Int_t multil[nplane] = {0};
	Int_t multit[nplane] = {0};
	for (Int_t itdc=0;itdc<ntdc;itdc++) {
	  Int_t nch_tdc = 64;

	  for (Int_t ch=0;ch<nch_tdc;ch++) {
	    Int_t plane = ch/16+itdc*4;
	    if (nhitl_channel[itdc][ch]>0) {
	      multil[plane]++;
	    }
	    if (nhitt_channel[itdc][ch]>0) {
	      multit[plane]++;
	    }
	  }
	}
	//#ifdef HIST

	for (Int_t iplane=0;iplane<nplane;iplane++) {
	  if (hist) cout << "h_multil[iplane]->Fill, iplane = " << iplane << endl;
	  h_multil[iplane]->Fill(multil[iplane]);
	  h_multit[iplane]->Fill(multit[iplane]);
	}
	//#endif

	//#ifdef HIST

	//ch correlations
	//tdc,channel loop0

  	
	//use channel correlation analysis later    
	Bool_t iACs[n_ac];
	for (Int_t iac=0;iac<n_ac;iac++) {
	  iACs[iac] = false;
	}

	if (debug) cout << "after iACs " << endl;
	for (Int_t itdc=0;itdc<ntdc;itdc++) {
	  if (debug) cout << "itdc = " << itdc << endl;
	  Int_t nch_tdc = 64;
	  
	  for (Int_t ich=0;ich<nch;ich++) {
	    if (debug) cout << "ich = " << ich << endl;
	    Int_t niele = 0;
	    Int_t ichannel = ich%16;
	    Int_t ich_tot = ich+itdc*64;
	    Int_t iplane = ich/16+itdc*4;
	    
	    //	    niele = mg0[ich];

	    //	    cout << " " << mg0aa
	    if (itdc==0) {
	      niele = mg0[ich];
	    } else if (itdc==1) {
	      niele = mg1[ich];
	    } else {
	      niele = mg2[ich];
	    }

	    Bool_t iRPC0 = false;
	    Bool_t iRPC1 = false;
	    Bool_t iRPC2 = false;
	    Bool_t iTSC1 = false;
	    Bool_t iTSC2 = false;
	    Bool_t iTSC3 = false;
	    Bool_t iAC = false;
	    Bool_t iREF = false;

	    Bool_t iTrig1 = false;
	    
	    Int_t it_b = 1;//top=1,bottom=0
	    Int_t istrip = -1;
	    Int_t idet = -1;

	    if (debug) cout << "iplane = " << iplane << endl;
	    //run0e
	    if (iplane==0) {//it_b top=1, bottom=0
	      iRPC0 = true;
	      it_b = ichannel/8;
	      istrip = ichannel%8;
	    } else if (iplane==1) {
	      if ((ichannel==5)||(ichannel==13)) {
		istrip = 24;
		if (ichannel==5) {
		  it_b = 0;
		} else {
		  it_b = 1;
		}
		iTSC1 = true;
	      } else if ((ichannel==6)||(ichannel==14)) {
		idet=0;//REF0
		if (ichannel==6) {
		  it_b = 0;
		} else {
		  it_b = 1;
		}
		iREF=true;
	      } else {
		iRPC1 = true;
		it_b = ichannel/8;
		istrip = ichannel%8;
	      }
	    } else if (iplane==2) {
	      iRPC2 = true;
	      it_b = ichannel/8;
	      istrip = ichannel%8;
	    } else if ((3<=iplane)&&(iplane<=5)) {//TSC1
	      it_b = (ich_tot-48)/24;
	      istrip = (ich_tot-48)%24;
	      if ((0<=istrip)&&(istrip<nstrip_tsc1)) {
		iTSC1 = true;
	      }
	    } else if (iplane==6) {//TSC2
	      iTSC2 = true;
	      it_b = ichannel/8;
	      istrip = ichannel%8;
	    } else if ((7<=iplane)&&(iplane<=8)) {//TSC3
	      if (ichannel<=11) {
		iTSC3 = true;
		//		it_b = (ich_tot-112)/12;
		//		istrip = (ich_tot-112)%12;
		istrip = ichannel;
		it_b = iplane-7;
	      } else  {//REF1, REF2
		iREF = true;
		//		idet = (ichannel-12)/2+1;
		idet = (ichannel-12)+1;
		it_b = iplane-7;
	      }
	      
	    } else if (iplane==9) {
	      //	      cout << "iplane, ichannel = "<< iplane << " " << ichannel << endl;
	      if (ichannel==8) {//AC4-1
		iAC = true;
		idet = 3;
		iACs[idet] = true;
	      } else if (ichannel==9) {//AC4-2
		iAC = true;
		idet = 4;
		iACs[idet] = true;
	      } else if (ichannel==10) {//AC4-3
		iAC = true;
		idet = 5;
		iACs[idet] = true;
	      } else if (ichannel==11) {//AC4-4
		iAC = true;
		idet = 6;
		iACs[idet] = true;
	      } else if (ichannel==12) {//AC4-5
		iAC = true;
		idet = 7;
		iACs[idet] = true;
	      } else if (ichannel==13) {//AC1
		iAC = true;
		idet = 0;
		iACs[idet] = true;
	      } else if (ichannel==14) {//AC2
		iAC = true;
		idet = 1;
		iACs[idet] = true;
	      } else if (ichannel==15) {//AC3
		iAC = true;
		idet = 2;
		iACs[idet] = true;
	      }
	      //	      cout << "iAC, idet = "<< iAC << " " << idet << endl;
	    } else if (iplane==10) {
	      if (ichannel==2) {
		iTrig1 = true;
		istrip = 0;// L1 
	      } else if (ichannel==3) {
		iTrig1 = true;
		istrip = 1;// L1 raw
	      } else if (ichannel==4) {
		iTrig1 = true;
		istrip = 2;// RPC trig
	      } else if (ichannel==5) {
		iTrig1 = true;
		istrip = 3;// TSC trig
	      } else if (ichannel==6) {
		iTrig1 = true;
		istrip = 4;// AC trig
	      }
	    }


	    //	    cout << "iAC, niele = " << iAC << " " << niele<< endl;
	    for (Int_t iele=0;iele<niele;iele++) {
	      Double_t ilt=0;
	      if (itdc==0) {
		ilt = gltdcs0[ich][iele];
	      } else if (itdc==1) {
		ilt = gltdcs1[ich][iele];
	      } else {
		ilt = gltdcs2[ich][iele];
	      }
	      Double_t itt=0;
	      if (itdc==0) {
		itt = gttdcs0[ich][iele];
	      } else if (itdc==1) {
		itt = gttdcs1[ich][iele];
	      } else {
		itt = gttdcs2[ich][iele];
	      }
	      //	      cout << "iele  " << iele << endl;

	      //fill tree variables
	      if (iRPC0) {
		if ((0<=istrip)&&(istrip<nstrip_rpc)
		    &&(0<=it_b)&&(it_b<nt_b)
		    &&(0<=iele)&&(iele<max_nh)) {
		  hlt_rpc0[istrip][it_b][iele] = ilt;
		  htt_rpc0[istrip][it_b][iele] = itt;
		  hnh_rpc0[istrip][it_b]++;
		}
	      }
	      if (iRPC1) {
		if ((0<=istrip)&&(istrip<nstrip_rpc)
		    &&(0<=it_b)&&(it_b<nt_b)
		    &&(0<=iele)&&(iele<max_nh)) {
		  hlt_rpc1[istrip][it_b][iele] = ilt;
		  htt_rpc1[istrip][it_b][iele] = itt;
		  hnh_rpc1[istrip][it_b]++;
		}
	      }
	      if (iRPC2) {
		if ((0<=istrip)&&(istrip<nstrip_rpc)
		    &&(0<=it_b)&&(it_b<nt_b)
		    &&(0<=iele)&&(iele<max_nh)) {
		  hlt_rpc2[istrip][it_b][iele] = ilt;
		  htt_rpc2[istrip][it_b][iele] = itt;
		  hnh_rpc2[istrip][it_b]++;
		}
	      }
	      if (iTSC1) {
		if ((0<=istrip)&&(istrip<nstrip_tsc1)
		    &&(0<=it_b)&&(it_b<nt_b)
		    &&(0<=iele)&&(iele<max_nh)) {
		  hlt_tsc1[istrip][it_b][iele] = ilt;
		  htt_tsc1[istrip][it_b][iele] = itt;
		  hnh_tsc1[istrip][it_b]++;
		}
	      }
	      if (iTSC2) {
		if ((0<=istrip)&&(istrip<nstrip_tsc2)
		    &&(0<=it_b)&&(it_b<nt_b)
		    &&(0<=iele)&&(iele<max_nh)) {
		  hlt_tsc2[istrip][it_b][iele] = ilt;
		  htt_tsc2[istrip][it_b][iele] = itt;
		  hnh_tsc2[istrip][it_b]++;
		}
	      }
	      if (iTSC3) {
		if ((0<=istrip)&&(istrip<nstrip_tsc3)
		    &&(0<=it_b)&&(it_b<nt_b)
		    &&(0<=iele)&&(iele<max_nh)) {
		  hlt_tsc3[istrip][it_b][iele] = ilt;
		  htt_tsc3[istrip][it_b][iele] = itt;
		  hnh_tsc3[istrip][it_b]++;
		}
	      }

	      //	      cout << "iAC = " << iAC << endl;
	      if (iAC) {
		//		cout << "in if (iAC), idet, iele = " << idet << " "<<iele<< endl;
		if ((0<=idet)&&(idet<n_ac)
		    &&(0<=iele)&&(iele<max_nh)) {
		  //		  cout << "iAC, ilt, itt = " << ilt << " " << itt << " " << endl;
		  hlt_ac[idet][iele] = ilt;
		  htt_ac[idet][iele] = itt;
		  hnh_ac[idet]++;
		}
	      }
	      if (iREF) {
		if ((0<=idet)&&(idet<n_ref)
		    &&(0<=it_b)&&(it_b<nt_b)
		    &&(0<=iele)&&(iele<max_nh)) {
		  hlt_ref[idet][it_b][iele] = ilt;
		  htt_ref[idet][it_b][iele] = itt;
		  hnh_ref[idet][it_b]++;
		}
	      }

	      if (iTrig1) {
		if ((0<=istrip)&&(istrip<nch_trig1)
		    &&(0<=iele)&&(iele<max_nh)) {
		  hlt_trig1[istrip][iele] = ilt;
		  htt_trig1[istrip][iele] = itt;
		  hnh_trig1[istrip]++;
		}
	      }

	      if (debug) cout << "iRPC0, iRPC1, iRPC2, iTSC1, iTSC2, iTSC3 = "
			      << iRPC0<< " "<<iRPC1<<" "<<iRPC2<<" "<<iTSC1<<" "<<iTSC2<<" "<<iTSC3 << endl;
	      h_lch[iplane] -> Fill(ich%16);
	      h_tch[iplane] -> Fill(ich%16);

	      if (iAC) {
		hlt_AC[idet]->Fill(ilt);
		//	      htt_AC[idet]->Fill(itt);
		hwt_AC[idet]->Fill(ilt-itt);
		hlt_wt_AC[idet]->Fill(ilt-itt,ilt);
		//		cout << "iAC, idet = " << iAC << " " << idet << endl;
		hnh_AC[idet]->Fill(hnh_ac[idet]);
	      }
	      
	    }//iele

	  }//ich
	}//itdc
	
	//AC histos

	if (debug) cout<< "before ac4 calculation" << endl;

	//DRS4 information
	Int_t n_amp_ac4 = 0;
	Double_t amp_ac4 = 0;
	Int_t n_adc_ac4 = 0;
	Double_t adc_ac4 = 0;
	Double_t wt_ac4 = 0;
	Double_t lt_ac4 = 0;
	Int_t n_ac4 = 0;
	Bool_t good_AC[n_ac];
	for (Int_t iac=0;iac<n_ac;iac++) {
	  good_AC[iac] = false;
	}
	for (Int_t ich=0;ich<n_ac;ich++) {
	  if (debug) cout << "*ich = " << ich << endl;
	  Double_t n_amp = amp0->at(ich).size();//same channel as HRTDC
	  Double_t n_adc = adc0->at(ich).size();
	  Double_t n_bl = bl0->at(ich).size();
	  Int_t iac=-1;
	  if (ich<=4) {
	    iac = ich+3;
	  } else {
	    iac = ich-5;
	  }
	  if (debug) cout << "*iac = " << iac << endl;
	  if (debug) cout << "*n_amp = " << n_amp << endl;	  
	  if (n_amp>0) {
	    if (debug) cout << "*before hamp_ac = " << endl;	  
	    hamp_ac[iac] = amp0->at(ich).at(0);
	    if (debug) cout << "*before hamp_AC = " << endl;	  
	    hamp_AC[iac]->Fill(hamp_ac[iac]);
	    if (debug) cout << "*after hamp_AC = " << endl;
	    if ((4<=iac)&&(iac<=7)) {
	      n_amp_ac4 ++;
	      amp_ac4 += hamp_ac[iac];
	    }
	    if (debug) cout << "*hnh_ac["<<iac<<" = "<< hnh_ac[iac] << endl;
	    if (hnh_ac[iac]>0) {
	      Double_t lt = hlt_ac[iac][0];
	      Double_t wt = hlt_ac[iac][0]-htt_ac[iac][0];
	      hamp_wt_AC[iac]->Fill(wt,hamp_ac[iac]);
	      if ((4<=iac)&&(iac<=7)) {
		n_ac4 ++;
		wt_ac4 += wt;
		lt_ac4 += lt;
	      }
	    }
	  } else {
	    hamp_ac[iac] = 0;
	  }
	  if (debug) cout << "*n_adc = " << n_adc << endl;	  	  
	  if (n_adc>0) {
	    hadc_ac[iac] = adc0->at(ich).at(0);
	    hadc_AC[iac]->Fill(hadc_ac[iac]);
	    if ((4<=iac)&&(iac<=7)) {
	      n_adc_ac4 ++;
	      adc_ac4 += hadc_ac[iac];
	    }
	    if (hnh_ac[iac]>0) {
	      Double_t lt = hlt_ac[iac][0];
	      Double_t wt = hlt_ac[iac][0]-htt_ac[iac][0];
	      hadc_wt_AC[iac]->Fill(wt,hadc_ac[iac]);
	    }
	  } else {
	    hadc_ac[iac] = 0;
	  }

	  if (n_bl>0) {
	    hbl_ac[iac] = bl0->at(ich).at(0);
	    hbl_AC[iac]->Fill(hbl_ac[iac]);
	  } else {
	    hbl_ac[iac] = 0;
	  }
	  	
	  good_AC[iac] = false;
	  if (hnh_ac[iac]>0) {
	    Double_t lt = hlt_ac[iac][0];
	    if ((t_min_AC<=lt)&&(lt<=t_max_AC)) {
	      good_AC[iac] = true;
	    }
	  }

	  if (good_AC[iac]) {
	    hlt_AC_g[iac]->Fill(hlt_ac[iac][0]);
	    hlt_wt_AC_g[iac]->Fill(hlt_ac[iac][0]-htt_ac[iac][0],hlt_ac[iac][0]);
	  }
	}

	
	if (n_ac4>0) {
	  lt_ac4 /= (double)n_ac4;
	}
	
	if (amp_ac4>0) {
	  hamp_AC12->Fill(amp_ac4);
	  if (wt_ac4>0) {
	    hamp_wt_AC12->Fill(wt_ac4,amp_ac4);
	  }	
	}
	if (adc_ac4>0) {
	  hadc_AC12->Fill(adc_ac4);
	  if (wt_ac4>0) {
	    hadc_wt_AC12->Fill(wt_ac4,adc_ac4);
	  }	
	}

	if (debug) cout<< "after ac4 calculation" << endl;

	htree->Fill();

    //#endif		  
    
  }//event

#ifdef HIST
  hlt_AC[0]->SetAxisRange(0,4000);
  hlt_AC[0]->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC_run%s.gif",runtag));
  c1->Update();
  hlt_AC[1]->SetAxisRange(0,4000);
  hlt_AC[1]->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC1_run%s.gif",runtag));
  c1->Update();
  hlt_AC[2]->SetAxisRange(0,4000);
  hlt_AC[2]->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC2_run%s.gif",runtag));
  c1->Update();
  hlt_AC[3]->SetAxisRange(0,4000);
  hlt_AC[3]->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC3_run%s.gif",runtag));
  c1->Update();
  hlt_AC[4]->SetAxisRange(0,4000);
  hlt_AC[4]->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC4_run%s.gif",runtag));
  c1->Update();

  hlt_AC[5]->SetAxisRange(0,4000);
  hlt_AC[5]->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC5_run%s.gif",runtag));
  c1->Update();

  hlt_AC[6]->SetAxisRange(0,4000);
  hlt_AC[6]->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC6_run%s.gif",runtag));
  c1->Update();

  hlt_AC[7]->SetAxisRange(0,4000);
  hlt_AC[7]->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC7_run%s.gif",runtag));
  c1->Update();

  hnh_AC[0]->Draw();
  c1->SaveAs(Form("ana/monitor/hnh_AC_run%s.gif",runtag));
  c1->Update();

  hwt_AC[0]->Draw();
  c1->SaveAs(Form("ana/monitor/hwt_AC_run%s.gif",runtag));
  c1->Update();

  hlt_wt_AC[0]->SetAxisRange(0,4000,"Y");
  hlt_wt_AC[0]->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hlt_wt_AC_run%s.gif",runtag));
  c1->Update();

  hadc_AC[0]->SetAxisRange(0,300);
  hadc_AC[0]->Draw();
  c1->SaveAs(Form("ana/monitor/hadc_AC_run%s.gif",runtag));
  c1->Update();

  hamp_AC[0]->SetAxisRange(0,600);
  hamp_AC[0]->Draw();
  c1->SaveAs(Form("ana/monitor/hamp_AC_run%s.gif",runtag));
  c1->Update();

  hadc_wt_AC[0]->SetAxisRange(0,40,"X");
  hadc_wt_AC[0]->SetAxisRange(0,300,"Y");
  hadc_wt_AC[0]->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hadc_wt_AC_run%s.gif",runtag));
  c1->Update();

  hamp_wt_AC[0]->SetAxisRange(0,40,"X");
  hamp_wt_AC[0]->SetAxisRange(0,600,"Y");
  hamp_wt_AC[0]->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hamp_wt_AC_run%s.gif",runtag));
  c1->Update();

#endif

    
#ifdef HIST
  for (Int_t iac=0;iac<n_ac;iac++) {
    hlt_AC[iac]->Write();
    hlt_AC_g[iac]->Write();
    //    htt_AC[iac]->Write();
    hnh_AC[iac]->Write();
    hwt_AC[iac]->Write();
    hlt_wt_AC[iac]->Write();
    hlt_wt_AC_g[iac]->Write();
    hadc_AC[iac]->Write();
    hbl_AC[iac]->Write();
    hamp_AC[iac]->Write();
    hadc_wt_AC[iac]->Write();
    hamp_wt_AC[iac]->Write();
  }
  
  hlt_AC12->Write();
  hlt_wt_AC12->Write();
  hlt_wt_AC12_g->Write();
  hadc_AC12->Write();
  hamp_AC12->Write();
  hadc_wt_AC12->Write();
  hamp_wt_AC12->Write();

  htree->Write();
  fout->Close();
#endif
}

check_drs4_run0e::check_drs4_run0e(Int_t run, Int_t subrun, TString basedir) {
  base = basedir;

  cout << "base = " << base << endl;
  fileopen0(run,subrun);

  check(run, subrun);

    //    return 0;
    
}

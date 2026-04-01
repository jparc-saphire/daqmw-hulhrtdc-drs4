#define HIST
#define  check_ac_cc
#include "check_ac.h"
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
static Int_t ich_AC1 = 48;
static Int_t ich_AC2 = 51;
static Int_t ich_AC3 = 50;
static const Int_t ich_DRS4_AC1 = 0;
static const Int_t ich_DRS4_AC2 = 1;
static const Int_t ich_DRS4_AC3 = 2;
static Int_t maxevent=0;


int main(int argc,char *argv[]) {

  cout << "argc = " << argc << endl;
  if (argc<=1) {
    cerr << "Usage check_ac run [subrun] [maxevent]" << endl;
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

  check_ac(run, subrun, basedir);
}
double check_ac::call(vector<vector<double> > *vec, Int_t arg1, Int_t arg2){

    double val;
    try{
        val = vec -> at(arg1).at(arg2);
    }
    catch(...){
        val = -999;
    }

    return val;
}

void check_ac::check(Int_t run, Int_t subrun){
  char runtag[256];

  if (subrun==0) {
    sprintf(runtag,"%d", run);
  } else {
    sprintf(runtag,"%d_%d",run,subrun);
  }
  const Bool_t sort_TDC = true;
  const Bool_t pairing_TDC = true;
  const Bool_t detail = false;
  gStyle->SetOptFit(1111);
  gStyle->SetPadTopMargin(0.12);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.12);
  Double_t t_min_AC1 = 0;
  Double_t t_max_AC1 = 500;

  Double_t t_min_AC2 = 0;
  Double_t t_max_AC2 = 500;

  Double_t t_min_AC3 = 0;
  Double_t t_max_AC3 = 500;

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);

  TH1D *hlt_AC1 = new TH1D("hlt_AC1","hlt_AC1",4000,0,1000);
  TH1D *hlt_AC2 = new TH1D("hlt_AC2","hlt_AC2",4000,0,1000);
  TH1D *hlt_AC3 = new TH1D("hlt_AC3","hlt_AC3",4000,0,1000);

  TH1D *hlt_AC1_g = new TH1D("hlt_AC1_g","hlt_AC1_g",4000,0,1000);
  TH1D *hlt_AC2_g = new TH1D("hlt_AC2_g","hlt_AC2_g",4000,0,1000);
  TH1D *hlt_AC3_g = new TH1D("hlt_AC3_g","hlt_A32_g",4000,0,1000);

  TH1D *htt_AC1 = new TH1D("htt_AC1","htt_AC1",4000,0,1000);
  TH1D *htt_AC2 = new TH1D("htt_AC2","htt_AC2",4000,0,1000);
  TH1D *htt_AC3 = new TH1D("htt_AC3","htt_AC3",4000,0,1000);

  TH1D *hnh_AC1 = new TH1D("hnh_AC1","hnh_AC1",100,-0.5,99.5);
  TH1D *hnh_AC2 = new TH1D("hnh_AC2","hnh_AC2",100,-0.5,99.5);
  TH1D *hnh_AC3 = new TH1D("hnh_AC3","hnh_AC3",100,-0.5,99.5);
  
  TH1D *hwt_AC1 = new TH1D("hwt_AC1","hwt_AC1",400,0,100);
  TH1D *hwt_AC2 = new TH1D("hwt_AC2","hwt_AC2",400,0,100);
  TH1D *hwt_AC3 = new TH1D("hwt_AC3","hwt_AC3",400,0,100);
  
  TH2D *hlt_wt_AC1 = new TH2D("hlt_wt_AC1","hlt_wt_AC1",400,0,100,4000,0,1000);
  TH2D *hlt_wt_AC2 = new TH2D("hlt_wt_AC2","hlt_wt_AC2",400,0,100,4000,0,1000);
  TH2D *hlt_wt_AC3 = new TH2D("hlt_wt_AC3","hlt_wt_AC3",400,0,100,4000,0,1000);

  TH2D *hlt_wt_AC1_g = new TH2D("hlt_wt_AC1_g","hlt_wt_AC1_g",400,0,100,4000,0,1000);
  TH2D *hlt_wt_AC2_g = new TH2D("hlt_wt_AC2_g","hlt_wt_AC2_g",400,0,100,4000,0,1000);
  TH2D *hlt_wt_AC3_g = new TH2D("hlt_wt_AC3_g","hlt_wt_AC3_g",400,0,100,4000,0,1000);


  TH1D *hamp_AC1 = new TH1D("hamp_AC1","hamp_AC1",4000,0,1000);
  TH1D *hadc_AC1 = new TH1D("hadc_AC1","hadc_AC1",4000,0,1000);
  TH1D *hbl_AC1 = new TH1D("hbl_AC1","hbl_AC1",4000,0,1000);
  TH2D *hamp_wt_AC1 = new TH2D("hamp_wt_AC1","hamp_wt_AC1",400,0,100,4000,0,1000);
  TH2D *hadc_wt_AC1 = new TH2D("hadc_wt_AC1","hadc_wt_AC1",400,0,100,4000,0,1000);  
  TH2D *hadc_ev_AC1 = new TH2D("hadc_ev_AC1","hadc_ev_AC1",1000,0,100000,1000,0,1000);
  TH2D *hlt_ev_AC1 = new TH2D("hlt_ev_AC1","hlt_ev_AC1",1000,0,100000,250,0,1000);

  TH1D *hamp_AC2 = new TH1D("hamp_AC2","hamp_AC2",4000,0,1000);
  TH1D *hadc_AC2 = new TH1D("hadc_AC2","hadc_AC2",4000,0,1000);
  TH1D *hbl_AC2 = new TH1D("hbl_AC2","hbl_AC2",4000,0,1000);
  TH2D *hamp_wt_AC2 = new TH2D("hamp_wt_AC2","hamp_wt_AC2",400,0,100,4000,0,1000);
  TH2D *hadc_wt_AC2 = new TH2D("hadc_wt_AC2","hadc_wt_AC2",400,0,100,4000,0,1000);  
  TH2D *hadc_ev_AC2 = new TH2D("hadc_ev_AC2","hadc_ev_AC2",1000,0,100000,1000,0,1000);
  TH2D *hlt_ev_AC2 = new TH2D("hlt_ev_AC2","hlt_ev_AC2",1000,0,100000,250,0,1000);

  TH1D *hamp_AC3 = new TH1D("hamp_AC3","hamp_AC3",4000,0,1000);
  TH1D *hadc_AC3 = new TH1D("hadc_AC3","hadc_AC3",4000,0,1000);
  TH1D *hbl_AC3 = new TH1D("hbl_AC3","hbl_AC3",4000,0,1000);
  TH2D *hamp_wt_AC3 = new TH2D("hamp_wt_AC3","hamp_wt_AC3",400,0,100,4000,0,1000);
  TH2D *hadc_wt_AC3 = new TH2D("hadc_wt_AC3","hadc_wt_AC3",400,0,100,4000,0,1000);  
  TH2D *hadc_ev_AC3 = new TH2D("hadc_ev_AC3","hadc_ev_AC3",1000,0,100000,1000,0,1000);
  TH2D *hlt_ev_AC3 = new TH2D("hlt_ev_AC3","hlt_ev_AC3",1000,0,100000,250,0,1000);
  TH2D *hmt_tsc1_AC2 = new TH2D("hmt_tsc1_AC2","hmt_tsc1_AC2",24,-0.5,23.5,250,0,1000);
  TH2D *hmt_tsc2_AC2 = new TH2D("hmt_tsc2_AC2","hmt_tsc2_AC2",8,-0.5,7.5,250,0,1000);

  TFile *fout = new TFile(Form("ana/check_ac/check_ac_run%s.root", runtag),"RECREATE");
    
  Int_t hev=0;
  Int_t hrun=run;
  const Int_t max_nh = 20;
  const Int_t nt_b = 2;
  //TSC1,TSC2
  const Int_t nstrip_tsc1 = 24;
  const Int_t nstrip_tsc2 = 8;
  
    //AC
  const Int_t n_ac = 3;
  //RPCR
  const Int_t n_rpcr = 2;
  const Int_t nstrip_rpcr = 4;
  
  const Int_t nch_trig1 = 7;
  
  TTree *htree = new TTree("htree","all hits");

  Int_t hnh_tsc1[nstrip_tsc1][nt_b];
  Int_t hnh_tsc2[nstrip_tsc2][nt_b];
  //  Int_t hnh_ac1;
  //  Int_t hnh_ac2;
  //  Int_t hnh_ac3;
  Int_t hnh_ac[n_ac];
  Int_t hnw_ac[n_ac];//waveform
  Int_t hnh_rpcr[n_rpcr][nstrip_rpcr];
  Int_t hnh_trig1[nch_trig1];

  Double_t hlt_tsc1[nstrip_tsc1][nt_b][max_nh];
  Double_t htt_tsc1[nstrip_tsc1][nt_b][max_nh];
  Double_t hlt_tsc2[nstrip_tsc2][nt_b][max_nh];
  Double_t htt_tsc2[nstrip_tsc2][nt_b][max_nh];
  //  Double_t hlt_ac1[max_nh];
  //  Double_t htt_ac1[max_nh];
  //  Double_t hlt_ac2[max_nh];
  //  Double_t htt_ac2[max_nh];
  //  Double_t hlt_ac3[max_nh];
  //  Double_t htt_ac3[max_nh];
  Double_t hlt_ac[n_ac][max_nh];
  Double_t htt_ac[n_ac][max_nh];
  Double_t hlt_rpcr[n_rpcr][nstrip_rpcr][max_nh];
  Double_t htt_rpcr[n_rpcr][nstrip_rpcr][max_nh];
  Double_t hlt_trig1[nch_trig1][max_nh];
  Double_t htt_trig1[nch_trig1][max_nh];

  Double_t hamp_ac[n_ac][max_nh];
  Double_t hadc_ac[n_ac][max_nh];
  Double_t hbl_ac[n_ac][max_nh];
  /*
  Double_t hamp_ac1;
  Double_t hadc_ac1;
  Double_t hbl_ac1;
  
  Double_t hamp_ac2;
  Double_t hadc_ac2;
  Double_t hbl_ac2;
  
  Double_t hamp_ac3;
  Double_t hadc_ac3;
  Double_t hbl_ac3;
  */
  htree->Branch("ievt",&hev, "ev/I");
  //  htree->Branch("nh_ac1",&hnh_ac1,"nh_ac1/I");
  //  htree->Branch("nh_ac2",&hnh_ac2,"nh_ac2/I");
  //  htree->Branch("nh_ac3",&hnh_ac3,"nh_ac3/I");
  htree->Branch("nh_tsc1",hnh_tsc1,"nh_tsc1[24][2]/I");//[strip][up_down]
  htree->Branch("nh_tsc2",hnh_tsc2,"nh_tsc2[8][2]/I");//[strip][up_down]
  htree->Branch("nh_ac",hnh_ac,"nh_ac[3]/I");//[det]
  htree->Branch("nw_ac",hnw_ac,"nw_ac[3]/I");//[det] waveform
  htree->Branch("nh_rpcr",hnh_rpcr,"nh_rpcr[2][4]/I");//[det][strip]
  htree->Branch("nh_trig1",hnh_trig1,"nh_trig1[7]/I");//[ch]

  //  htree->Branch("lt_ac1",hlt_ac1,"lt_ac1[20]/D");//[max_hit]
  //  htree->Branch("lt_ac2",hlt_ac2,"lt_ac2[20]/D");//[max_hit]
  //  htree->Branch("lt_ac3",hlt_ac3,"lt_ac3[20]/D");//[max_hit]
  
  //  htree->Branch("tt_ac1",htt_ac1,"tt_ac1[20]/D");//[max_hit]
  //  htree->Branch("tt_ac2",htt_ac2,"tt_ac2[20]/D");//[max_hit]
  //  htree->Branch("tt_ac3",htt_ac3,"tt_ac3[20]/D");//[max_hit]

  //  htree->Branch("amp_ac1",&hamp_ac1,"amp1/D");
  //  htree->Branch("adc_ac1",&hadc_ac1,"adc1/D");
  //  htree->Branch("bl_ac1",&hbl_ac1,"bl1/D");
  
  //  htree->Branch("amp_ac2",&hamp_ac2,"amp2/D");
  //  htree->Branch("adc_ac2",&hadc_ac2,"adc2/D");
  //  htree->Branch("bl_ac2",&hbl_ac2,"bl2/D");
  
  //  htree->Branch("amp_ac3",&hamp_ac3,"amp3/D");
  //  htree->Branch("adc_ac3",&hadc_ac3,"adc3/D");
  //  htree->Branch("bl_ac3",&hbl_ac3,"bl3/D");
  htree->Branch("lt_tsc1",hlt_tsc1,"lt_tsc1[24][2][20]/D");
  htree->Branch("lt_tsc2",hlt_tsc2,"lt_tsc2[8][2][20]/D");
  htree->Branch("lt_ac",hlt_ac,"lt_ac[3][20]/D");
  htree->Branch("lt_rpcr",hlt_rpcr,"lt_rpcr[2][4][20]/D");
  htree->Branch("lt_trig1",hlt_trig1,"lt_trig1[7][20]/D");//trig1(MRPC/TSC/AC/RPCREF/L1/L1SD)

  htree->Branch("tt_tsc1",htt_tsc1,"tt_tsc1[24][2][20]/D");
  htree->Branch("tt_tsc2",htt_tsc2,"tt_tsc2[8][2][20]/D");
  htree->Branch("tt_ac",htt_ac,"tt_ac[3][20]/D");
  htree->Branch("tt_rpcr",htt_rpcr,"tt_rpcr[2][4][20]/D");
  htree->Branch("tt_trig1",htt_trig1,"tt_trig1[7][20]/D");//trig1(MRPC/TSC/AC/RPCREF/L1/L1SD)
  htree->Branch("amp_ac",hamp_ac,"amp_ac[3][20]/D");
  htree->Branch("adc_ac",hadc_ac,"adc_ac[3][20]/D");
  htree->Branch("bl_ac",hbl_ac,"bl_ac[3][20]/D");

  Int_t nentries = tree0 -> GetEntriesFast();
  Int_t nentries1 = tree1 -> GetEntriesFast();

  cout << "Event : " << nentries << endl;
  if (maxevent>0) {
    nentries = maxevent;
  }

  const Int_t shift = -1;
  for(Int_t event = 0; event < nentries; event++){
    Int_t sevent = event + shift;
    if (sevent<0) {
      sevent = 0;
    } else if (sevent>=nentries) {
      sevent = nentries-1;
    }
    hev = event;
    /*    hadc_ac1 = -1;
    hamp_ac1 = -1;
    hadc_ac2 = -1;
    hamp_ac2 = -1;
    hadc_ac3 = -1;
    hamp_ac3 = -1;
    */
    if ((event%10000) == 0) {
      cout << "event : " << event << endl;
    }
    Int_t ientry = tree0 -> LoadTree(event);
    tree0 -> GetEntry(ientry);
    Int_t jentry = tree1 -> LoadTree(sevent);
    tree1 -> GetEntry(jentry);



    for (Int_t is=0;is<n_ac;is++) {
      hnh_ac[is]=0;
      hnw_ac[is]=0;

      for (Int_t ih=0;ih<max_nh;ih++) {
	hlt_ac[is][ih]=-1;
	htt_ac[is][ih]=-1;
	hamp_ac[is][ih] = -1;
	hadc_ac[is][ih] = -1;
	hbl_ac[is][ih] = -1;
      }
    }

    
    /*
    hnh_ac1=0;
    hnh_ac2=0;
    hnh_ac3=0;

    for (Int_t ih=0;ih<max_nh;ih++) {
      hlt_ac1[ih]=-1;
      htt_ac1[ih]=-1;
      hlt_ac2[ih]=-1;
      htt_ac2[ih]=-1;
      hlt_ac3[ih]=-1;
      htt_ac3[ih]=-1;
    }
    */


    Int_t max_pair = 16;
    Double_t gltdcs0[nch][max_pair];
    Double_t gttdcs0[nch][max_pair];
    Int_t mg0[nch];//keep no. of hits

    for (Int_t ich=0;ich<nch;ich++) {
      mg0[ich] = 0;
      for (Int_t ip=0;ip<max_pair;ip++) {
	gltdcs0[ich][ip]=0;
	gttdcs0[ich][ip]=0;
      }
    }
		
    //check leading(L) > trailing(T) > leading(L) > trailing(T)... order
    //if LL or TT reject earlier (larger) one
    //order leading and trailing in decending order
    //Secure LT pairs and reject LLTTLLLT -> select  xLTxxxLT
    //Select only LT pairs

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
    for (Int_t ich = 0; ich < n_ac; ich++){
      hnh_ac[ich] = 0;
      for (Int_t ih=0;ih<max_nh;ih++) {
	hlt_ac[ich][ih]=-1;
	htt_ac[ich][ih]=-1;
      }
    }
    for (Int_t ich = 0; ich < n_rpcr; ich++){
      for (Int_t itb = 0; itb < nstrip_rpcr; itb++){
	hnh_rpcr[ich][itb] = 0;
	for (Int_t ih=0;ih<max_nh;ih++) {
	  hlt_rpcr[ich][itb][ih]=-1;
	  htt_rpcr[ich][itb][ih]=-1;
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
    
    
    Int_t nhitl_channel[nch]={0};//leading multiplicity for each channel
    Int_t nhitt_channel[nch]={0};//trailing multiplicity for each channel

    //----------leading time----------
      
    for (Int_t ch = 0; ch < nch; ch++){
      Int_t nhitl = 0;
      Int_t nhitt = 0;
      
      nhitl = ltdc0->at(ch).size();
      nhitt = ttdc0->at(ch).size();
	
      //sorting with vector in descending order
      if (sort_TDC) {
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
      }
	  
      nhitl_channel[ch] = nhitl;
      nhitt_channel[ch] = nhitt;
      Int_t nhit = nhitl;

      if (nhitl != nhitt) {

	if (nhitl<=nhitt) {
	  nhit = nhitl;
	} else {
	  nhit = nhitt;
	}
      } else {
	//	    cout << "consistent nhitl nhitt" << endl;
      }

      if (debug) {
	for (Int_t element = 0; element < nhitl; element++){
	  Double_t lt = ltdc0->at(ch).at(element);
	  cout << "@ ch, element, lt " << ch << " " << element << " " << lt << endl;
	}
      }
	
      //----------trailing time----------
      if (debug) cout << "nhitt = " << nhitt << endl;
      if (debug) {
	if ((ttdc0->at(ch).size()>0)) {
	  cout << "@ should be empty ttdcs["<<ch<<"].size() = " << ttdc0->at(ch).size() << endl;
	}
      }

      if (debug) {
	for (Int_t element = 0; element < nhitt; element++){
	  Double_t tt = 0;
	  tt = ttdc0->at(ch).at(element);
	  cout << "@ ch, element, tt " << ch << " " << element << " " << tt << endl;
	}
      }
	   
      if (debug) {
	cout << "before paring, ltdcs[ch].size() = "
	     << ltdc0->at(ch).size() << endl;
	
	cout << "before paring, ttdcs[ch].size() = "
	     << ttdc0->at(ch).size() << endl;	
      }
	  
      //pairing algorithm!! leading and trailing already sorted in descending order
      //force!
      //	  mg0[ch] = 0;
      //	  mg1[ch] = 0;
      if (debug) cout << "=== going to paring, ch = " << ch << endl;
      Int_t nltdcs = 0;
		    
      nltdcs = ltdc0->at(ch).size();

      for (Int_t ie=0;ie<nltdcs;ie++) {
	if (debug) cout << "ie = " << ie << endl;
	Double_t lt = ltdc0->at(ch).at(ie);
	Int_t nttdcs = 0;
	nttdcs = ttdc0->at(ch).size();
	
	for (Int_t je=0;je<nttdcs;je++) {
	  if (debug) cout << "je = " << je << endl;
	  Double_t tt = ttdc0->at(ch).at(je);
	  if (pairing_TDC) {
	    if (lt>tt) {		
	      Double_t tt_prev = lt+1.;// if (je==0) ok
	      if (je>=1) {
		tt_prev = ttdc0->at(ch).at(je-1);
	      }
	      if (tt_prev>lt) {//ok
		Double_t lt_next = tt-1.;// if (ie==ltdcs[itdc][ch].size()-1)
		Int_t nnltdcs = 0;
		if (ie<=(nltdcs-2)) {
		  lt_next = ltdc0->at(ch).at(ie+1);
		}
		if (tt>lt_next) {//good
		  if (debug) {
		    cout << "!!!! found good pair" << endl;
		    cout << "tt_prev, lt, tt, lt_next = " << tt_prev << " " << lt << " " << tt << " " << lt_next << endl;
		    cout << "gltdcs, gttdcs.push_back, ch = " << ch << endl;
		  }

		  if ((0<=ch)&&(ch<nch)&&(mg0[ch]<max_pair)) {
		    gltdcs0[ch][mg0[ch]]=lt;
		    gttdcs0[ch][mg0[ch]]=tt;
		    mg0[ch]++;
		  } 
		  
		  break;
		}
	      }
	    }
	  } else {//if (pairing_TDC)
	    if ((ie==je)&&(je<nltdcs)) {
	      if ((0<=ch)&&(ch<nch)&&(mg0[ch]<max_pair)) {
		gltdcs0[ch][mg0[ch]]=lt;
		gttdcs0[ch][mg0[ch]]=tt;
		mg0[ch]++;
	      } 
	    }
	  }//if (paring_TDC)
	}
      }
	
      //check paring algorithm
      if (debug) cout << "--- Before pairing, leading" << endl;
      Int_t mltdcs = ltdc0->at(ch).size();
      
      if (debug) {
	for (Int_t ie=0;ie<mltdcs;ie++) {
	  cout << "- ch, element, lt " << ch << " " << ie << " " << ltdc0->at(ch).at(ie) << endl;	  
	}
      }

      Int_t mttdcs = ttdc0->at(ch).size();
	  			
      if (debug) {
	cout << "--- Before pairing, trailing" << endl;
	for (Int_t ie=0;ie<mttdcs;ie++) {
	  cout << "- ch, element, tt " << ch << " " << ie << " " << ttdc0->at(ch).at(ie) << endl;
	}
      }

      if (debug) cout << "--- After pairing, leading, trailing" << endl;

      Int_t ngltdcs = ngltdcs = mg0[ch];
      if (debug)cout << "ngltdcs = " << ngltdcs << endl;
      
      if (debug) {
	for (Int_t ie=0;ie<ngltdcs;ie++) {
	  cout << "+ ch, element, lt, tt " << ch << " " << ie << " " << gltdcs0[ch][ie] << " " << gttdcs0[ch][ie] << endl;
	  
	}
      }
	
      // width (sako)
      for(Int_t element = 0; element < ngltdcs; element++){
	Double_t lt = gltdcs0[ch][element];
	Double_t tt = gttdcs0[ch][element];
      }

    } //ch loop

#ifdef HIST

    //check tsc1 and tsc2
    const Int_t nstrip_tsc1 = 24;
    const Int_t nstrip_tsc2 = 8;
    Double_t lt_tsc1[nstrip_tsc1][2]={0};
    Double_t lt_tsc2[nstrip_tsc2][2]={0};
    Double_t tt_tsc1[nstrip_tsc1][2]={0};
    Double_t tt_tsc2[nstrip_tsc2][2]={0};
    Double_t mt_tsc1[nstrip_tsc1]={0};
    Double_t dt_tsc1[nstrip_tsc1]={0};
    Double_t wt_tsc1[nstrip_tsc1]={0};
    Double_t mt_tsc2[nstrip_tsc2]={0};
    Double_t dt_tsc2[nstrip_tsc2]={0};
    Double_t wt_tsc2[nstrip_tsc2]={0};

    //ch correlations
    //tdc,channel loop0
    //	for (Int_t itdc=0;itdc<ntdc;itdc++) {
	  
    for (Int_t ich=0;ich<nch;ich++) {
      Int_t itdc=1;
      Int_t niele = mg0[ich];
      Int_t ichannel = ich%16;
      Int_t ich_tot = ich+itdc*64;
      Int_t iplane = ich/16+itdc*4;

      //      Bool_t iAC1 = false;
      //      Bool_t iAC2 = false;
      //      Bool_t iAC3 = false;
      
      Bool_t iTSC1 = false;
      Bool_t iTSC2 = false;
      Bool_t iAC = false;
      Bool_t iRPCR = false;

      Bool_t iTrig1 = false;
      /*
      if ((ich==ich_AC1)&&(niele>0)) {
	iAC1 = true;
      } else if ((ich==ich_AC2)&&(niele>0)) {
	iAC2 = true;
      } else if ((ich==ich_AC3)&&(niele>0)) {
	iAC3 = true;
      } 
      */

      Int_t it_b = -1;
      Int_t istrip = -1;
      Int_t idet = -1;
      if (iplane==4) {//tsc1
	it_b = (ich_tot-32)/24;
	istrip = (ich_tot-32)%24;
	//	cout << "TSC1: ich_tot, it_b, istrip = " << ich_tot << " "  << it_b << " " << istrip << endl;
	if ((0<=istrip)&&(istrip<nstrip_tsc1)) {
	  iTSC1 = true;
	}
      } else if (iplane==6) {//TSC2
	iTSC2 = true;
	it_b = ichannel/8;
	istrip = ichannel%8;
	//	      cout << "+++ iTSC2, ichannel, it_b, istrip = " << iTSC2 << " " << ichannel << " " << it_b << " " << istrip << endl;
      } else if (iplane==7) {
	if ((0<=ichannel)&&(ichannel<=3)) {//AC
	  if (ichannel==0) {
	    iAC = true;
	    idet = 0;
	  } else if (ichannel==2) {
	    iAC = true;
	    idet = 2;
	  } else if (ichannel==3) {
	    iAC = true;
	    idet = 1;
	  }
	} else if ((8<=ichannel)&&(ichannel<=15)) {
	  iRPCR = true;
	  idet = (ichannel-8)/4;
	  istrip = (ichannel-8)%4;
	}
      }

      //RUN0d
      //trig1
      if (iplane==5) {
	if (ichannel==0) {
	  iTrig1 = true;
	  istrip = 0;// MRPC trig
	} else if (ichannel==1) {
	  iTrig1 = true;
	  istrip = 1;// TSC trig
	      } else if (ichannel==2) {
	  iTrig1 = true;
	  istrip = 2;// AC trig
	} else if (ichannel==3) {
	  iTrig1 = true;
	  istrip = 3;// RPCREF trig
	  //	  cout << "RPCREF trig!!" << endl;
	} else if (ichannel==4) {
	  iTrig1 = true;
	  istrip = 4;// L1 raw
	} else if (ichannel==5) {
	  iTrig1 = true;
	  istrip = 5;// L1 delay scale down
	} else if (ichannel==6) {
	  iTrig1 = true;
	  istrip = 6;// TSC2 trig
	}
      }
      
      
      Int_t itb_tsc1 = -1;//top=1,bottom=0
      Int_t is_tsc1 = -1;
      Int_t itb_tsc2 = -1;//top=1,bottom=0
      Int_t is_tsc2 = -1;

      for (Int_t iele=0;iele<niele;iele++) {
	
	Double_t ilt = gltdcs0[ich][iele];
	Double_t itt = gttdcs0[ich][iele];

	Double_t iwt = ilt-itt;
	const Int_t ich_TSC1_min = 0;
	const Int_t ich_TSC1_max = 15;
	const Int_t ich_TSC2_min = 32;
	const Int_t ich_TSC2_max = 47;

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
	if (iAC) {
	  if ((0<=idet)&&(idet<n_ac)
	      &&(0<=iele)&&(iele<max_nh)) {
	    hlt_ac[idet][iele] = ilt;
	    htt_ac[idet][iele] = itt;
	    hnh_ac[idet]++;

	    if (iele==0) {
	      if (idet==0) {
		hlt_AC1->Fill(ilt);
		hlt_ev_AC1->Fill(event,ilt);
		htt_AC1->Fill(itt);
		hwt_AC1->Fill(iwt);
		hlt_wt_AC1->Fill(iwt,ilt);
	      } else if (idet==1) {
		hlt_AC2->Fill(ilt);
		hlt_ev_AC2->Fill(event,ilt);
		htt_AC2->Fill(itt);
		hwt_AC2->Fill(iwt);
		hlt_wt_AC2->Fill(iwt,ilt);
	      } else if (idet==2) {
		hlt_AC3->Fill(ilt);
		hlt_ev_AC3->Fill(event,ilt);
		htt_AC3->Fill(itt);
		hwt_AC3->Fill(iwt);
		hlt_wt_AC3->Fill(iwt,ilt);
	      }
	    }
	  }
	}
	if (iRPCR) {
	  if ((0<=idet)&&(idet<n_rpcr)
	      &&(0<=istrip)&&(istrip<nstrip_rpcr)
	      &&(0<=iele)&&(iele<max_nh)) {
	    hlt_rpcr[idet][istrip][iele] = ilt;
	    htt_rpcr[idet][istrip][iele] = itt;
	    hnh_rpcr[idet][istrip]++;
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

      }//iele

      //check tsc
      for (Int_t is=0;is<nstrip_tsc1;is++) {
	if ((hlt_tsc1[is][1][0]>0)&&(hlt_tsc1[is][0][0]>0)) {
	  //mt, wt
	  mt_tsc1[is] = (hlt_tsc1[is][1][0]+hlt_tsc1[is][0][0])/2.;
	  dt_tsc1[is] = (hlt_tsc1[is][1][0]-hlt_tsc1[is][0][0]);
	  wt_tsc1[is] = ((hlt_tsc1[is][1][0]-htt_tsc1[is][1][0])+(hlt_tsc1[is][0][0]-htt_tsc1[is][0][0]))/2.;
	}
      }
    }//ich


    hnh_AC1->Fill(hnh_ac[0]);
    hnh_AC2->Fill(hnh_ac[1]);
    hnh_AC3->Fill(hnh_ac[2]);

    //waveform analysis

    /*sako
    Double_t n_amp1 = amp0->at(ich_DRS4_AC1).size();
    Double_t n_adc1 = adc0->at(ich_DRS4_AC1).size();
    Double_t n_bl1 = bl0->at(ich_DRS4_AC1).size();
    */
    
    Double_t n_amp1 = amp1->at(ich_DRS4_AC1).size();
    Double_t n_adc1 = adc1->at(ich_DRS4_AC1).size();
    Double_t n_bl1  = bl1->at(ich_DRS4_AC1).size();

    hnw_ac[0]=n_amp1;
    if (n_adc1<n_amp1) {
      hnw_ac[0] = n_adc1;
    }
    if (n_bl1<n_amp1) {
      hnw_ac[0] = n_bl1;
    }
    if (hnw_ac[0]>max_nh) {
      hnw_ac[0] = max_nh;
    }

    if (hnw_ac[0]>0) {
      for (Int_t ie=0;ie<hnw_ac[0];ie++) {
	hamp_ac[0][ie] = amp1->at(ich_DRS4_AC1).at(ie);
	hadc_ac[0][ie] = adc1->at(ich_DRS4_AC1).at(ie);
	hbl_ac[0][ie]  = bl1->at(ich_DRS4_AC1).at(ie);
	if (ie==0) {
	  hamp_AC1->Fill(hamp_ac[0][ie]);
	  hadc_AC1->Fill(hadc_ac[0][ie]);
	  hadc_ev_AC1->Fill(event,hadc_ac[0][ie]);
	  hbl_AC1->Fill(hbl_ac[0][ie]);
	  if (hnh_ac[0]>0) {
	    Double_t lt = hlt_ac[0][0];
	    Double_t wt = hlt_ac[0][0]-htt_ac[0][0];
	    hamp_wt_AC1->Fill(wt,hamp_ac[0][ie]);
	    hadc_wt_AC1->Fill(wt,hadc_ac[0][ie]);
	  }
	}
      }
    }

    
    Double_t n_amp2 = amp1->at(ich_DRS4_AC2).size();
    Double_t n_adc2 = adc1->at(ich_DRS4_AC2).size();
    Double_t n_bl2  = bl1->at(ich_DRS4_AC2).size();

    hnw_ac[1]=n_amp2;
    if (n_adc2<n_amp2) {
      hnw_ac[1] = n_adc2;
    }
    if (n_bl2<n_amp2) {
      hnw_ac[1] = n_bl2;
    }
    if (hnw_ac[1]>max_nh) {
      hnw_ac[1] = max_nh;
    }

    if (hnw_ac[1]>0) {
      for (Int_t ie=0;ie<hnw_ac[1];ie++) {
	hamp_ac[1][ie] = amp1->at(ich_DRS4_AC2).at(ie);
	hadc_ac[1][ie] = adc1->at(ich_DRS4_AC2).at(ie);
	hbl_ac[1][ie]  = bl1->at(ich_DRS4_AC2).at(ie);
	if (ie==0) {
	  hamp_AC2->Fill(hamp_ac[1][ie]);
	  hadc_AC2->Fill(hadc_ac[1][ie]);
	  hadc_ev_AC2->Fill(event,hadc_ac[1][ie]);
	  hbl_AC2->Fill(hbl_ac[1][ie]);
	  if (hnh_ac[1]>0) {
	    Double_t lt = hlt_ac[1][0];
	    Double_t wt = hlt_ac[1][0]-htt_ac[1][0];
	    hamp_wt_AC2->Fill(wt,hamp_ac[1][0]);
	    hadc_wt_AC2->Fill(wt,hadc_ac[1][0]);
	  }
	}
      }
    }
  



    Double_t n_amp3 = amp1->at(ich_DRS4_AC3).size();
    Double_t n_adc3 = adc1->at(ich_DRS4_AC3).size();
    Double_t n_bl3  = bl1->at(ich_DRS4_AC3).size();

    hnw_ac[2]=n_amp3;
    if (n_adc3<n_amp3) {
      hnw_ac[2] = n_adc3;
    }
    if (n_bl3<n_amp3) {
      hnw_ac[2] = n_bl3;
    }
    if (hnw_ac[2]>max_nh) {
      hnw_ac[2] = max_nh;
    }

    if (hnw_ac[2]>0) {
      for (Int_t ie=0;ie<hnw_ac[2];ie++) {
	hamp_ac[2][ie] = amp1->at(ich_DRS4_AC3).at(ie);
	hadc_ac[2][ie] = adc1->at(ich_DRS4_AC3).at(ie);
	hbl_ac[2][ie]  = bl1->at(ich_DRS4_AC3).at(ie);
	if (ie==0) {
	  hamp_AC3->Fill(hamp_ac[2][ie]);
	  hadc_AC3->Fill(hadc_ac[2][ie]);
	  hadc_ev_AC3->Fill(event,hadc_ac[2][ie]);
	  hbl_AC3->Fill(hbl_ac[2][ie]);
	  if (hnh_ac[2]>0) {
	    Double_t lt = hlt_ac[2][0];
	    Double_t wt = hlt_ac[2][0]-htt_ac[2][0];
	    hamp_wt_AC3->Fill(wt,hamp_ac[2][0]);
	    hadc_wt_AC3->Fill(wt,hadc_ac[2][0]);
	  }
	}
      }
    }


    Bool_t good_AC1 = false;
    if (hnh_ac[0]>0) {
      Double_t lt = hlt_ac[0][0];
      if ((t_min_AC1<=lt)&&(lt<=t_max_AC1)) {
	good_AC1 = true;
      }
    }
    Bool_t good_AC2 = false;
    if (hnh_ac[1]>0) {
      Double_t lt = hlt_ac[1][0];
      if ((t_min_AC2<=lt)&&(lt<=t_max_AC2)) {
	good_AC2 = true;
      }
    }
    Bool_t good_AC3 = false;
    if (hnh_ac[2]>0) {
      Double_t lt = hlt_ac[2][0];
      if ((t_min_AC3<=lt)&&(lt<=t_max_AC3)) {
	good_AC3 = true;
      }
    }

    if (good_AC1) {
      hlt_AC1_g->Fill(hlt_ac[0][0]);
      hlt_wt_AC1_g->Fill(hlt_ac[0][0]-htt_ac[0][0],hlt_ac[0][0]);
    }
    if (good_AC2) {
      hlt_AC2_g->Fill(hlt_ac[1][0]);
      hlt_wt_AC2_g->Fill(hlt_ac[1][0]-htt_ac[1][0],hlt_ac[1][0]);
    }
    if (good_AC3) {
      hlt_AC3_g->Fill(hlt_ac[2][0]);
      hlt_wt_AC3_g->Fill(hlt_ac[2][0]-htt_ac[2][0],hlt_ac[2][0]);
    }
    
    htree->Fill();

#endif		  

    
  }

#ifdef HIST

  hlt_AC1->SetAxisRange(0,4000);
  hlt_AC1->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC1_run%s.gif",runtag));
  c1->Update();
  
  hlt_AC2->SetAxisRange(0,4000);
  hlt_AC2->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC2_run%s.gif",runtag));
  c1->Update();

  hlt_AC3->SetAxisRange(0,4000);
  hlt_AC3->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC3_run%s.gif",runtag));
  c1->Update();

  hnh_AC1->Draw();
  c1->SaveAs(Form("ana/monitor/hnh_AC1_run%s.gif",runtag));
  c1->Update();

  hnh_AC2->Draw();
  c1->SaveAs(Form("ana/monitor/hnh_AC2_run%s.gif",runtag));
  c1->Update();
  
  hnh_AC3->Draw();
  c1->SaveAs(Form("ana/monitor/hnh_AC3_run%s.gif",runtag));
  c1->Update();

  hwt_AC1->Draw();
  c1->SaveAs(Form("ana/monitor/hwt_AC1_run%s.gif",runtag));
  c1->Update();

  hwt_AC2->Draw();
  c1->SaveAs(Form("ana/monitor/hwt_AC2_run%s.gif",runtag));
  c1->Update();
  
  hwt_AC3->Draw();
  c1->SaveAs(Form("ana/monitor/hwt_AC3_run%s.gif",runtag));
  c1->Update();

  hlt_wt_AC1->SetAxisRange(100,150,"Y");
  hlt_wt_AC1->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hlt_wt_AC1_run%s.gif",runtag));

  hlt_wt_AC2->SetAxisRange(100,150,"Y");
  hlt_wt_AC2->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hlt_wt_AC2_run%s.gif",runtag));
  
  hlt_wt_AC3->SetAxisRange(100,150,"Y");
  hlt_wt_AC3->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hlt_wt_AC3_run%s.gif",runtag));

  hadc_AC1->SetAxisRange(0,300);
  hadc_AC1->Draw();
  c1->SaveAs(Form("ana/monitor/hadc_AC1_run%s.gif",runtag));
  c1->Update();

  hadc_AC2->SetAxisRange(0,300);
  hadc_AC2->Draw();
  c1->SaveAs(Form("ana/monitor/hadc_AC2_run%s.gif",runtag));
  c1->Update();
  
  hadc_AC3->SetAxisRange(0,300);
  hadc_AC3->Draw();
  c1->SaveAs(Form("ana/monitor/hadc_AC3_run%s.gif",runtag));
  c1->Update();
  
  hamp_AC1->SetAxisRange(0,300);
  hamp_AC1->Draw();
  c1->SaveAs(Form("ana/monitor/hamp_AC1_run%s.gif",runtag));
  c1->Update();

  hamp_AC2->SetAxisRange(0,300);
  hamp_AC2->Draw();
  c1->SaveAs(Form("ana/monitor/hamp_AC2_run%s.gif",runtag));
  c1->Update();
  
  hamp_AC3->SetAxisRange(0,300);
  hamp_AC3->Draw();
  c1->SaveAs(Form("ana/monitor/hamp_AC3_run%s.gif",runtag));
  c1->Update();
  
  hadc_wt_AC1->SetAxisRange(0,40,"X");
  hadc_wt_AC1->SetAxisRange(0,300,"Y");
  hadc_wt_AC1->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hadc_wt_AC1_run%s.gif",runtag));
  c1->Update();

  hadc_wt_AC2->SetAxisRange(0,40,"X");
  hadc_wt_AC2->SetAxisRange(0,300,"Y");
  hadc_wt_AC2->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hadc_wt_AC2_run%s.gif",runtag));
  c1->Update();

  hadc_wt_AC3->SetAxisRange(0,40,"X");
  hadc_wt_AC3->SetAxisRange(0,300,"Y");
  hadc_wt_AC3->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hadc_wt_AC3_run%s.gif",runtag));
  c1->Update();

  hamp_wt_AC1->SetAxisRange(0,40,"X");
  hamp_wt_AC1->SetAxisRange(0,300,"Y");
  hamp_wt_AC1->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hamp_wt_AC1_run%s.gif",runtag));
  c1->Update();

  hamp_wt_AC2->SetAxisRange(0,40,"X");
  hamp_wt_AC2->SetAxisRange(0,300,"Y");
  hamp_wt_AC2->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hamp_wt_AC2_run%s.gif",runtag));
  c1->Update();

  hamp_wt_AC3->SetAxisRange(0,40,"X");
  hamp_wt_AC3->SetAxisRange(0,300,"Y");
  hamp_wt_AC3->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hamp_wt_AC3_run%s.gif",runtag));
  c1->Update();
  
#endif

    
#ifdef HIST
  hlt_AC1->Write();
  hlt_AC2->Write();
  hlt_AC3->Write();
  hlt_ev_AC1->Write();
  hlt_ev_AC2->Write();
  hlt_ev_AC3->Write();

  hlt_AC1_g->Write();
  hlt_AC2_g->Write();
  hlt_AC3_g->Write();
  
  
  htt_AC1->Write();
  htt_AC2->Write();
  htt_AC3->Write();

  hnh_AC1->Write();
  hnh_AC2->Write();
  hnh_AC3->Write();

  hwt_AC1->Write();
  hwt_AC2->Write();
  hwt_AC3->Write();

  hlt_wt_AC1->Write();
  hlt_wt_AC2->Write();
  hlt_wt_AC3->Write();

  hlt_wt_AC1_g->Write();
  hlt_wt_AC2_g->Write();
  hlt_wt_AC3_g->Write();

  hadc_AC1->Write();
  hadc_AC2->Write();
  hadc_AC3->Write();
  hadc_ev_AC1->Write();
  hadc_ev_AC2->Write();
  hadc_ev_AC3->Write();
  hamp_AC1->Write();
  hamp_AC2->Write();
  hamp_AC3->Write();
  hadc_wt_AC1->Write();
  hadc_wt_AC2->Write();
  hadc_wt_AC3->Write();
  hamp_wt_AC1->Write();
  hamp_wt_AC2->Write();
  hamp_wt_AC3->Write();

#endif

  htree->Write();
  fout->Close();

}

check_ac::check_ac(Int_t run, Int_t subrun, TString basedir) {
  base = basedir;

  cout << "base = " << base << endl;
  fileopen0(run,subrun);

  check(run, subrun);

    //    return 0;
    
}

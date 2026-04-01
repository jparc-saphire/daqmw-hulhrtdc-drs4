#define HIST
#define  check_actsc_cc
#include "check_actsc.h"
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

static Int_t ich_AC  = 32;
static Int_t ich_AC1 = 33;
static Int_t ich_AC2 = 34;
static Int_t ich_AC3 = 35;
static Int_t ich_AC4 = 36;
/*
static Int_t ich_AC  = 47;
static Int_t ich_AC1 = 46;
static Int_t ich_AC2 = 45;
static Int_t ich_AC3 = 44;
static Int_t ich_AC4 = 43;
*/
static const Int_t ich_DRS4_AC  = 0;
static const Int_t ich_DRS4_AC1 = 1;
static const Int_t ich_DRS4_AC2 = 2;
static const Int_t ich_DRS4_AC3 = 3;
static const Int_t ich_DRS4_AC4 = 4;


static const Int_t ich_SH1 = 0;
static const Int_t ich_SH2 = 8;
static const Int_t ich_SV1 = 24;

/*
static const Int_t ich_SH1 = 15;
static const Int_t ich_SH2 = 7;
static const Int_t ich_SV1 = 31;
*/

static Int_t maxevent=0;

int main(int argc,char *argv[]) {

  cout << "argc = " << argc << endl;
  if (argc<=1) {
    cerr << "Usage check_actsc run [subrun] [maxevent]" << endl;
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

  check_actsc(run, subrun, basedir);
}
double check_actsc::call(vector<vector<double> > *vec, Int_t arg1, Int_t arg2){

    double val;
    try{
        val = vec -> at(arg1).at(arg2);
    }
    catch(...){
        val = -999;
    }

    return val;
}

void check_actsc::check(Int_t run, Int_t subrun){


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

  Double_t t_min_SV1 = 244;
  Double_t t_max_SV1 = 249;

  //tight
  //  Double_t t_min_SV1 = 117.7;
  //  Double_t t_max_SV1 = 120.5;

  Double_t t_min_SH1 = 254;
  Double_t t_max_SH1 = 270;

  Double_t t_min_SH2 = 254;
  Double_t t_max_SH2 = 270;

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);

  Int_t nt = 5000;
  Int_t maxt = 500;
  Int_t maxam = 600;
  Int_t maxad = 300;
  TH1D *hlt_AC = new TH1D("hlt_AC","hlt_AC",nt,0,maxt);
  TH1D *hlt_AC1 = new TH1D("hlt_AC1","hlt_AC1",nt,0,maxt);
  TH1D *hlt_AC2 = new TH1D("hlt_AC2","hlt_AC2",nt,0,maxt);
  TH1D *hlt_AC3 = new TH1D("hlt_AC3","hlt_AC3",nt,0,maxt);
  TH1D *hlt_AC4 = new TH1D("hlt_AC4","hlt_AC4",nt,0,maxt);
  TH1D *hlt_AC12 = new TH1D("hlt_AC12","hlt_AC12",nt,0,maxt);
  TH1D *hlt_SH1 = new TH1D("hlt_SH1","hlt_SH1",nt,0,maxt);
  TH1D *hlt_SH2 = new TH1D("hlt_SH2","hlt_SH2",nt,0,maxt);
  TH1D *hlt_SV1 = new TH1D("hlt_SV1","hlt_SV1",nt,0,maxt);

  TH1D *hlt_AC_g = new TH1D("hlt_AC_g","hlt_AC_g",nt,0,maxt);
  TH1D *hlt_AC1_g = new TH1D("hlt_AC1_g","hlt_AC1_g",nt,0,maxt);
  TH1D *hlt_AC2_g = new TH1D("hlt_AC2_g","hlt_AC2_g",nt,0,maxt);
  TH1D *hlt_AC3_g = new TH1D("hlt_AC3_g","hlt_AC3_g",nt,0,maxt);
  TH1D *hlt_AC4_g = new TH1D("hlt_AC4_g","hlt_AC4_g",nt,0,maxt);
  TH1D *hlt_SH1_g = new TH1D("hlt_SH1_g","hlt_SH1_g",nt,0,maxt);
  TH1D *hlt_SH2_g = new TH1D("hlt_SH2_g","hlt_SH2_g",nt,0,maxt);
  TH1D *hlt_SV1_g = new TH1D("hlt_SV1_g","hlt_SV1_g",nt,0,maxt);

  TH1D *hlt_AC_a = new TH1D("hlt_AC_a","hlt_AC_a",nt,0,maxt);
  TH1D *hlt_AC1_a = new TH1D("hlt_AC1_a","hlt_AC1_a",nt,0,maxt);
  TH1D *hlt_AC2_a = new TH1D("hlt_AC2_a","hlt_AC2_a",nt,0,maxt);
  TH1D *hlt_AC3_a = new TH1D("hlt_AC3_a","hlt_AC3_a",nt,0,maxt);
  TH1D *hlt_AC4_a = new TH1D("hlt_AC4_a","hlt_AC4_a",nt,0,maxt);
  TH1D *hlt_AC12_a = new TH1D("hlt_AC12_a","hlt_AC12_a",nt,0,maxt);
  TH1D *hlt_SV1_a = new TH1D("hlt_SV1_a","hlt_SV1_a",nt,0,maxt);
  TH1D *hlt_SH1_a = new TH1D("hlt_SH1_a","hlt_SH1_a",nt,0,maxt);
  TH1D *hlt_SH2_a = new TH1D("hlt_SH2_a","hlt_SH2_a",nt,0,maxt);

  TH1D *htt_AC = new TH1D("htt_AC","htt_AC",nt,0,maxt);
  TH1D *htt_AC1 = new TH1D("htt_AC1","htt_AC1",nt,0,maxt);
  TH1D *htt_AC2 = new TH1D("htt_AC2","htt_AC2",nt,0,maxt);
  TH1D *htt_AC3 = new TH1D("htt_AC3","htt_AC3",nt,0,maxt);
  TH1D *htt_AC4 = new TH1D("htt_AC4","htt_AC4",nt,0,maxt);
  TH1D *htt_SV1 = new TH1D("htt_SV1","htt_SV1",nt,0,maxt);
  TH1D *htt_SH1 = new TH1D("htt_SH1","htt_SH1",nt,0,maxt);
  TH1D *htt_SH2 = new TH1D("htt_SH2","htt_SH2",nt,0,maxt);

  TH1D *hnh_AC = new TH1D("hnh_AC","hnh_AC",100,-0.5,99.5);
  TH1D *hnh_AC1 = new TH1D("hnh_AC1","hnh_AC1",100,-0.5,99.5);
  TH1D *hnh_AC2 = new TH1D("hnh_AC2","hnh_AC2",100,-0.5,99.5);
  TH1D *hnh_AC3 = new TH1D("hnh_AC3","hnh_AC3",100,-0.5,99.5);
  TH1D *hnh_AC4 = new TH1D("hnh_AC4","hnh_AC4",100,-0.5,99.5);
  TH1D *hnh_SV1 = new TH1D("hnh_SV1","hnh_SV1",100,-0.5,99.5);
  TH1D *hnh_SH1 = new TH1D("hnh_SH1","hnh_SH1",100,-0.5,99.5);
  TH1D *hnh_SH2 = new TH1D("hnh_SH2","hnh_SH2",100,-0.5,99.5);
  
  TH1D *hwt_AC = new TH1D("hwt_AC","hwt_AC",400,0,100);
  TH1D *hwt_AC1 = new TH1D("hwt_AC1","hwt_AC1",400,0,100);
  TH1D *hwt_AC2 = new TH1D("hwt_AC2","hwt_AC2",400,0,100);
  TH1D *hwt_AC3 = new TH1D("hwt_AC3","hwt_AC3",400,0,100);
  TH1D *hwt_AC4 = new TH1D("hwt_AC4","hwt_AC4",400,0,100);
  TH1D *hwt_SV1 = new TH1D("hwt_SV1","hwt_SV1",400,0,100);
  TH1D *hwt_SH1 = new TH1D("hwt_SH1","hwt_SH1",400,0,100);
  TH1D *hwt_SH2 = new TH1D("hwt_SH2","hwt_SH2",400,0,100);
  
  TH1D *hmt_SH12 = new TH1D("hmt_SH12","hmt_SH12",nt,0,maxt);
  TH1D *hdt_SH12 = new TH1D("hdt_SH12","hdt_SH12",1600,-200,200);
  
  TH2D *hlt_wt_AC = new TH2D("hlt_wt_AC","hlt_wt_AC",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC1 = new TH2D("hlt_wt_AC1","hlt_wt_AC1",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC2 = new TH2D("hlt_wt_AC2","hlt_wt_AC2",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC3 = new TH2D("hlt_wt_AC3","hlt_wt_AC3",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC4 = new TH2D("hlt_wt_AC4","hlt_wt_AC4",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_SV1 = new TH2D("hlt_wt_SV1","hlt_wt_SV1",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_SH1 = new TH2D("hlt_wt_SH1","hlt_wt_SH1",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_SH2 = new TH2D("hlt_wt_SH2","hlt_wt_SH2",400,0,100,nt,0,maxt);

  TH2D *hmt_wt_SH12 = new TH2D("hmt_wt_SH12","hmt_wt_SH12",400,0,100,nt,0,maxt);
  TH2D *hmt_wt_SH12_g = new TH2D("hmt_wt_SH12_g","hmt_wt_SH12_g",400,0,100,nt,0,maxt);
  TH2D *hmt_wt_SH12_a = new TH2D("hmt_wt_SH12_a","hmt_wt_SH12_a",400,0,100,nt,0,maxt);

  TH2D *hlt_wt_AC_g = new TH2D("hlt_wt_AC_g","hlt_wt_AC_g",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC1_g = new TH2D("hlt_wt_AC1_g","hlt_wt_AC1_g",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC2_g = new TH2D("hlt_wt_AC2_g","hlt_wt_AC2_g",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC3_g = new TH2D("hlt_wt_AC3_g","hlt_wt_AC3_g",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC4_g = new TH2D("hlt_wt_AC4_g","hlt_wt_AC4_g",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC12_g = new TH2D("hlt_wt_AC12_g","hlt_wt_AC12_g",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_SV1_g = new TH2D("hlt_wt_SV1_g","hlt_wt_SV1_g",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_SH1_g = new TH2D("hlt_wt_SH1_g","hlt_wt_SH1_g",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_SH2_g = new TH2D("hlt_wt_SH2_g","hlt_wt_SH2_g",400,0,100,nt,0,maxt);

  TH2D *hlt_wt_AC_a = new TH2D("hlt_wt_AC_a","hlt_wt_AC_a",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC1_a = new TH2D("hlt_wt_AC1_a","hlt_wt_AC1_a",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC2_a = new TH2D("hlt_wt_AC2_a","hlt_wt_AC2_a",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC3_a = new TH2D("hlt_wt_AC3_a","hlt_wt_AC3_a",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC4_a = new TH2D("hlt_wt_AC4_a","hlt_wt_AC4_a",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_AC12_a = new TH2D("hlt_wt_AC12_a","hlt_wt_AC12_a",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_SV1_a = new TH2D("hlt_wt_SV1_a","hlt_wt_SV1_a",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_SH1_a = new TH2D("hlt_wt_SH1_a","hlt_wt_SH1_a",400,0,100,nt,0,maxt);
  TH2D *hlt_wt_SH2_a = new TH2D("hlt_wt_SH2_a","hlt_wt_SH2_a",400,0,100,nt,0,maxt);

  //amp = peak, adc = integral
  TH1D *hamp_AC = new TH1D("hamp_AC","hamp_AC",nt,0,maxam);
  TH1D *hadc_AC = new TH1D("hadc_AC","hadc_AC",nt,0,maxad);
  TH1D *hbl_AC = new TH1D("hbl_AC","hbl_AC",nt,0,maxam);
  TH2D *hamp_wt_AC = new TH2D("hamp_wt_AC","hamp_wt_AC",400,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC = new TH2D("hadc_wt_AC","hadc_wt_AC",400,0,100,nt,0,maxad);
  
  TH2D *hamp_wt_AC_a = new TH2D("hamp_wt_AC_a","hamp_wt_AC_a",400,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC_a = new TH2D("hadc_wt_AC_a","hadc_wt_AC_a",400,0,100,nt,0,maxad);

  TH2D *hadc_ev_AC = new TH2D("hadc_ev_AC","hadc_ev_AC",nt,0,100000,nt,0,maxad);
  TH2D *hlt_ev_AC = new TH2D("hlt_ev_AC","hlt_ev_AC",nt,0,100000,250,0,maxt);

  TH1D *hamp_AC1 = new TH1D("hamp_AC1","hamp_AC1",nt,0,maxam);
  TH1D *hadc_AC1 = new TH1D("hadc_AC1","hadc_AC1",nt,0,maxad);
  TH1D *hbl_AC1 = new TH1D("hbl_AC1","hbl_AC1",nt,0,maxam);
  TH2D *hamp_wt_AC1 = new TH2D("hamp_wt_AC1","hamp_wt_AC1",400,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC1 = new TH2D("hadc_wt_AC1","hadc_wt_AC1",400,0,100,nt,0,maxad);
  TH2D *hamp_wt_AC1_a = new TH2D("hamp_wt_AC1_a","hamp_wt_AC1_a",400,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC1_a = new TH2D("hadc_wt_AC1_a","hadc_wt_AC1_a",400,0,100,nt,0,maxad);
  TH2D *hadc_ev_AC1 = new TH2D("hadc_ev_AC1","hadc_ev_AC1",nt,0,100000,nt,0,maxam);
  TH2D *hlt_ev_AC1 = new TH2D("hlt_ev_AC1","hlt_ev_AC1",nt,0,100000,250,0,maxam);

  TH1D *hamp_AC2 = new TH1D("hamp_AC2","hamp_AC2",nt,0,maxam);
  TH1D *hadc_AC2 = new TH1D("hadc_AC2","hadc_AC2",nt,0,maxad);
  TH1D *hbl_AC2 = new TH1D("hbl_AC2","hbl_AC2",nt,0,maxt);
  TH2D *hamp_wt_AC2 = new TH2D("hamp_wt_AC2","hamp_wt_AC2",400,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC2 = new TH2D("hadc_wt_AC2","hadc_wt_AC2",400,0,100,nt,0,maxad);
  TH2D *hamp_wt_AC2_a = new TH2D("hamp_wt_AC2_a","hamp_wt_AC2_a",400,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC2_a = new TH2D("hadc_wt_AC2_a","hadc_wt_AC2_a",400,0,100,nt,0,maxad);
  TH2D *hadc_ev_AC2 = new TH2D("hadc_ev_AC2","hadc_ev_AC2",nt,0,100000,nt,0,maxad);
  TH2D *hlt_ev_AC2 = new TH2D("hlt_ev_AC2","hlt_ev_AC2",nt,0,100000,250,0,maxt);

  TH1D *hamp_AC3 = new TH1D("hamp_AC3","hamp_AC3",nt,0,maxam);
  TH1D *hadc_AC3 = new TH1D("hadc_AC3","hadc_AC3",nt,0,maxad);
  TH1D *hbl_AC3 = new TH1D("hbl_AC3","hbl_AC3",nt,0,maxt);
  TH2D *hamp_wt_AC3 = new TH2D("hamp_wt_AC3","hamp_wt_AC3",400,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC3 = new TH2D("hadc_wt_AC3","hadc_wt_AC3",400,0,100,nt,0,maxad);
  TH2D *hamp_wt_AC3_a = new TH2D("hamp_wt_AC3_a","hamp_wt_AC3_a",400,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC3_a = new TH2D("hadc_wt_AC3_a","hadc_wt_AC3_a",400,0,100,nt,0,maxad);
  TH2D *hadc_ev_AC3 = new TH2D("hadc_ev_AC3","hadc_ev_AC3",nt,0,100000,nt,0,maxam);
  TH2D *hlt_ev_AC3 = new TH2D("hlt_ev_AC3","hlt_ev_AC3",nt,0,100000,250,0,maxad);

  TH1D *hamp_AC4 = new TH1D("hamp_AC4","hamp_AC4",nt,0,maxam);
  TH1D *hadc_AC4 = new TH1D("hadc_AC4","hadc_AC4",nt,0,maxad);
  TH1D *hbl_AC4 = new TH1D("hbl_AC4","hbl_AC4",nt,0,maxam);
  TH2D *hamp_wt_AC4 = new TH2D("hamp_wt_AC4","hamp_wt_AC4",400,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC4 = new TH2D("hadc_wt_AC4","hadc_wt_AC4",400,0,100,nt,0,maxad);
  TH2D *hamp_wt_AC4_a = new TH2D("hamp_wt_AC4_a","hamp_wt_AC4_a",400,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC4_a = new TH2D("hadc_wt_AC4_a","hadc_wt_AC4_a",400,0,100,nt,0,maxad);
  TH2D *hadc_ev_AC4 = new TH2D("hadc_ev_AC4","hadc_ev_AC4",nt,0,100000,nt,0,maxad);
  TH2D *hlt_ev_AC4 = new TH2D("hlt_ev_AC4","hlt_ev_AC4",nt,0,100000,250,0,maxt);

  TH1D *hamp_AC12 = new TH1D("hamp_AC12","hamp_AC12",nt,0,maxam);
  TH1D *hadc_AC12 = new TH1D("hadc_AC12","hadc_AC12",nt,0,maxad);
  TH1D *hbl_AC12 = new TH1D("hbl_AC12","hbl_AC12",nt,0,maxam);
  TH2D *hamp_wt_AC12 = new TH2D("hamp_wt_AC12","hamp_wt_AC12",1200,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC12 = new TH2D("hadc_wt_AC12","hadc_wt_AC12",1200,0,100,nt,0,maxad);
  TH2D *hamp_wt_AC12_a = new TH2D("hamp_wt_AC12_a","hamp_wt_AC12_a",1200,0,100,nt,0,maxam);
  TH2D *hadc_wt_AC12_a = new TH2D("hadc_wt_AC12_a","hadc_wt_AC12_a",1200,0,100,nt,0,maxad);
  TH2D *hadc_ev_AC12 = new TH2D("hadc_ev_AC12","hadc_ev_AC12",nt,0,100000,nt,0,maxad);
  TH2D *hlt_ev_AC12 = new TH2D("hlt_ev_AC12","hlt_ev_AC12",nt,0,100000,250,0,maxt);


  TFile *fout = new TFile(Form("ana/check_actsc/check_actsc_run%s.root", runtag),"RECREATE");
    
  Int_t hev=0;
  Int_t hrun=run;
  const Int_t max_nh = 20;
  const Int_t nt_b = 2;
  
  TTree *htree = new TTree("htree","all hits");
  Int_t hnh_ac;
  Int_t hnh_ac1;
  Int_t hnh_ac2;
  Int_t hnh_ac3;
  Int_t hnh_ac4;
  Int_t hnh_sv;
  Int_t hnh_sh[nt_b];
  
  Double_t hlt_ac[max_nh];
  Double_t htt_ac[max_nh];
  Double_t hlt_ac1[max_nh];
  Double_t htt_ac1[max_nh];
  Double_t hlt_ac2[max_nh];
  Double_t htt_ac2[max_nh];
  Double_t hlt_ac3[max_nh];
  Double_t htt_ac3[max_nh];
  Double_t hlt_ac4[max_nh];
  Double_t htt_ac4[max_nh];
  Double_t hlt_sv[max_nh];
  Double_t htt_sv[max_nh];
  Double_t hlt_sh[nt_b][max_nh];
  Double_t htt_sh[nt_b][max_nh];

  Double_t hamp_ac;
  Double_t hadc_ac;
  Double_t hbl_ac;
  
  Double_t hamp_ac1;
  Double_t hadc_ac1;
  Double_t hbl_ac1;
  
  Double_t hamp_ac2;
  Double_t hadc_ac2;
  Double_t hbl_ac2;
  
  Double_t hamp_ac3;
  Double_t hadc_ac3;
  Double_t hbl_ac3;

  Double_t hamp_ac4;
  Double_t hadc_ac4;
  Double_t hbl_ac4;

  htree->Branch("ievt",&hev, "ev/I");
  htree->Branch("nh_ac",&hnh_ac,"nh_ac/I");
  htree->Branch("nh_ac1",&hnh_ac1,"nh_ac2/I");
  htree->Branch("nh_ac2",&hnh_ac2,"nh_ac2/I");
  htree->Branch("nh_ac2",&hnh_ac3,"nh_ac3/I");
  htree->Branch("nh_ac2",&hnh_ac4,"nh_ac4/I");
  htree->Branch("nh_sv",&hnh_sv,"nh_sv/I");
  htree->Branch("nh_sh",hnh_sh,"nh_sh[2]/I");//[up_down]
  
  htree->Branch("lt_ac",hlt_ac,"lt_ac[20]/D");//[max_hit]
  htree->Branch("lt_ac1",hlt_ac1,"lt_ac1[20]/D");//[max_hit]
  htree->Branch("lt_ac2",hlt_ac2,"lt_ac2[20]/D");//[max_hit]
  htree->Branch("lt_ac3",hlt_ac3,"lt_ac3[20]/D");//[max_hit]
  htree->Branch("lt_ac4",hlt_ac4,"lt_ac4[20]/D");//[max_hit]
  htree->Branch("lt_sv",hlt_sv,"lt_sv[20]/D");
  htree->Branch("lt_sh",hlt_sh,"lt_sh[2][20]/D");//[up_down][max_hit]
  
  htree->Branch("tt_ac",htt_ac,"tt_ac[20]/D");//[max_hit]
  htree->Branch("tt_ac1",htt_ac1,"tt_ac1[20]/D");//[max_hit]
  htree->Branch("tt_ac2",htt_ac2,"tt_ac2[20]/D");//[max_hit]
  htree->Branch("tt_ac3",htt_ac3,"tt_ac3[20]/D");//[max_hit]
  htree->Branch("tt_ac4",htt_ac3,"tt_ac4[20]/D");//[max_hit]
  htree->Branch("tt_sv",htt_sv,"tt_sv[20]/D");
  htree->Branch("tt_sh",htt_sh,"tt_sh[2][20]/D");//[up_down][max_hit]

  htree->Branch("amp_ac",&hamp_ac,"amp/D");
  htree->Branch("adc_ac",&hadc_ac,"adc/D");
  htree->Branch("adc_ac",&hbl_ac,"bl/D");
  
  htree->Branch("amp_ac1",&hamp_ac1,"amp1/D");
  htree->Branch("adc_ac1",&hadc_ac1,"adc1/D");
  htree->Branch("adc_ac1",&hbl_ac1,"bl1/D");

  htree->Branch("amp_ac2",&hamp_ac2,"amp2/D");
  htree->Branch("adc_ac2",&hadc_ac2,"adc2/D");
  htree->Branch("adc_ac2",&hbl_ac2,"bl2/D");
  
  htree->Branch("amp_ac3",&hamp_ac3,"amp3/D");
  htree->Branch("adc_ac3",&hadc_ac3,"adc3/D");
  htree->Branch("adc_ac3",&hbl_ac3,"bl3/D");

  htree->Branch("amp_ac4",&hamp_ac4,"amp4/D");
  htree->Branch("adc_ac4",&hadc_ac4,"adc4/D");
  htree->Branch("adc_ac4",&hbl_ac4,"bl4/D");
  
  Int_t nentries = tree0 -> GetEntriesFast();

  cout << "Event : " << nentries << endl;
  if (maxevent>0) {
    nentries = maxevent;
  }


  for(Int_t event = 0; event < nentries; event++){
    hev = event;
    hadc_ac = -1;
    hamp_ac = -1;
    hadc_ac1 = -1;
    hamp_ac1 = -1;
    hadc_ac2 = -1;
    hamp_ac2 = -1;
    hadc_ac3 = -1;
    hamp_ac3 = -1;
    hadc_ac4 = -1;
    hamp_ac4 = -1;
    if ((event%10000) == 0) {
      cout << "event : " << event << endl;
    }
    Int_t ientry = tree0 -> LoadTree(event);
    tree0 -> GetEntry(ientry);


    Int_t max_pair = 16;
    Double_t gltdcs0[nch][max_pair];
    Double_t gttdcs0[nch][max_pair];
    Int_t mg0[nch];//keep no. of hits
    Int_t mg1[nch];//keep no. of hits

    for (Int_t ich=0;ich<nch;ich++) {
      mg0[ich] = 0;
      mg1[ich] = 0;
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
    
    hnh_ac=0;
    hnh_ac1=0;
    hnh_ac2=0;
    hnh_ac3=0;
    hnh_ac4=0;
    hnh_sv=0;
    for (Int_t itb = 0; itb < nt_b; itb++){
      hnh_sh[itb]=0;//
    }
    for (Int_t ih=0;ih<max_nh;ih++) {
      hlt_ac[ih]=-1;
      hlt_sv[ih]=-1;
      htt_ac[ih]=-1;
      htt_ac1[ih]=-1;
      htt_ac2[ih]=-1;
      htt_ac3[ih]=-1;
      htt_ac4[ih]=-1;
      htt_sv[ih]=-1;
      for (Int_t itb = 0; itb < nt_b; itb++){
	hlt_sh[itb][ih]=-1;
	htt_sh[itb][ih]=-1;
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

    //ch correlations
    //tdc,channel loop0
    //	for (Int_t itdc=0;itdc<ntdc;itdc++) {
	  
    for (Int_t ich=0;ich<nch;ich++) {
      Int_t niele = 0;
	    
      niele = mg0[ich];


      Bool_t iAC = false;
      Bool_t iAC1 = false;
      Bool_t iAC2 = false;
      Bool_t iAC3 = false;
      Bool_t iAC4 = false;
      Bool_t iSV = false;
      Bool_t iSH1 = false;
      Bool_t iSH2 = false;
      
      Int_t it_b = 1;//top=1,bottom=0

      if ((ich==ich_AC)&&(niele>0)) {
	iAC = true;
      } else if ((ich==ich_AC1)&&(niele>0)) {
	iAC1 = true;
      } else if ((ich==ich_AC2)&&(niele>0)) {
	iAC2 = true;
      } else if ((ich==ich_AC3)&&(niele>0)) {
	iAC3 = true;
      } else if ((ich==ich_AC4)&&(niele>0)) {
	iAC4 = true;
      } else if ((ich==ich_SV1)&&(niele>0)) {
	iSV = true;
      } else if ((ich==ich_SH1)&&(niele>0)) {
	iSH1 = true;
	it_b = 0;
      } else if ((ich==ich_SH2)&&(niele>0)) {
	iSH2 = true;
	it_b = 1;
      }
	    
      for (Int_t iele=0;iele<niele;iele++) {
	
	Double_t ilt = gltdcs0[ich][iele];
	Double_t itt = gttdcs0[ich][iele];

	Double_t iwt = ilt-itt;
	
	//fill tree variables
	if (iAC) {
	  if ((0<=iele)&&(iele<max_nh)) {
	    hlt_ac[iele] = ilt;
	    htt_ac[iele] = itt;
	    
	    hnh_ac++;
	    if (iele==0) {
	      hlt_AC->Fill(ilt);
	      hlt_ev_AC->Fill(event,ilt);
	      htt_AC->Fill(itt);
	      hwt_AC->Fill(iwt);
	      hlt_wt_AC->Fill(iwt,ilt);
	    }
	  }
	}
	if (iAC1) {
	  if ((0<=iele)&&(iele<max_nh)) {
	    hlt_ac1[iele] = ilt;
	    htt_ac1[iele] = itt;
	    
	    hnh_ac1++;
	    if (iele==0) {
	      hlt_AC1->Fill(ilt);
	      hlt_ev_AC1->Fill(event,ilt);
	      htt_AC1->Fill(itt);
	      hwt_AC1->Fill(iwt);
	      hlt_wt_AC1->Fill(iwt,ilt);
	    }
	  }
	}
	if (iAC2) {
	  if ((0<=iele)&&(iele<max_nh)) {
	    hlt_ac2[iele] = ilt;
	    htt_ac2[iele] = itt;
	    
	    hnh_ac2++;
	    if (iele==0) {
	      hlt_AC2->Fill(ilt);
	      hlt_ev_AC2->Fill(event,ilt);
	      htt_AC2->Fill(itt);
	      hwt_AC2->Fill(iwt);
	      hlt_wt_AC2->Fill(iwt,ilt);
	    }
	  }
	}
	if (iAC3) {
	  if ((0<=iele)&&(iele<max_nh)) {
	    hlt_ac3[iele] = ilt;
	    htt_ac3[iele] = itt;
	    
	    hnh_ac3++;
	    if (iele==0) {
	      hlt_AC3->Fill(ilt);
	      hlt_ev_AC3->Fill(event,ilt);
	      htt_AC3->Fill(itt);
	      hwt_AC3->Fill(iwt);
	      hlt_wt_AC3->Fill(iwt,ilt);
	    }
	  }
	}
	if (iAC4) {
	  if ((0<=iele)&&(iele<max_nh)) {
	    hlt_ac4[iele] = ilt;
	    htt_ac4[iele] = itt;
	    
	    hnh_ac4++;
	    if (iele==0) {
	      hlt_AC4->Fill(ilt);
	      hlt_ev_AC4->Fill(event,ilt);
	      htt_AC4->Fill(itt);
	      hwt_AC4->Fill(iwt);
	      hlt_wt_AC4->Fill(iwt,ilt);
	    }
	  }
	}
	if (iSV) {
	  if ((0<=iele)&&(iele<max_nh)) {
	    hlt_sv[iele] = ilt;
	    htt_sv[iele] = itt;
	    hnh_sv++;
	    if (iele==0) {
	      hlt_SV1->Fill(ilt);
	      htt_SV1->Fill(itt);
	      hwt_SV1->Fill(iwt);
	      hlt_wt_SV1->Fill(iwt,ilt);
	    }
	  }
	}
	if (iSH1) {
	  if ((0<=iele)&&(iele<max_nh)) {
	    hlt_sh[it_b][iele] = ilt;
	    htt_sh[it_b][iele] = itt;
	    hnh_sh[it_b]++;
	    if (iele==0) {
	      hlt_SH1->Fill(ilt);
	      htt_SH1->Fill(itt);
	      hwt_SH1->Fill(iwt);
	      hlt_wt_SH1->Fill(iwt,ilt);
	    }
	  }
	}
	if (iSH2) {
	  if ((0<=iele)&&(iele<max_nh)) {
	    hlt_sh[it_b][iele] = ilt;
	    htt_sh[it_b][iele] = itt;
	    hnh_sh[it_b]++;
	    if (iele==0) {
	      hlt_SH2->Fill(ilt);
	      htt_SH2->Fill(itt);
	      hwt_SH2->Fill(iwt);
	      hlt_wt_SH2->Fill(iwt,ilt);
	    }
	  }
	}
      }//iele
    }//ich


    
    if ((hnh_sh[0]>0)&&(hnh_sh[1]>0)) {
      Double_t mt = (hlt_sh[0][0]+hlt_sh[1][0])/2.;
      Double_t dt = hlt_sh[0][0]-hlt_sh[1][0];
      Double_t wt = ((hlt_sh[0][0]-htt_sh[0][0])+(hlt_sh[1][0]-htt_sh[1][0]))/2.;
      hmt_SH12->Fill(mt);
      hdt_SH12->Fill(dt);
      hmt_wt_SH12->Fill(wt,mt);
    }

    hnh_AC->Fill(hnh_ac);
    hnh_AC1->Fill(hnh_ac1);
    hnh_AC2->Fill(hnh_ac2);
    hnh_AC3->Fill(hnh_ac3);
    hnh_AC4->Fill(hnh_ac4);
    hnh_SV1->Fill(hnh_sv);
    hnh_SH1->Fill(hnh_sh[0]);
    hnh_SH2->Fill(hnh_sh[1]);

    //waveform analysis

    Double_t n_amp = amp0->at(ich_DRS4_AC).size();
    Double_t n_adc = adc0->at(ich_DRS4_AC).size();
    Double_t n_bl = bl0->at(ich_DRS4_AC).size();
    if (n_amp>0) {
      hamp_ac = amp0->at(ich_DRS4_AC).at(0);
      hamp_AC->Fill(hamp_ac);
      if (hnh_ac>0) {
	Double_t lt = hlt_ac[0];
	Double_t wt = hlt_ac[0]-htt_ac[0];
	hamp_wt_AC->Fill(wt,hamp_ac);
      }
    } else {
      hamp_ac = 0;
    }
    if (n_adc>0) {
      hadc_ac = adc0->at(ich_DRS4_AC).at(0);
      hadc_AC->Fill(hadc_ac);
      hadc_ev_AC->Fill(event,hadc_ac);
      if (hnh_ac>0) {
	Double_t lt = hlt_ac[0];
	Double_t wt = hlt_ac[0]-htt_ac[0];
	hadc_wt_AC->Fill(wt,hadc_ac);
      }
    } else {
      hadc_ac = 0;
    }
    if (n_bl>0) {
      hbl_ac = bl0->at(ich_DRS4_AC).at(0);
      hbl_AC->Fill(hbl_ac);
    } else {
      hbl_ac = 0;
    }

    Double_t n_amp1 = amp0->at(ich_DRS4_AC1).size();
    Double_t n_adc1 = adc0->at(ich_DRS4_AC1).size();
    Double_t n_bl1 = bl0->at(ich_DRS4_AC1).size();
    if (n_amp1>0) {
      hamp_ac1 = amp0->at(ich_DRS4_AC1).at(0);
      hamp_AC1->Fill(hamp_ac1);
      if (hnh_ac1>0) {
	Double_t lt = hlt_ac1[0];
	Double_t wt = hlt_ac1[0]-htt_ac1[0];
	hamp_wt_AC1->Fill(wt,hamp_ac1);
      }
    } else {
      hamp_ac1 = 0;
    }
    if (n_adc1>0) {
      hadc_ac1 = adc0->at(ich_DRS4_AC1).at(0);
      hadc_AC1->Fill(hadc_ac1);
      hadc_ev_AC1->Fill(event,hadc_ac1);
      if (hnh_ac1>0) {
	Double_t lt = hlt_ac1[0];
	Double_t wt = hlt_ac1[0]-htt_ac1[0];
	hadc_wt_AC1->Fill(wt,hadc_ac1);
      }
    } else {
      hadc_ac1 = 0;
    }

    Double_t n_amp2 = amp0->at(ich_DRS4_AC2).size();
    Double_t n_adc2 = adc0->at(ich_DRS4_AC2).size();
    Double_t n_bl2 = bl0->at(ich_DRS4_AC2).size();
    if (n_amp2>0) {
      hamp_ac2 = amp0->at(ich_DRS4_AC2).at(0);
      hamp_AC2->Fill(hamp_ac2);
      if (hnh_ac2>0) {
	Double_t lt = hlt_ac2[0];
	Double_t wt = hlt_ac2[0]-htt_ac2[0];
	hamp_wt_AC2->Fill(wt,hamp_ac2);
      }
    } else {
      hamp_ac2 = 0;
    }
    if (n_adc2>0) {
      hadc_ac2 = adc0->at(ich_DRS4_AC2).at(0);
      hadc_AC2->Fill(hadc_ac2);
      hadc_ev_AC2->Fill(event,hadc_ac2);
      if (hnh_ac2>0) {
	Double_t lt = hlt_ac2[0];
	Double_t wt = hlt_ac2[0]-htt_ac2[0];
	hadc_wt_AC2->Fill(wt,hadc_ac2);
      }
    } else {
      hadc_ac2 = 0;
    }

    Double_t n_amp3 = amp0->at(ich_DRS4_AC3).size();
    Double_t n_adc3 = adc0->at(ich_DRS4_AC3).size();
    Double_t n_bl3 = bl0->at(ich_DRS4_AC3).size();
    if (n_amp3>0) {
      hamp_ac3 = amp0->at(ich_DRS4_AC3).at(0);
      hamp_AC3->Fill(hamp_ac3);
      if (hnh_ac3>0) {
	Double_t lt = hlt_ac3[0];
	Double_t wt = hlt_ac3[0]-htt_ac3[0];
	hamp_wt_AC3->Fill(wt,hamp_ac3);
      }
    } else {
      hamp_ac3 = 0;
    }
    if (n_adc3>0) {
      hadc_ac3 = adc0->at(ich_DRS4_AC3).at(0);
      hadc_AC3->Fill(hadc_ac3);
      hadc_ev_AC3->Fill(event,hadc_ac3);
      if (hnh_ac3>0) {
	Double_t lt = hlt_ac3[0];
	Double_t wt = hlt_ac3[0]-htt_ac3[0];
	hadc_wt_AC3->Fill(wt,hadc_ac3);
      }
    } else {
      hadc_ac3 = 0;
    }

    Double_t n_amp4 = amp0->at(ich_DRS4_AC4).size();
    Double_t n_adc4 = adc0->at(ich_DRS4_AC4).size();
    Double_t n_bl4 = bl0->at(ich_DRS4_AC4).size();
    if (n_amp4>0) {
      hamp_ac4 = amp0->at(ich_DRS4_AC4).at(0);
      hamp_AC4->Fill(hamp_ac4);
      if (hnh_ac4>0) {
	Double_t lt = hlt_ac4[0];
	Double_t wt = hlt_ac4[0]-htt_ac4[0];
	hamp_wt_AC4->Fill(wt,hamp_ac4);
      }
    } else {
      hamp_ac4 = 0;
    }
    if (n_adc4>0) {
      hadc_ac4 = adc0->at(ich_DRS4_AC4).at(0);
      hadc_AC4->Fill(hadc_ac4);
      hadc_ev_AC4->Fill(event,hadc_ac4);
      if (hnh_ac4>0) {
	Double_t lt = hlt_ac4[0];
	Double_t wt = hlt_ac4[0]-htt_ac4[0];
	hadc_wt_AC4->Fill(wt,hadc_ac4);
      }
    } else {
      hadc_ac4 = 0;
    }

    Double_t amp_ac12 = hamp_ac1+hamp_ac2+hamp_ac3+hamp_ac4;
    Double_t adc_ac12 = hadc_ac1+hadc_ac2+hadc_ac3+hadc_ac4;
    Double_t wt1 = 0;
    if (hnh_ac1>0) {
      wt1=hlt_ac1[0]-htt_ac1[0];
    }
    Double_t wt2 = 0;
    if (hnh_ac2>0) {
      wt2=hlt_ac2[0]-htt_ac2[0];
    }
    Double_t wt3 = 0;
    if (hnh_ac3>0) {
      wt3=hlt_ac3[0]-htt_ac3[0];
    }
    Double_t wt4 = 0;
    if (hnh_ac4>0) {
      wt4=hlt_ac4[0]-htt_ac4[0];
    }
    Double_t lt_ac12 = 0;
    Int_t nlt_ac12 = 0;
    if (hnh_ac1>0) {
      nlt_ac12++;
      lt_ac12 += hlt_ac1[0];
    }
    if (hnh_ac2>0) {
      nlt_ac12++;
      lt_ac12 += hlt_ac2[0];
    }
    if (hnh_ac3>0) {
      nlt_ac12++;
      lt_ac12 += hlt_ac3[0];
    }
    if (hnh_ac4>0) {
      nlt_ac12++;
      lt_ac12 += hlt_ac4[0];
    }

    if (nlt_ac12>0) {
      lt_ac12 /= (double)nlt_ac12;
    }

    Double_t wt_ac12=wt1+wt2+wt3+wt4;
    if (amp_ac12>0) {
      hamp_AC12->Fill(amp_ac12);
      if (wt_ac12>0) {
	hamp_wt_AC12->Fill(wt_ac12,amp_ac12);
      }	
    }
    if (adc_ac12>0) {
      hadc_AC12->Fill(adc_ac12);
      if (wt_ac12>0) {
	hadc_wt_AC12->Fill(wt_ac12,adc_ac12);
      }	
    }
    
    if (n_bl>0) {
      hbl_ac = bl0->at(ich_DRS4_AC).at(0);
      hbl_AC->Fill(hbl_ac);
    } else {
      hbl_ac = 0;
    }

    Bool_t good_AC = false;
    if (hnh_ac>0) {
      Double_t lt = hlt_ac[0];
      if ((t_min_AC<=lt)&&(lt<=t_max_AC)) {
	good_AC = true;
      }
    }
    Bool_t good_AC1 = false;
    if (hnh_ac1>0) {
      Double_t lt = hlt_ac1[0];
      if ((t_min_AC1<=lt)&&(lt<=t_max_AC1)) {
	good_AC1 = true;
      }
    }
    Bool_t good_AC2 = false;
    if (hnh_ac2>0) {
      Double_t lt = hlt_ac2[0];
      if ((t_min_AC2<=lt)&&(lt<=t_max_AC2)) {
	good_AC2 = true;
      }
    }
    Bool_t good_AC3 = false;
    if (hnh_ac3>0) {
      Double_t lt = hlt_ac3[0];
      if ((t_min_AC3<=lt)&&(lt<=t_max_AC3)) {
	good_AC3 = true;
      }
    }
    Bool_t good_AC4 = false;
    if (hnh_ac4>0) {
      Double_t lt = hlt_ac4[0];
      if ((t_min_AC4<=lt)&&(lt<=t_max_AC4)) {
	good_AC4 = true;
      }
    }

    Bool_t good_AC12 = false;
    if (good_AC1||good_AC2) {
      good_AC12 = true;
    }

    Bool_t good_SV1 = false;
    if (hnh_sv>0) {
      Double_t lt = hlt_sv[0];
      if ((t_min_SV1<=lt)&&(lt<=t_max_SV1)) {
	good_SV1 = true;
      }
    }
    Bool_t good_SH1 = false;
    if (hnh_sh[0]>0) {
      Double_t lt = hlt_sh[0][0];
      if ((t_min_SH1<=lt)&&(lt<=t_max_SH1)) {
	good_SH1 = true;
      }
    }
    Bool_t good_SH2 = false;
    if (hnh_sh[1]>0) {
      Double_t lt = hlt_sh[1][0];
      if ((t_min_SH2<=lt)&&(lt<=t_max_SH2)) {
	good_SH2 = true;
      }
    }

    Bool_t good_trig = false;
    Bool_t acc_trig = false;
    Bool_t good_SH12 = false;
    if (good_SV1&&good_SH1&&good_SH2) {//good trigger
      good_trig = true;
      if (good_AC) {
	hlt_AC_g->Fill(hlt_ac[0]);
	hlt_wt_AC_g->Fill(hlt_ac[0]-htt_ac[0],hlt_ac[0]);
      }
      if (good_AC1) {
	hlt_AC1_g->Fill(hlt_ac1[0]);
	hlt_wt_AC1_g->Fill(hlt_ac1[0]-htt_ac1[0],hlt_ac1[0]);
      }
      if (good_AC2) {
	hlt_AC2_g->Fill(hlt_ac2[0]);
	hlt_wt_AC2_g->Fill(hlt_ac2[0]-htt_ac2[0],hlt_ac2[0]);
      }
      if (good_AC3) {
	hlt_AC3_g->Fill(hlt_ac3[0]);
	hlt_wt_AC3_g->Fill(hlt_ac3[0]-htt_ac3[0],hlt_ac3[0]);
      }
      if (good_AC4) {
	hlt_AC4_g->Fill(hlt_ac4[0]);
	hlt_wt_AC4_g->Fill(hlt_ac4[0]-htt_ac4[0],hlt_ac4[0]);
      }
      if (good_SV1) {
	hlt_SV1_g->Fill(hlt_sv[0]);
	hlt_wt_SV1_g->Fill(hlt_sv[0]-htt_sv[0],hlt_sv[0]);
      }
      if (good_SH1) {
	hlt_SH1_g->Fill(hlt_sh[0][0]);
	hlt_wt_SH1_g->Fill(hlt_sh[0][0]-htt_sh[0][0],hlt_sh[0][0]);
      }
      if (good_SH2) {
	hlt_SH2_g->Fill(hlt_sh[1][0]);
	hlt_wt_SH2_g->Fill(hlt_sh[1][0]-htt_sh[1][0],hlt_sh[1][0]);
      }
      if (good_SH1&&good_SH2) {
	Double_t mt = (hlt_sh[0][0]+hlt_sh[1][0])/2.;
	Double_t wt = ((hlt_sh[0][0]-htt_sh[0][0])+(hlt_sh[1][0]-htt_sh[1][0]))/2.;      
	hmt_wt_SH12_g->Fill(wt,mt);
	Double_t dt = hlt_sh[0][0]-hlt_sh[1][0];
	if (fabs(dt-(-1.68471))<6.83404e-01*2.) {
	  good_SH12 = true;
	}
      }
      //      if (good_SH1&&good_SH2) {//accepted
      if (good_SH12) {//accepted
	acc_trig = true;

	if (good_AC) {
	  hlt_AC_a->Fill(hlt_ac[0]);
	  hlt_wt_AC_a->Fill(hlt_ac[0]-htt_ac[0],hlt_ac[0]);
	  if (hadc_ac>=0) {
	    hadc_wt_AC_a->Fill(hlt_ac[0]-htt_ac[0],hadc_ac);
	    hamp_wt_AC_a->Fill(hlt_ac[0]-htt_ac[0],hamp_ac);
	  }
	}
	if (good_AC2) {
	  hlt_AC2_a->Fill(hlt_ac2[0]);
	  hlt_wt_AC2_a->Fill(hlt_ac2[0]-htt_ac2[0],hlt_ac2[0]);
	  if (hadc_ac2>=0) {
	    hadc_wt_AC2_a->Fill(hlt_ac2[0]-htt_ac2[0],hadc_ac2);
	    hamp_wt_AC2_a->Fill(hlt_ac2[0]-htt_ac2[0],hamp_ac2);
	  }
	}
	if (good_AC3) {
	  hlt_AC3_a->Fill(hlt_ac3[0]);
	  hlt_wt_AC3_a->Fill(hlt_ac3[0]-htt_ac3[0],hlt_ac3[0]);
	  if (hadc_ac3>=0) {
	    hadc_wt_AC3_a->Fill(hlt_ac3[0]-htt_ac3[0],hadc_ac3);
	    hamp_wt_AC3_a->Fill(hlt_ac3[0]-htt_ac3[0],hamp_ac3);
	  }
	}
	if (good_AC4) {
	  hlt_AC4_a->Fill(hlt_ac4[0]);
	  hlt_wt_AC4_a->Fill(hlt_ac4[0]-htt_ac4[0],hlt_ac4[0]);
	  if (hadc_ac4>=0) {
	    hadc_wt_AC4_a->Fill(hlt_ac4[0]-htt_ac4[0],hadc_ac4);
	    hamp_wt_AC4_a->Fill(hlt_ac4[0]-htt_ac4[0],hamp_ac4);
	  }
	}

	if (good_SV1) {
	  hlt_SV1_a->Fill(hlt_sv[0]);
	  hlt_wt_SV1_a->Fill(hlt_sv[0]-htt_sv[0],hlt_sv[0]);
	}
	if (good_SH1) {
	  hlt_SH1_a->Fill(hlt_sh[0][0]);
	  hlt_wt_SH1_a->Fill(hlt_sh[0][0]-htt_sh[0][0],hlt_sh[0][0]);
	}
	if (good_SH2) {
	  hlt_SH2_a->Fill(hlt_sh[1][0]);
	  hlt_wt_SH2_a->Fill(hlt_sh[1][0]-htt_sh[1][0],hlt_sh[1][0]);
	}
	if (good_SH1&&good_SH2) {
	  Double_t mt = (hlt_sh[0][0]+hlt_sh[1][0])/2.;
	  Double_t wt = ((hlt_sh[0][0]-htt_sh[0][0])+(hlt_sh[1][0]-htt_sh[1][0]))/2.;
	  hmt_wt_SH12_a->Fill(wt,mt);
	}

      }
      
    }
    
    htree->Fill();

#endif		  

    
  }

#ifdef HIST
  hlt_AC->SetAxisRange(0,4000);
  hlt_AC->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC_run%s.gif",runtag));
  c1->Update();
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
  hlt_AC4->SetAxisRange(0,4000);
  hlt_AC4->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_AC4_run%s.gif",runtag));
  c1->Update();
  hlt_SV1->SetAxisRange(0,4000);
  hlt_SV1->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_SV1_run%s.gif",runtag));
  c1->Update();
  hlt_SH1->SetAxisRange(0,4000);
  hlt_SH1->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_SH1_run%s.gif",runtag));
  c1->Update();
  hlt_SH2->SetAxisRange(0,4000);
  hlt_SH2->Draw();
  c1->SaveAs(Form("ana/monitor/hlt_SH2_run%s.gif",runtag));
  c1->Update();

  htt_AC->SetAxisRange(0,4000);
  htt_AC->Draw();
  c1->SaveAs(Form("ana/monitor/htt_AC_run%s.gif",runtag));
  c1->Update();
  htt_AC1->SetAxisRange(0,4000);
  htt_AC1->Draw();
  c1->SaveAs(Form("ana/monitor/htt_AC1_run%s.gif",runtag));
  c1->Update();
  htt_AC2->SetAxisRange(0,4000);
  htt_AC2->Draw();
  c1->SaveAs(Form("ana/monitor/htt_AC2_run%s.gif",runtag));
  c1->Update();
  htt_AC3->SetAxisRange(0,4000);
  htt_AC3->Draw();
  c1->SaveAs(Form("ana/monitor/htt_AC3_run%s.gif",runtag));
  c1->Update();
  htt_AC4->SetAxisRange(0,4000);
  htt_AC4->Draw();
  c1->SaveAs(Form("ana/monitor/htt_AC4_run%s.gif",runtag));
  c1->Update();
  htt_SV1->SetAxisRange(0,4000);
  htt_SV1->Draw();
  c1->SaveAs(Form("ana/monitor/htt_SV1_run%s.gif",runtag));
  c1->Update();
  htt_SH1->SetAxisRange(0,4000);
  htt_SH1->Draw();
  c1->SaveAs(Form("ana/monitor/htt_SH1_run%s.gif",runtag));
  c1->Update();
  htt_SH2->SetAxisRange(0,4000);
  htt_SH2->Draw();
  c1->SaveAs(Form("ana/monitor/htt_SH2_run%s.gif",runtag));
  c1->Update();

  hnh_AC->Draw();
  c1->SaveAs(Form("ana/monitor/hnh_AC_run%s.gif",runtag));
  c1->Update();
  hnh_SV1->Draw();
  c1->SaveAs(Form("ana/monitor/hnh_SV1_run%s.gif",runtag));
  c1->Update();
  hnh_SH1->Draw();
  c1->SaveAs(Form("ana/monitor/hnh_SH1_run%s.gif",runtag));
  c1->Update();
  hnh_SH2->Draw();
  c1->SaveAs(Form("ana/monitor/hnh_SH2_run%s.gif",runtag));
  c1->Update();

  hwt_AC->Draw();
  c1->SaveAs(Form("ana/monitor/hwt_AC_run%s.gif",runtag));
  c1->Update();
  hwt_SV1->Draw();
  c1->SaveAs(Form("ana/monitor/hwt_SV1_run%s.gif",runtag));
  c1->Update();
  hwt_SH1->Draw();
  c1->SaveAs(Form("ana/monitor/hwt_SH1_run%s.gif",runtag));
  c1->Update();
  hwt_SH2->Draw();
  c1->SaveAs(Form("ana/monitor/hwt_SH2_run%s.gif",runtag));
  c1->Update();

  hlt_wt_AC->SetAxisRange(0,4000,"Y");
  hlt_wt_AC->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hlt_wt_AC_run%s.gif",runtag));
  c1->Update();
  hlt_wt_SV1->SetAxisRange(0,4000,"Y");
  hlt_wt_SV1->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hlt_wt_SV1_run%s.gif",runtag));
  c1->Update();
  hlt_wt_SH1->SetAxisRange(0,4000,"Y");
  hlt_wt_SH1->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hlt_wt_SH1_run%s.gif",runtag));
  c1->Update();
  hlt_wt_SH2->SetAxisRange(0,4000,"Y");
  hlt_wt_SH2->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hlt_wt_SH2_run%s.gif",runtag));
  c1->Update();

  hmt_SH12->SetAxisRange(0,4000);
  hmt_SH12->Draw();
  c1->SaveAs(Form("ana/monitor/hmt_SH12_run%s.gif",runtag));
  c1->Update();

  hdt_SH12->SetAxisRange(-50,50);
  hdt_SH12->Draw();
  c1->SaveAs(Form("ana/monitor/hdt_SH12_run%s.gif",runtag));
  c1->Update();

  hmt_wt_SH12->SetAxisRange(0,4000,"Y");
  hmt_wt_SH12->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hmt_wt_SH12_run%s.gif",runtag));
  c1->Update();

  hadc_AC->SetAxisRange(0,300);
  hadc_AC->Draw();
  c1->SaveAs(Form("ana/monitor/hadc_AC_run%s.gif",runtag));
  c1->Update();

  hamp_AC->SetAxisRange(0,600);
  hamp_AC->Draw();
  c1->SaveAs(Form("ana/monitor/hamp_AC_run%s.gif",runtag));
  c1->Update();

  hadc_wt_AC->SetAxisRange(0,40,"X");
  hadc_wt_AC->SetAxisRange(0,300,"Y");
  hadc_wt_AC->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hadc_wt_AC_run%s.gif",runtag));
  c1->Update();

  hamp_wt_AC->SetAxisRange(0,40,"X");
  hamp_wt_AC->SetAxisRange(0,600,"Y");
  hamp_wt_AC->Draw("colz");
  c1->SaveAs(Form("ana/monitor/hamp_wt_AC_run%s.gif",runtag));
  c1->Update();

#endif

    
#ifdef HIST
  hlt_AC->Write();
  hlt_AC1->Write();
  hlt_AC2->Write();
  hlt_AC3->Write();
  hlt_AC4->Write();
  hlt_AC12->Write();
  hlt_ev_AC->Write();
  hlt_ev_AC1->Write();
  hlt_ev_AC2->Write();
  hlt_ev_AC3->Write();
  hlt_ev_AC4->Write();
  hlt_SV1->Write();
  hlt_SH1->Write();
  hlt_SH2->Write();

  hlt_AC_g->Write();
  hlt_AC1_g->Write();
  hlt_AC2_g->Write();
  hlt_AC3_g->Write();
  hlt_AC4_g->Write();
  hlt_SV1_g->Write();
  hlt_SH1_g->Write();
  hlt_SH2_g->Write();
  
  hlt_AC_a->Write();
  hlt_AC1_a->Write();
  hlt_AC2_a->Write();
  hlt_AC3_a->Write();
  hlt_AC4_a->Write();
  hlt_SV1_a->Write();
  hlt_SH1_a->Write();
  hlt_SH2_a->Write();
  
  htt_AC->Write();
  htt_AC1->Write();
  htt_AC2->Write();
  htt_AC3->Write();
  htt_AC4->Write();
  htt_SV1->Write();
  htt_SH1->Write();
  htt_SH2->Write();
  hnh_AC->Write();
  hnh_AC1->Write();
  hnh_AC2->Write();
  hnh_AC3->Write();
  hnh_AC4->Write();
  hnh_SV1->Write();
  hnh_SH1->Write();
  hnh_SH2->Write();
  hwt_AC->Write();
  hwt_AC1->Write();
  hwt_AC2->Write();
  hwt_AC3->Write();
  hwt_AC4->Write();
  hwt_SV1->Write();
  hwt_SH1->Write();
  hwt_SH2->Write();
  hlt_wt_AC->Write();
  hlt_wt_AC1->Write();
  hlt_wt_AC2->Write();
  hlt_wt_AC3->Write();
  hlt_wt_AC4->Write();
  hlt_wt_SV1->Write();
  hlt_wt_SH1->Write();
  hlt_wt_SH2->Write();
  hlt_wt_AC_g->Write();
  hlt_wt_AC1_g->Write();
  hlt_wt_AC2_g->Write();
  hlt_wt_AC3_g->Write();
  hlt_wt_AC4_g->Write();
  hlt_wt_AC12_g->Write();
  hlt_wt_SV1_g->Write();
  hlt_wt_SH1_g->Write();
  hlt_wt_SH2_g->Write();
  hlt_wt_AC_a->Write();
  hlt_wt_AC1_a->Write();
  hlt_wt_AC2_a->Write();
  hlt_wt_AC3_a->Write();
  hlt_wt_AC4_a->Write();
  hlt_wt_AC12_a->Write();
  hlt_wt_SV1_a->Write();
  hlt_wt_SH1_a->Write();
  hlt_wt_SH2_a->Write();
  hmt_SH12->Write();
  hdt_SH12->Write();
  hmt_wt_SH12->Write();
  hmt_wt_SH12_a->Write();
  hmt_wt_SH12_g->Write();
  hadc_AC->Write();
  hadc_AC1->Write();
  hadc_AC2->Write();
  hadc_AC3->Write();
  hadc_AC4->Write();
  hadc_AC12->Write();

  hbl_AC->Write();
  hbl_AC1->Write();
  hbl_AC2->Write();
  hbl_AC3->Write();
  hbl_AC4->Write();

  hadc_ev_AC->Write();
  hadc_ev_AC1->Write();
  hadc_ev_AC2->Write();
  hadc_ev_AC3->Write();
  hadc_ev_AC4->Write();
  hamp_AC->Write();
  hamp_AC1->Write();
  hamp_AC2->Write();
  hamp_AC3->Write();
  hamp_AC4->Write();
  hamp_AC12->Write();
  hadc_wt_AC->Write();
  hadc_wt_AC1->Write();
  hadc_wt_AC2->Write();
  hadc_wt_AC3->Write();
  hadc_wt_AC4->Write();
  hadc_wt_AC12->Write();
  hamp_wt_AC->Write();
  hamp_wt_AC1->Write();
  hamp_wt_AC2->Write();
  hamp_wt_AC3->Write();
  hamp_wt_AC4->Write();
  hamp_wt_AC12->Write();
  hadc_wt_AC_a->Write();
  hadc_wt_AC1_a->Write();
  hadc_wt_AC2_a->Write();
  hadc_wt_AC3_a->Write();
  hadc_wt_AC4_a->Write();
  hadc_wt_AC12_a->Write();
  hamp_wt_AC_a->Write();
  hamp_wt_AC1_a->Write();
  hamp_wt_AC2_a->Write();
  hamp_wt_AC3_a->Write();
  hamp_wt_AC4_a->Write();
  hamp_wt_AC12_a->Write();
#endif

  htree->Write();
  fout->Close();

}

check_actsc::check_actsc(Int_t run, Int_t subrun, TString basedir) {
  base = basedir;

  cout << "base = " << base << endl;
  fileopen0(run,subrun);

  check(run, subrun);

    //    return 0;
    
}

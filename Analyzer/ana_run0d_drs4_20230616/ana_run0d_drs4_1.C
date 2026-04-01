#include <iostream>
#include <fstream>

#include <cstdlib>
#include <math.h>

#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TRint.h>
#include <TTree.h>
#include <TRandom.h>
#include <TLinearFitter.h>
#include <TMath.h>
#include <TArc.h>
#include <TGraphErrors.h>
using namespace std;


static const Double_t PI = 3.1415927;


int main(int argc,char *argv[]) {

  cout << "argc = " << argc << endl;
  if (argc<4) {
    cerr << "Usage ana_run0d_1 run infile outfile listflag" << endl;
    exit(-1);
  }
  cout << "infile = " << argv[2] << endl;
  cout << "outfile = " << argv[3] << endl;

  Int_t run = atoi(argv[1]);

  TChain *htree = new TChain("htree");


  if (argc>=5) {
    ifstream filelist(argv[2]);
    char line[256];
    while (!filelist.eof()) {
      filelist.getline(line,sizeof(line));
      if (strcmp(line, "") == 0) continue;
      cout << "line = " << line << endl;
      TFile a(line);
      if (!a.IsOpen()) {
	std::cerr << "failed in open :" << line << std::endl;
	exit(-1);
      }
      if (gROOT->FindObject(htree->GetName()) == 0) {
	std::cerr << htree->GetName() << " is not found in :" << line << std::endl;
	exit(-1);
      }
      cout << "add to tree" << line << endl;
      htree->AddFile(line);
    }
  } else {
    htree->AddFile(argv[2]);
  }

  TROOT theROOT("htree","test");

  Int_t hev=0;
  Int_t hrun=0;
  const Int_t max_nh = 20;
  const Int_t nstrip_rpc = 8;
  const Int_t nstrip_tsc1 = 24;
  const Int_t nstrip_tsc2 = 8;
  //AC
  const Int_t n_ac = 3;
  //RPCR
  const Int_t n_rpcr = 2;
  const Int_t nstrip_rpcr = 4;

  const Int_t nt_b = 2;//bottom/top
  const Int_t nch_trig1 = 7;

  const Double_t clight = 299.792458;//mm/ns


  Int_t hnh_tsc1[nstrip_tsc1][nt_b];
  Int_t hnh_tsc2[nstrip_tsc2][nt_b];
  Int_t hnh_ac[n_ac];
  Int_t hnw_ac[n_ac];//waveform
  Int_t hnh_rpcr[n_rpcr][nstrip_rpcr];
  Int_t hnh_trig1[nch_trig1];

    
  Double_t hlt_tsc1[nstrip_tsc1][nt_b][max_nh];
  Double_t htt_tsc1[nstrip_tsc1][nt_b][max_nh];
  Double_t hlt_tsc2[nstrip_tsc2][nt_b][max_nh];
  Double_t htt_tsc2[nstrip_tsc2][nt_b][max_nh];
  Double_t hlt_ac[n_ac][max_nh];
  Double_t htt_ac[n_ac][max_nh];
  Double_t hlt_rpcr[n_rpcr][nstrip_rpcr][max_nh];
  Double_t htt_rpcr[n_rpcr][nstrip_rpcr][max_nh];

  Double_t hlt_trig1[nch_trig1][max_nh];
  Double_t htt_trig1[nch_trig1][max_nh];

  Double_t hamp_ac[n_ac][max_nh];
  Double_t hadc_ac[n_ac][max_nh];
  Double_t hbl_ac[n_ac][max_nh];

  htree->SetBranchAddress("ievt",&hev);
  htree->SetBranchAddress("irun",&hrun);//2021/7/1
  htree->SetBranchAddress("nh_tsc1",hnh_tsc1);//[strip][up_down]
  htree->SetBranchAddress("nh_tsc2",hnh_tsc2);//[strip][up_down]
  htree->SetBranchAddress("nh_ac",hnh_ac);//[det]
  htree->SetBranchAddress("nw_ac",hnw_ac);//waveform
  htree->SetBranchAddress("nh_rpcr",hnh_rpcr);//[det][strip]
  htree->SetBranchAddress("nh_trig1",hnh_trig1);//[ch]

  htree->SetBranchAddress("lt_tsc1",hlt_tsc1);
  htree->SetBranchAddress("lt_tsc2",hlt_tsc2);
  htree->SetBranchAddress("lt_ac",hlt_ac);
  htree->SetBranchAddress("lt_rpcr",hlt_rpcr);
  htree->SetBranchAddress("lt_trig1",hlt_trig1);//trig1(MRPC/TSC/AC/RPCREF/L1/L1SD)

  htree->SetBranchAddress("tt_tsc1",htt_tsc1);
  htree->SetBranchAddress("tt_tsc2",htt_tsc2);
  htree->SetBranchAddress("tt_ac",htt_ac);
  htree->SetBranchAddress("tt_rpcr",htt_rpcr);
  htree->SetBranchAddress("tt_trig1",htt_trig1);//trig1(L1/ESC/MRPC/TSC/HSC)

  htree->SetBranchAddress("amp_ac",hamp_ac);
  htree->SetBranchAddress("adc_ac",hadc_ac);
  htree->SetBranchAddress("bl_ac",hbl_ac);

  //RPC
  const Double_t t_min_RPC = 0;
  const Double_t t_max_RPC = 500;
  const Double_t dt_min_RPC = -20;
  const Double_t dt_max_RPC = 20;
  
  const Double_t t_min_TSC = 0;
  const Double_t t_max_TSC = 500;

  const Double_t t_min_RPCR = 0;
  const Double_t t_max_RPCR = 500;

  const Double_t t_min_AC = 0;
  const Double_t t_max_AC = 500;

  const Double_t dt_min_TSC = -10;
  const Double_t dt_max_TSC = 10;
  const Double_t dt_cut_TSC = 1;//+-2ns

  TH2F *hlt_s_t1 = new TH2F("hlt_s_t1","hlt_s_t1",48,-0.5,47.5,1000,0,500);
  TH2F *hlt_s_t2 = new TH2F("hlt_s_t2","hlt_s_t2",16,-0.5,15.5,1000,0,500);
  TH2F *hlt_s_ref1 = new TH2F("hlt_s_ref1","hlt_s_ref1",4,-0.5,3.5,1000,0,500);
  TH2F *hlt_s_ref2 = new TH2F("hlt_s_ref2","hlt_s_ref2",4,-0.5,3.5,1000,0,500);
  TH2F *hlt_s_a = new TH2F("hlt_s_a","hlt_s_a",3,-0.5,2.5,1000,0,500);

  TH1F *htl1 = new TH1F("htl1","htl1",25000,0,5000);
  TH1F *htl1d = new TH1F("htl1d","htl1d",25000,0,5000);
  TH1F *htrpc = new TH1F("htrpc","htrpc",25000,0,5000);
  TH1F *httsc = new TH1F("httsc","httsc",25000,0,5000);
  TH1F *htac = new TH1F("htac","htac",25000,0,5000);
  TH1F *htref = new TH1F("htref","htref",25000,0,5000);

  TH2F *hmt_s_t1 = new TH2F("hmt_s_t1","hmt_s_t1",24,-0.5,23.5,5000,0,500);
  TH2F *hmt_s_t2 = new TH2F("hmt_s_t2","hmt_s_t2",8,-0.5,7.5,5000,0,500);
  TH2F *hmt_s_ac = new TH2F("hmt_s_ac","hmt_s_ac",3,-0.5,2.5,5000,0,500);
  TH2F *hmt_s_ren = new TH2F("hmt_s_ren","hmt_s_ren",2,-0.5,1.5,5000,0,500);
  TH2F *hmt_s_ref = new TH2F("hmt_s_ref","hmt_s_ref",2,-0.5,1.5,5000,0,500);

  TH1D *hamp_a1 = new TH1D("hamp_a1","hamp_a1",4000,0,1000);
  TH1D *hadc_a1 = new TH1D("hadc_a1","hadc_a1",4000,0,1000);
  TH1D *hbl_a1 = new TH1D("hbl_a1","hbl_a1",4000,0,1000);
  TH2D *hamp_wt_a1 = new TH2D("hamp_wt_a1","hamp_wt_a1",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a1 = new TH2D("hadc_wt_a1","hadc_wt_a1",400,0,100,4000,0,1000);
  TH2D *hamp_wt_a1_t2 = new TH2D("hamp_wt_a1_t2","hamp_wt_a1_t2",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a1_t2 = new TH2D("hadc_wt_a1_t2","hadc_wt_a1_t2",400,0,100,4000,0,1000);

  TH2D *hamp_wt_a1_all = new TH2D("hamp_wt_a1_all","hamp_wt_a1_all",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a1_all = new TH2D("hadc_wt_a1_all","hadc_wt_a1_all",400,0,100,4000,0,1000);
  TH2D *hamp_wt_a1_t2_all = new TH2D("hamp_wt_a1_t2_all","hamp_wt_a1_t2_all",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a1_t2_all = new TH2D("hadc_wt_a1_t2_all","hadc_wt_a1_t2_all",400,0,100,4000,0,1000);
  
  TH1D *hamp_a2 = new TH1D("hamp_a2","hamp_a2",4000,0,1000);
  TH1D *hadc_a2 = new TH1D("hadc_a2","hadc_a2",4000,0,1000);
  TH1D *hbl_a2 = new TH1D("hbl_a2","hbl_a2",4000,0,1000);
  TH2D *hamp_wt_a2 = new TH2D("hamp_wt_a2","hamp_wt_a2",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a2 = new TH2D("hadc_wt_a2","hadc_wt_a2",400,0,100,4000,0,1000);

  TH2D *hamp_wt_a2_t2 = new TH2D("hamp_wt_a2_t2","hamp_wt_a2_t2",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a2_t2 = new TH2D("hadc_wt_a2_t2","hadc_wt_a2_t2",400,0,100,4000,0,1000);

  TH2D *hamp_wt_a2_all = new TH2D("hamp_wt_a2_all","hamp_wt_a2_all",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a2_all = new TH2D("hadc_wt_a2_all","hadc_wt_a2_all",400,0,100,4000,0,1000);
  TH2D *hamp_wt_a2_t2_all = new TH2D("hamp_wt_a2_t2_all","hamp_wt_a2_t2_all",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a2_t2_all = new TH2D("hadc_wt_a2_t2_all","hadc_wt_a2_t2_all",400,0,100,4000,0,1000);
  
  TH1D *hamp_a3 = new TH1D("hamp_a3","hamp_a3",4000,0,1000);
  TH1D *hadc_a3 = new TH1D("hadc_a3","hadc_a3",4000,0,1000);
  TH1D *hbl_a3 = new TH1D("hbl_a3","hbl_a3",4000,0,1000);
  TH2D *hamp_wt_a3 = new TH2D("hamp_wt_a3","hamp_wt_a3",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a3 = new TH2D("hadc_wt_a3","hadc_wt_a3",400,0,100,4000,0,1000);

  TH2D *hamp_wt_a3_t2 = new TH2D("hamp_wt_a3_t2","hamp_wt_a3_t2",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a3_t2 = new TH2D("hadc_wt_a3_t2","hadc_wt_a3_t2",400,0,100,4000,0,1000);

  TH2D *hamp_wt_a3_all = new TH2D("hamp_wt_a3_all","hamp_wt_a3_all",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a3_all = new TH2D("hadc_wt_a3_all","hadc_wt_a3_all",400,0,100,4000,0,1000);
  TH2D *hamp_wt_a3_t2_all = new TH2D("hamp_wt_a3_t2_all","hamp_wt_a3_t2_all",400,0,100,4000,0,1000);
  TH2D *hadc_wt_a3_t2_all = new TH2D("hadc_wt_a3_t2_all","hadc_wt_a3_t2_all",400,0,100,4000,0,1000);
  
  TH2F *hdt_s_t1 = new TH2F("hdt_s_t1","hdt_s_t1",24,-0.5,23.5,400,-20,20);
  TH2F *hdt_s_t2 = new TH2F("hdt_s_t2","hdt_s_t2",8,-0.5,7.5,400,-20,20);
  TH2F *hdt_s_ac = new TH2F("hdt_s_ac","hdt_s_ac",3,-0.5,2.5,400,-20,20);
  TH2F *hdt_s_ren = new TH2F("hdt_s_ren","hdt_s_ren",2,-0.5,1.5,400,-20,20);
  TH2F *hdt_s_ref = new TH2F("hdt_s_ref","hdt_s_ref",2,-0.5,1.5,400,-20,20);

  TH2F *htof_s_tsc1_r2s0 = new TH2F("htof_s_tsc1_r2s0","htof_s_tsc1_r2s0",24,-0.5,23.5,800,-40,40);
  TH2F *htof_s_tsc1_r2s1 = new TH2F("htof_s_tsc1_r2s1","htof_s_tsc1_r2s1",24,-0.5,23.5,800,-40,40);
  TH2F *htof_s_tsc1_r2s0_ac = new TH2F("htof_s_tsc1_r2s0_ac","htof_s_tsc1_r2s0_ac",24,-0.5,23.5,800,-40,40);
  TH2F *htof_s_tsc1_r2s1_ac = new TH2F("htof_s_tsc1_r2s1_ac","htof_s_tsc1_r2s1_ac",24,-0.5,23.5,800,-40,40);

  TH2F *hmt_s_t1_tref = new TH2F("hmt_s_t1_tref","hmt_s_t1_tref",24,-0.5,23.5,5000,0,500);
  TH2F *hmt_s_t2_tref = new TH2F("hmt_s_t2_tref","hmt_s_t2_tref",8,-0.5,7.5,5000,0,500);
  TH2F *hmt_s_ac_tref = new TH2F("hmt_s_ac_tref","hmt_s_ac_tref",3,-0.5,2.5,5000,0,500);
  TH2F *hmt_s_ren_tref = new TH2F("hmt_s_ren_tref","hmt_s_ren_tref",2,-0.5,1.5,5000,0,500);
  TH2F *hmt_s_ref_tref = new TH2F("hmt_s_ref_tref","hmt_s_ref_tref",2,-0.5,1.5,5000,0,500);
  TH2F *hmt_s_ac_tsc1 = new TH2F("hmt_s_ac_tsc1","hmt_s_ac_tsc1",3,-0.5,2.5,5000,0,500);
  TH2F *hmt_s_ac_tsc2 = new TH2F("hmt_s_ac_tsc2","hmt_s_ac_tsc2",3,-0.5,2.5,5000,0,500);
  
  TH2F *hdt_s_t1_tref = new TH2F("hdt_s_t1_tref","hdt_s_t1_tref",24,-0.5,23.5,400,-20,20);
  TH2F *hdt_s_t2_tref = new TH2F("hdt_s_t2_tref","hdt_s_t2_tref",8,-0.5,7.5,400,-20,20);
  TH2F *hdt_s_ac_tref = new TH2F("hdt_s_ac_tref","hdt_s_ac_tref",3,-0.5,2.5,400,-20,20);
  TH2F *hdt_s_ren_tref = new TH2F("hdt_s_ren_tref","hdt_s_ren_tref",2,-0.5,1.5,400,-20,20);
  TH2F *hdt_s_ref_tref = new TH2F("hdt_s_ref_tref","hdt_s_ref_tref",2,-0.5,1.5,400,-20,20);

  TH2F *hmt_wt_t1[nstrip_tsc1] = {0}; 
  TH2F *hmt_wt_t2[nstrip_tsc2] = {0}; 
  TH2F *hmt_wt_ac[n_ac] = {0}; 
  TH2F *hmt_wt_ren[n_rpcr] = {0}; 
  TH2F *hmt_wt_ref[n_rpcr] = {0};
  TH2F *hmt_wt_ren_tref[n_rpcr] = {0}; 
  TH2F *hmt_wt_ref_tref[n_rpcr] = {0}; 
  for (Int_t is=0;is<nstrip_tsc1;is++) {
    hmt_wt_t1[is] = new TH2F(Form("hmt_wt_t1_%d",is),Form("hmt_wt_t1_%d",is),200,0.5,50.5,5000,0,500);
  }  
  for (Int_t is=0;is<nstrip_tsc2;is++) {
    hmt_wt_t2[is] = new TH2F(Form("hmt_wt_t2_%d",is),Form("hmt_wt_t2_%d",is),200,0.5,50.5,5000,0,500);
  }  
  for (Int_t is=0;is<n_ac;is++) {
    hmt_wt_ac[is] = new TH2F(Form("hmt_wt_ac_%d",is),Form("hmt_wt_ac_%d",is),200,0.5,50.5,5000,0,500);
  } 
  for (Int_t is=0;is<n_rpcr;is++) {
    hmt_wt_ren[is] = new TH2F(Form("hmt_wt_ren_%d",is),Form("hmt_wt_ren_%d",is),200,0.5,50.5,5000,0,500);
    hmt_wt_ref[is] = new TH2F(Form("hmt_wt_ref_%d",is),Form("hmt_wt_ref_%d",is),200,0.5,50.5,5000,0,500);
    hmt_wt_ren_tref[is] = new TH2F(Form("hmt_wt_ren_tref_%d",is),Form("hmt_wt_ren_tref_%d",is),200,0.5,50.5,5000,0,500);
    hmt_wt_ref_tref[is] = new TH2F(Form("hmt_wt_ref_tref_%d",is),Form("hmt_wt_ref_tref_%d",is),200,0.5,50.5,5000,0,500);
  } 
  Int_t nentries = (Int_t)htree->GetEntries();
  for (Int_t i=0; i<nentries; i++) {
    if ((i%10000) == 0) {
      cout << "event " << i << endl;
    }

    htree->GetEntry(i);

    Double_t lt_tRPC = 0; 
    Double_t lt_tTSC = 0;
    Double_t lt_tAC = 0;
    Double_t lt_tREF = 0;
    Double_t lt_tL1 = 0;
    Double_t lt_tL1_delay = 0;

    if (hnh_trig1[0]>0) {
      lt_tRPC =  hlt_trig1[0][0];
      htrpc->Fill(lt_tRPC);
    }
    if (hnh_trig1[1]>0) {
      lt_tTSC = hlt_trig1[1][0];
      //      cout << "lt_tTSC = " << lt_tTSC << endl;
      httsc->Fill(lt_tTSC);
    }
    if (hnh_trig1[2]>0) {
      //      cout << "lt_tAC = " << lt_tAC << endl;
      lt_tAC = hlt_trig1[2][0];
      htac->Fill(lt_tAC);
    }
    if (hnh_trig1[3]>0) {
      //      cout << "lt_tREF = " << lt_tREF << endl;
      lt_tREF = hlt_trig1[3][0];
      htref->Fill(lt_tREF);
    }
    if (hnh_trig1[4]>0) {
      lt_tL1 = hlt_trig1[4][0];
      htl1->Fill(lt_tL1);
    }
    if (hnh_trig1[5]>0) {
      lt_tL1_delay = hlt_trig1[5][0];
      htl1d->Fill(lt_tL1_delay);
    }


    const Double_t tTSC_min = 240;//?
    const Double_t tTSC_max = 243;//?

    const Double_t ltTSC1_min1 = 381.5;//channel 0-7
    const Double_t ltTSC1_max1 = 384.5;//channel 8-23
    const Double_t ltTSC1_min2 = 380;//channel 0-7
    const Double_t ltTSC1_max2 = 393;//channel 8-23
    const Double_t ltTSC2_min = 386.5;
    const Double_t ltTSC2_max = 389.0;
    //    const Double_t tAC_min = 295;
    //    const Double_t tAC_max = 295.7;
    const Double_t tAC_min = 295;
    const Double_t tAC_max = 296.5;
    const Double_t ltAC_min = 365;
    const Double_t ltAC_max = 373;
    Double_t tREF_min = 287;
    Double_t tREF_max = 288.2;

    if (40104<=run) {
      tREF_min = 294;
      tREF_max = 297;
    }

    Bool_t trig_RPC = false;
    Bool_t trig_TSC = false;
    Bool_t trig_AC = false;
    Bool_t trig_REF = false;

    if ((tTSC_min<lt_tTSC)&&(lt_tTSC<tTSC_max)) {
      trig_TSC = true;
    }
    if ((tAC_min<lt_tAC)&&(lt_tAC<tAC_max)) {
      trig_AC = true;
    }
    if ((tREF_min<lt_tREF)&&(lt_tREF<tREF_max)) {
      trig_REF = true;
    }

  
    //mean tdc
    Double_t mt_t1[nstrip_tsc1] = {0};
    for (Int_t is=0;is<nstrip_tsc1;is++) {
      mt_t1[is] = 0;
    }
    Double_t mt_t2[nstrip_tsc2] = {0};
    Double_t mt_ac[n_ac] = {0};
    Double_t mt_re[n_rpcr][2] = {0};//near/far

    //diff tdc
    Double_t dt_t1[nstrip_tsc1] = {0};
    Double_t dt_t2[nstrip_tsc2] = {0};
    Double_t dt_re[n_rpcr][2] = {0};//near/far
    for (Int_t idet=0;idet<n_rpcr;idet++) {
      for (Int_t is=0;is<2;is++) {
	dt_re[idet][is]=-1000;
      }
    }

    //tot tdc
    Double_t wt_t1[nstrip_tsc1] = {0};
    Double_t wt_t2[nstrip_tsc2] = {0};
    Double_t wt_ac[n_ac] = {0};
    Double_t wt_re[n_rpcr][2] = {0};//near/far
    
    for (Int_t is=0;is<nstrip_tsc1;is++) {
      Double_t lt_b = -1;
      Double_t lt_t = -1;
      Double_t tt_b = -1;
      Double_t tt_t = -1;
      if (hnh_tsc1[is][0]>0) {
	lt_b = hlt_tsc1[is][0][0];
	tt_b = htt_tsc1[is][0][0];
	hlt_s_t1->Fill(is,lt_b);
      }
      if (hnh_tsc1[is][1]>0) {
	lt_t = hlt_tsc1[is][1][0];
	tt_t = htt_tsc1[is][1][0];
	hlt_s_t1->Fill(is+24,lt_t);
      }
      if ((hnh_tsc1[is][0]>0)&&(hnh_tsc1[is][1]>0)) {
	mt_t1[is] = (lt_t+lt_b)/2.;
	dt_t1[is] = lt_t-lt_b;
	wt_t1[is] = ((lt_t-tt_t)+(lt_b-tt_b))/2.;
      }
      //      if ((hnh_tsc1[is][0]>0)||(hnh_tsc1[is][1]>0)) {
	//	cout << "t1: is, lt_t, lt_b, mt = " << is << " " << lt_t << " " << lt_b << " " << mt_t1[is] << endl;
      //      }

    }
    for (Int_t is=0;is<nstrip_tsc2;is++) {
      Double_t lt_b = -1;
      Double_t lt_t = -1;
      Double_t tt_b = -1;
      Double_t tt_t = -1;
      if (hnh_tsc2[is][0]>0) {
	lt_b = hlt_tsc2[is][0][0];
	tt_b = htt_tsc2[is][0][0];
	hlt_s_t2->Fill(is,lt_b);
      }
      if (hnh_tsc2[is][1]>0) {
	lt_t = hlt_tsc2[is][1][0];
	tt_t = htt_tsc2[is][1][0];
	hlt_s_t2->Fill(is+8,lt_t);
      }
      if ((hnh_tsc2[is][0]>0)&&(hnh_tsc2[is][1]>0)) {
	mt_t2[is] = (lt_t+lt_b)/2.;
	dt_t2[is] = lt_t-lt_b;
	wt_t2[is] = ((lt_t-tt_t)+(lt_b-tt_b))/2.;
      }
    }
    for (Int_t is=0;is<n_ac;is++) {
      if (hnh_ac[is]>0) {
	Double_t lt = hlt_ac[is][0];
	Double_t tt = htt_ac[is][0];
	mt_ac[is] = lt;
	wt_ac[is] = lt-tt;
	hlt_s_a->Fill(is,lt);
      }
    }
    for (Int_t is=0;is<n_rpcr;is++) {
      if (is==0) {
	if (hnh_rpcr[0][0]>0) {
	  hlt_s_ref1->Fill(0.,hlt_rpcr[0][0][0]);
	}
	if (hnh_rpcr[0][1]>0) {
	  hlt_s_ref1->Fill(1.,hlt_rpcr[0][1][0]);
	}
	if (hnh_rpcr[0][2]>0) {
	  hlt_s_ref1->Fill(2.,hlt_rpcr[0][2][0]);
	}
	if (hnh_rpcr[0][3]>0) {
	  hlt_s_ref1->Fill(3.,hlt_rpcr[0][3][0]);
	}
      } else if (is==1) {
	if (hnh_rpcr[1][0]>0) {
	  hlt_s_ref2->Fill(0.,hlt_rpcr[1][0][0]);
	}
	if (hnh_rpcr[1][1]>0) {
	  hlt_s_ref2->Fill(1.,hlt_rpcr[1][1][0]);
	}
	if (hnh_rpcr[1][2]>0) {
	  hlt_s_ref2->Fill(2.,hlt_rpcr[1][2][0]);
	}
	if (hnh_rpcr[1][3]>0) {
	  hlt_s_ref2->Fill(3.,hlt_rpcr[1][3][0]);
	}
      }
      if ((hnh_rpcr[is][0]>0)&&(hnh_rpcr[is][1]>0)) {
	Double_t lt_b = hlt_rpcr[is][0][0];
	Double_t lt_t = hlt_rpcr[is][1][0];
	Double_t tt_b = htt_rpcr[is][0][0];
	Double_t tt_t = htt_rpcr[is][1][0];
	mt_re[is][0] = (lt_t+lt_b)/2.;
	dt_re[is][0] = lt_t-lt_b;
	wt_re[is][0] = ((lt_t-tt_t)+(lt_b-tt_b))/2.;
      }
      if ((hnh_rpcr[is][2]>0)&&(hnh_rpcr[is][3]>0)) {
	Double_t lt_b = hlt_rpcr[is][2][0];
	Double_t lt_t = hlt_rpcr[is][3][0];
	Double_t tt_b = htt_rpcr[is][2][0];
	Double_t tt_t = htt_rpcr[is][3][0];
	mt_re[is][1] = (lt_t+lt_b)/2.;
	dt_re[is][1] = lt_t-lt_b;
	wt_re[is][1] = ((lt_t-tt_t)+(lt_b-tt_b))/2.;
      }
    }

    const Double_t ref0_0_m =6.39396e-02;
    const Double_t ref0_0_s =1.35967e-01;
    const Double_t ref1_0_m =-2.71760e-01;
    const Double_t ref1_0_s =9.81199e-02;

    const Double_t ref0_1_m =-4.62932e-01;
    const Double_t ref0_1_s = 2.08055e-01;
    const Double_t ref1_1_m =-3.41804e-01;
    const Double_t ref1_1_s = 8.57070e-02;
    const Double_t ref0_fac = 4;
    const Double_t ref1_fac = 4;

    const Double_t ref_mt_mean = 370.;
    const Double_t ref_mt_cut = 5.;
    const Double_t ref0_wt_min = 12.;
    const Double_t ref1_wt_min = 8.;

    const Double_t t1_1_mt_mean = 3.82997e+02;//CH0-7
    const Double_t t1_1_mt_cut = 4.59990e-01*3.;
    const Double_t t1_2_mt_mean = 3.88652e+02;//CH8-23
    const Double_t t1_2_mt_cut = 1.13047e+00*3.;
    const Double_t t1_mt_mean = 3.83006e+02;
    const Double_t t1_mt_cut = 4.96994e-01*3.;
    //    const Double_t t2_mt_mean = 3.88092e+02;
    //    const Double_t t2_mt_cut = 4.70724e-01*3;
    const Double_t t2_mt_mean = 4.12424e+02;
    //    const Double_t t2_mt_cut = 2.89574e+00*3;
    const Double_t t2_mt_cut = 15;
    const Double_t t1_dt_mean = 0;
    const Double_t t1_dt_cut = 6;
    const Double_t t2_dt_mean = 0;
    const Double_t t2_dt_cut = 17;
    const Double_t ac_mt_mean = 370.;
    const Double_t ac_mt_cut = 1.;

    Bool_t good_ref1 = false;
    if ((fabs(dt_re[0][0]-ref0_0_m)<ref0_fac*ref0_0_s)&&(fabs(dt_re[0][1]-ref0_1_m)<ref0_fac*ref0_1_s)
	&&(fabs(mt_re[0][0]-ref_mt_mean)<ref_mt_cut)&&(fabs(mt_re[0][1]-ref_mt_mean)<ref_mt_cut)
	&&(wt_re[0][0]>ref0_wt_min)&&(wt_re[0][1]>ref0_wt_min)) {
      good_ref1 = true;
    }
    Bool_t good_ref2 = false;
    if ((fabs(dt_re[1][0]-ref1_0_m)<ref1_fac*ref1_0_s)&&(fabs(dt_re[1][1]-ref1_1_m)<ref0_fac*ref1_1_s)
	&&(fabs(mt_re[1][0]-ref_mt_mean)<ref_mt_cut)&&(fabs(mt_re[1][1]-ref_mt_mean)<ref_mt_cut)
	&&(wt_re[1][0]>ref1_wt_min)&&(wt_re[1][1]>ref1_wt_min)) {
      good_ref2 = true;
    }
    Bool_t good_tsc1 = false;
    Int_t is_good_tsc1 = -1;
    for (Int_t is=0;is<nstrip_tsc1;is++) {
      Double_t t1_mt_mean = 0;
      Double_t t1_mt_cut = 0;
      if (is<=7) {
	t1_mt_mean = t1_1_mt_mean;
	t1_mt_cut = t1_1_mt_cut;
      } else {
	t1_mt_mean = t1_2_mt_mean;
	t1_mt_cut = t1_2_mt_cut;
      }
      if ((fabs(dt_t1[is]-t1_dt_mean)<t1_dt_cut)
	  &&(fabs(mt_t1[is]-t1_mt_mean)<t1_mt_cut)) {
	good_tsc1 = true;
	is_good_tsc1 = is;
	break;
      }
    }
    Bool_t good_tsc2 = false;
    Int_t is_good_tsc2 = -1;
    for (Int_t is=0;is<nstrip_tsc2;is++) {
      if ((fabs(dt_t2[is]-t2_dt_mean)<t2_dt_cut)
	  &&(fabs(mt_t2[is]-t2_mt_mean)<t2_mt_cut)) {
	good_tsc2 = true;
	is_good_tsc2 = is;
	break;
      }
    }
    Bool_t good_ac = false;
    Int_t is_good_ac = -1;
    for (Int_t is=0;is<n_ac;is++) {
      if (fabs(mt_ac[is]-ac_mt_mean)<ac_mt_cut) {
	good_ac = true;
	is_good_ac = is;
	break;
      }
    }
    
  
    for (Int_t is=0;is<nstrip_tsc1;is++) {
      if (mt_t1[is]>0) {
	hdt_s_t1->Fill(is,dt_t1[is]);
	hmt_s_t1->Fill(is,mt_t1[is]);
	hmt_wt_t1[is]->Fill(wt_t1[is],mt_t1[is]);
	if (trig_REF) {
	  hdt_s_t1_tref->Fill(is,dt_t1[is]);
	  hmt_s_t1_tref->Fill(is,mt_t1[is]);
	}
      }
    }
    for (Int_t is=0;is<nstrip_tsc2;is++) {
      if (mt_t2[is]>0) {
	hdt_s_t2->Fill(is,dt_t2[is]);
	hmt_s_t2->Fill(is,mt_t2[is]);
	hmt_wt_t2[is]->Fill(wt_t2[is],mt_t2[is]);
	if (trig_REF) {
	  hdt_s_t2_tref->Fill(is,dt_t2[is]);
	  hmt_s_t2_tref->Fill(is,mt_t2[is]);
	}
      }
    }
    for (Int_t is=0;is<n_ac;is++) {
      if (mt_ac[is]>0) {
	hmt_s_ac->Fill(is,mt_ac[is]);
	hmt_wt_ac[is]->Fill(wt_ac[is],mt_ac[is]);
	if (good_tsc1) {
	  hmt_s_ac_tsc1->Fill(is,mt_ac[is]);
	}
	if (good_tsc2) {
	  hmt_s_ac_tsc2->Fill(is,mt_ac[is]);
	}
	if (trig_REF) {
	  hmt_s_ac_tref->Fill(is,mt_ac[is]);
	}

	if (hnw_ac[is]>0) {
	  for (Int_t ie=0;ie<hnw_ac[is];ie++) {
	    if (is==0) {
	      if (ie==0) {
		hamp_a1->Fill(hamp_ac[is][ie]);
		hadc_a1->Fill(hadc_ac[is][ie]);
		hbl_a1->Fill(hbl_ac[is][ie]);
		hamp_wt_a1->Fill(wt_ac[is],hamp_ac[is][ie]);
		hadc_wt_a1->Fill(wt_ac[is],hadc_ac[is][ie]);
		if (good_tsc2) {
		  hamp_wt_a1_t2->Fill(wt_ac[is],hamp_ac[is][ie]);
		  hadc_wt_a1_t2->Fill(wt_ac[is],hadc_ac[is][ie]);
		}
	      }
	      hamp_wt_a1_all->Fill(wt_ac[is],hamp_ac[is][ie]);
	      hadc_wt_a1_all->Fill(wt_ac[is],hadc_ac[is][ie]);
	      if (good_tsc2) {
		hamp_wt_a1_t2_all->Fill(wt_ac[is],hamp_ac[is][ie]);
		hadc_wt_a1_t2_all->Fill(wt_ac[is],hadc_ac[is][ie]);
	      }
	    } else if (is==1) {
	      if (ie==0) {
		hamp_a2->Fill(hamp_ac[is][ie]);
		hadc_a2->Fill(hadc_ac[is][ie]);
		hbl_a2->Fill(hbl_ac[is][ie]);
		hamp_wt_a2->Fill(wt_ac[is],hamp_ac[is][ie]);
		hadc_wt_a2->Fill(wt_ac[is],hadc_ac[is][ie]);
		if (good_tsc2) {
		  hamp_wt_a2_t2->Fill(wt_ac[is],hamp_ac[is][ie]);
		  hadc_wt_a2_t2->Fill(wt_ac[is],hadc_ac[is][ie]);
		}
	      }
	      hamp_wt_a2_all->Fill(wt_ac[is],hamp_ac[is][ie]);
	      hadc_wt_a2_all->Fill(wt_ac[is],hadc_ac[is][ie]);
	      if (good_tsc2) {
		hamp_wt_a2_t2_all->Fill(wt_ac[is],hamp_ac[is][ie]);
		hadc_wt_a2_t2_all->Fill(wt_ac[is],hadc_ac[is][ie]);
	      }
	    } else if (is==2) {
	      if (ie==0) {
		hamp_a3->Fill(hamp_ac[is][ie]);
		hadc_a3->Fill(hadc_ac[is][ie]);
		hbl_a3->Fill(hbl_ac[is][ie]);
		hamp_wt_a3->Fill(wt_ac[is],hamp_ac[is][ie]);
		hadc_wt_a3->Fill(wt_ac[is],hadc_ac[is][ie]);
		if (good_tsc2) {
		  hamp_wt_a3_t2->Fill(wt_ac[is],hamp_ac[is][ie]);
		  hadc_wt_a3_t2->Fill(wt_ac[is],hadc_ac[is][ie]);
		}
	      }
	      hamp_wt_a3_all->Fill(wt_ac[is],hamp_ac[is][ie]);
	      hadc_wt_a3_all->Fill(wt_ac[is],hadc_ac[is][ie]);
	      if (good_tsc2) {
		hamp_wt_a3_t2_all->Fill(wt_ac[is],hamp_ac[is][ie]);
		hadc_wt_a3_t2_all->Fill(wt_ac[is],hadc_ac[is][ie]);
	      }
	    }
	  }
	}
      }
    }
    for (Int_t is=0;is<n_rpcr;is++) {
      if (mt_re[is][0]>0) {
	hdt_s_ren->Fill(is,dt_re[is][0]);
	hmt_s_ren->Fill(is,mt_re[is][0]);
	hmt_wt_ren[is]->Fill(wt_re[is][0],mt_re[is][0]);
	if (trig_REF) {
	  hmt_wt_ren_tref[is]->Fill(wt_re[is][0],mt_re[is][0]);
	  hdt_s_ren_tref->Fill(is,dt_re[is][0]);
	  hmt_s_ren_tref->Fill(is,mt_re[is][0]);
	}
      }
      if (mt_re[is][1]>0) {
	hdt_s_ref->Fill(is,dt_re[is][1]);
	hmt_s_ref->Fill(is,mt_re[is][1]);
	hmt_wt_ref[is]->Fill(wt_re[is][1],mt_re[is][1]);
	if (trig_REF) {
	  hmt_wt_ref_tref[is]->Fill(wt_re[is][1],mt_re[is][1]);
	  hdt_s_ref_tref->Fill(is,dt_re[is][1]);
	  hmt_s_ref_tref->Fill(is,mt_re[is][1]);
	}
      }
    }


    //next what?
    //correlations of ref counter, TSC, and RPC
    //good ref
  }


  TFile *fout = new TFile(argv[3],"RECREATE");

  htrpc->Write();
  httsc->Write();
  htac->Write();
  htref->Write();
  htl1->Write();
  htl1d->Write();

  hdt_s_t1->Write();
  hdt_s_t2->Write();
  hdt_s_ac->Write();
  hdt_s_ren->Write();
  hdt_s_ref->Write();
  
  hmt_s_t1->Write();
  hmt_s_t2->Write();
  hmt_s_ac->Write();
  hmt_s_ren->Write();
  hmt_s_ref->Write();

  hdt_s_t1_tref->Write();
  hdt_s_t2_tref->Write();
  hdt_s_ac_tref->Write();
  hdt_s_ren_tref->Write();
  hdt_s_ref_tref->Write();

  hmt_s_t1_tref->Write();
  hmt_s_t2_tref->Write();
  hmt_s_ac_tref->Write();
  hmt_s_ren_tref->Write();
  hmt_s_ref_tref->Write();
  hmt_s_ac_tsc1->Write();
  hmt_s_ac_tsc2->Write();

  htof_s_tsc1_r2s0->Write();
  htof_s_tsc1_r2s1->Write();
  htof_s_tsc1_r2s0_ac->Write();
  htof_s_tsc1_r2s1_ac->Write();
  
  for (Int_t is=0;is<nstrip_tsc1;is++) {
    hmt_wt_t1[is]->Write();
  }
  for (Int_t is=0;is<nstrip_tsc2;is++) {
    hmt_wt_t2[is]->Write();
  }
  for (Int_t is=0;is<n_ac;is++) {
    hmt_wt_ac[is]->Write();
  }
  for (Int_t is=0;is<n_rpcr;is++) {
    hmt_wt_ren[is]->Write();
    hmt_wt_ref[is]->Write();
    hmt_wt_ren_tref[is]->Write();
    hmt_wt_ref_tref[is]->Write();
  }

  hlt_s_t1->Write();
  hlt_s_t2->Write();
  hlt_s_ref1->Write();
  hlt_s_ref2->Write();
  hlt_s_a->Write();

  hamp_a1->Write();
  hadc_a1->Write();
  hbl_a1->Write();
  hamp_wt_a1->Write();
  hadc_wt_a1->Write();
  hamp_wt_a1_t2->Write();
  hadc_wt_a1_t2->Write();
  hamp_wt_a1_all->Write();
  hadc_wt_a1_all->Write();
  hamp_wt_a1_t2_all->Write();
  hadc_wt_a1_t2_all->Write();

  hamp_a2->Write();
  hadc_a2->Write();
  hbl_a2->Write();
  hamp_wt_a2->Write();
  hadc_wt_a2->Write();
  hamp_wt_a2_t2->Write();
  hadc_wt_a2_t2->Write();
  hamp_wt_a2_all->Write();
  hadc_wt_a2_all->Write();
  hamp_wt_a2_t2_all->Write();
  hadc_wt_a2_t2_all->Write();

  hamp_a3->Write();
  hadc_a3->Write();
  hbl_a3->Write();
  hamp_wt_a3->Write();
  hadc_wt_a3->Write();
  hamp_wt_a3_t2->Write();
  hadc_wt_a3_t2->Write();
  hamp_wt_a3_all->Write();
  hadc_wt_a3_all->Write();
  hamp_wt_a3_t2_all->Write();
  hadc_wt_a3_t2_all->Write();

  fout->Close();
}

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
#include <TStyle.h>
using namespace std;

static const Int_t debug = false;

//sako 2021/12/1
#define DTC_HALF_SLEW //new method by Kyan to remove crab claw

//run 3624

Double_t t0_r1[8][2] = {{239,239},
{230.839,230.412},
{230.295,230.529},
{230.242,230.331},
{239,239},
{229.794,230.576},
{229.149,230.394},
{229.299,230.068}};
Double_t t0_r2[8][2] = {{238.2,238.2},
{230.36,231.175},
{229.595,230.938},
{229.653,231.108},
{229.385,230.623},
{228.965,230.645},
{228.583,231.099},
{238.2,238.2}};
Double_t t0_ts[8][2] = {{231.656,230.332},
{231.823,231.212},
{231.539,231.489},
{231.682,231.223},
{229.884,230.531},
{231.394,231.507},
{230.652,231.467},
{230.822,230.984}};


//run0a, run3623, 2021/7/2
Double_t t0_es[18] = {210.396,211.404,211.177,211.229,211.244,211.968,211.86,211.267,211.231,211.442,211.828,211.949,210.823,211.592,211.162,211.231,211.62,211.869};
Double_t et0_es[18] = {0.000448594,0.00193805,0.00194841,0.00205703,0.00167747,0.00114757,0.00102827,0.000829976,0.000547132,0.000575682,0.000688913,0.00643491,0.000959052,0.00118267,0.00168931,0.00334496,0.00127197,0.000574774};
Double_t st_es[18] = {0.419868,0.724031,0.737936,0.892567,0.659897,0.748277,0.67617,0.40836,0.563674,0.576873,0.576893,1.2973,0.681089,0.680305,0.758028,1.05833,0.678593,0.450173};
Double_t est_es[18] = {0.000498953,0.00161513,0.00207641,0.00238567,0.00216821,0.00131694,0.00116737,0.00108072,0.00059911,0.000737556,0.00103411,0.00559903,0.00108521,0.00141189,0.00216779,0.00314972,0.00136065,0.000660565};

Double_t t0c_es[18] = {211.966,213.231,212.043,211.707,212.336,212.023,211.391,211.804,211.552,211.279,211.808,212.294,212.057,211.966,211.336,213.299,208.72,208.882};
Double_t et0c_es[18] = {0.00340034,0.00783548,0.0175553,0.0111543,0.00392291,0.0037236,0.0101303,0.0108431,0.00254846,0.00386069,0.0103272,0.00888351,0.00621973,0.00549213,0.0248536,0.0107419,0.0053961,0.00350102};
Double_t stc_es[18] = {0.674834,0.936352,0.981818,1.01184,0.65047,0.820324,0.984471,1.00451,0.673016,0.805681,1.10335,1.20482,0.887253,0.806643,1.12127,1.15407,0.819604,0.555739};
Double_t estc_es[18] = {0.00334919,0.0106681,0.0149134,0.0125114,0.00446799,0.00408556,0.00997517,0.00918897,0.0026573,0.0045474,0.00971863,0.0123189,0.0065877,0.00602912,0.0210438,0.0120609,0.0061189,0.0037903};


//const Double_t esc_mt_window = 0.7*6;
//const Double_t esc_mt_mean = 212;
const Double_t esc_mt_window = 2.;
const Double_t esc_mt_mean = 211.3;
const Double_t esc_mt_min = 210.3;
const Double_t esc_mt_max = 213.7;


static const Double_t PI = 3.1415927;



int main(int argc,char *argv[]) {

  cout << "argc = " << argc << endl;
  if (argc<4) {
    cerr << "Usage ana_rpccosmic run infile outfile listflag" << endl;
    exit(-1);
  }
  cout << "infile = " << argv[1] << endl;
  cout << "outfile = " << argv[2] << endl;

  Int_t run = atoi(argv[1]);

  //  getT0(run);
  //  getESCT0(run);


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

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
  Double_t hlt_tmin = 100;
  Double_t hlt_tmax = 160;

  if (run>=2052) {
    hlt_tmin = 150;
    hlt_tmax = 200;
  }

  //  const Double_t r1_wmin = 0;

  TH1D *hlt_r1 = new TH1D("hlt_r1","hlt_r1",600,hlt_tmin,hlt_tmax);
  TH1D *hlt_r2 = new TH1D("hlt_r2","hlt_r2",600,hlt_tmin,hlt_tmax);
  TH1D *hlt_s1 = new TH1D("hlt_s1","hlt_s1",600,hlt_tmin,hlt_tmax);
  TH1D *hlt_s2 = new TH1D("hlt_s2","hlt_s2",600,hlt_tmin,hlt_tmax);
  TH1D *hlt_s3 = new TH1D("hlt_s3","hlt_s3",600,hlt_tmin,hlt_tmax);
  TH1D *hlt_s4 = new TH1D("hlt_s4","hlt_s4",600,hlt_tmin,hlt_tmax);

  TH2D *hlt_wt_r1 = new TH2D("hlt_wt_r1","hlt_wt_r1",80,0,80,200,175,185);
  TH2D *hlt_wt_r2 = new TH2D("hlt_wt_r2","hlt_wt_r2",80,0,80,200,175,185);
  TH2D *hlt_wt_s1 = new TH2D("hlt_wt_s1","hlt_wt_s1",80,0,80,200,150,160);
  TH2D *hlt_wt_s2 = new TH2D("hlt_wt_s2","hlt_wt_s2",80,0,80,200,150,160);
  TH2D *hlt_wt_s3 = new TH2D("hlt_wt_s3","hlt_wt_s3",80,0,80,200,155,165);
  TH2D *hlt_wt_s4 = new TH2D("hlt_wt_s4","hlt_wt_s4",80,0,80,200,155,165);

  TH1D *hmt_r12 = new TH1D("hmt_r12","hmt_r12",600,hlt_tmin,hlt_tmax);
  TH1D *hmt_s12 = new TH1D("hmt_s12","hmt_s12",600,hlt_tmin,hlt_tmax);
  TH1D *hmt_s34 = new TH1D("hmt_s34","hmt_s34",600,hlt_tmin,hlt_tmax);

  TH2D *hmt_wt_r12 = new TH2D("hmt_wt_r12","hmt_wt_r12",50,0,50,200,175,185);
  TH2D *hmt_wt_s12 = new TH2D("hmt_wt_s12","hmt_wt_s12",50,0,50,200,150,160);
  TH2D *hmt_wt_s34 = new TH2D("hmt_wt_s34","hmt_wt_s34",50,0,50,200,155,165);

  TH2D *hmt_wt_r12_c12 = new TH2D("hmt_wt_r12_c12","hmt_wt_r12_c12",80,0,80,200,175,185);
  TH2D *hmt_wt_s12_c12 = new TH2D("hmt_wt_s12_c12","hmt_wt_s12_c12",80,0,80,200,150,160);
  TH2D *hmt_wt_s34_c12 = new TH2D("hmt_wt_s34_c12","hmt_wt_s34_c12",80,0,80,200,155,165);

  TH1D *hdt_r12 = new TH1D("hdt_r12","hdt_r12",1200,-30,30);
  TH1D *hdt_s12 = new TH1D("hdt_s12","hdt_s12",1200,-30,30);
  TH1D *hdt_s34 = new TH1D("hdt_s34","hdt_s34",1200,-30,30);

  TH1D *htof_r12_s12 = new TH1D("htof_r12_s12","htof_r12_s12",1000,0,50);
  TH1D *htof_r12_s34 = new TH1D("htof_r12_s34","htof_r12_s34",1000,0,50);
  TH1D *htof_s12_s34 = new TH1D("htof_s12_s34","htof_s12_s34",1000,-25,25);

  TH1D *htof_r12_s12_sl = new TH1D("htof_r12_s12_sl","htof_r12_s12_sl",500,0,50);
  TH1D *htof_r12_s34_sl = new TH1D("htof_r12_s34_sl","htof_r12_s34_sl",500,0,50);
  TH1D *htof_s12_s34_sl = new TH1D("htof_s12_s34_sl","htof_s12_s34_sl",500,-25,25);

  TH1D *htof_r12_s12_slg = new TH1D("htof_r12_s12_slg","htof_r12_s12_slg",500,0,50);
  TH1D *htof_r12_s34_slg = new TH1D("htof_r12_s34_slg","htof_r12_s34_slg",500,0,50);
  TH1D *htof_s12_s34_slg = new TH1D("htof_s12_s34_slg","htof_s12_s34_slg",500,-25,25);

  TH1D *htofs_r12_s12_slg = new TH1D("htofs_r12_s12_slg","htofs_r12_s12_slg",500,0,50);
  TH1D *htofs_r12_s34_slg = new TH1D("htofs_r12_s34_slg","htofs_r12_s34_slg",500,0,50);
  TH1D *htofs_s12_s34_slg = new TH1D("htofs_s12_s34_slg","htofs_s12_s34_slg",500,-25,25);

  TH1D *htof_r12_s12_c12 = new TH1D("htof_r12_s12_c12","htof_r12_s12_c12",500,0,50);
  TH1D *htof_r12_s34_c12 = new TH1D("htof_r12_s34_c12","htof_r12_s34_c12",500,0,50);
  TH1D *htof_s12_s34_c12 = new TH1D("htof_s12_s34_c12","htof_s12_s34_c12",500,-25,25);

  TH1D *htof_r12_s12_c12g = new TH1D("htof_r12_s12_c12g","htof_r12_s12_c12g",500,0,50);
  TH1D *htof_r12_s34_c12g = new TH1D("htof_r12_s34_c12g","htof_r12_s34_c12g",500,0,50);
  TH1D *htof_s12_s34_c12g = new TH1D("htof_s12_s34_c12g","htof_s12_s34_c12g",500,-25,25);

  TH1D *htofs_r12_s12_c12g = new TH1D("htofs_r12_s12_c12g","htofs_r12_s12_c12g",500,0,50);
  TH1D *htofs_r12_s34_c12g = new TH1D("htofs_r12_s34_c12g","htofs_r12_s34_c12g",500,0,50);
  TH1D *htofs_s12_s34_c12g = new TH1D("htofs_s12_s34_c12g","htofs_s12_s34_c12g",500,-25,25);

  TH1D *htof_r12_s12_c1 = new TH1D("htof_r12_s12_c1","htof_r12_s12_c1",1000,0,50);
  TH1D *htof_r12_s34_c1 = new TH1D("htof_r12_s34_c1","htof_r12_s34_c1",1000,0,50);
  TH1D *htof_s12_s34_c1 = new TH1D("htof_s12_s34_c1","htof_s12_s34_c1",1000,-25,25);

  TH1D *htof_r12_s12_c2 = new TH1D("htof_r12_s12_c2","htof_r12_s12_c2",1000,0,50);
  TH1D *htof_r12_s34_c2 = new TH1D("htof_r12_s34_c2","htof_r12_s34_c2",1000,0,50);
  TH1D *htof_s12_s34_c2 = new TH1D("htof_s12_s34_c2","htof_s12_s34_c2",1000,-25,25);


  Double_t t0_global = 0;

  if ((20124<=run)&&(run<=20126)) {
    t0_global = 34.26;
  } else if (run<=20127) {
    t0_global = 20;
  } else if (run<=20129) {
    t0_global = -10;
  } else if (run<=20130) {
    t0_global = -25;
  } else  {
    t0_global = -19;
  }

  Int_t hev=0;
  //2021/7/1
  Int_t hrun=0;
  Double_t rpc_tmin = 130;
  Double_t rpc_tmax = 160;
  Double_t trig_tmin = 110;
  //  const Double_t trig_tmax = 130;//def
  Double_t trig_tmax = 120;//from run 2040 (discri width 60/40->10ns)
  Double_t c1_tmin = 90;
  Double_t c1_tmax = 120;
  Double_t c2_tmin = 95;
  Double_t c2_tmax = 125;
  Double_t s1_tmin = 110;
  Double_t s1_tmax = 140;
  Double_t s2_tmin = 110;
  Double_t s2_tmax = 140;
  Double_t s3_tmin = 110;
  Double_t s3_tmax = 140;
  Double_t s4_tmin = 110;
  Double_t s4_tmax = 140;

  Double_t s12_wtmin = 20;
  Double_t s12_wtmax = 46;
  Double_t s34_wtmin = 20;
  Double_t s34_wtmax = 47;
  Double_t r12_wtmin = 1;
  Double_t r12_wtmax = 8;

  Double_t s12_mean = -1.47497e+00;
  Double_t s34_mean = 4.35745e-02;
  Double_t s12_sigma = 5.60209e-01;
  Double_t s34_sigma = 7.31343e-01;
  Double_t s12_tcut = 0.3;
  Double_t s34_tcut = 0.3;

  if (run>=2052) {
    rpc_tmin = 165;
    rpc_tmax = 195;
    c1_tmin = 125;
    c1_tmax = 155;
    c2_tmin = 125;
    c2_tmax = 155;
    s1_tmin = 145;
    s1_tmax = 175;
    s2_tmin = 145;
    s2_tmax = 175;
    s3_tmin = 145;
    s3_tmax = 175;
    s4_tmin = 145;
    s4_tmax = 175;
  }
  //  const Double_t trig_tmax = 120;

  const Int_t max_nh = 20;
  const Int_t nstrip_rpc = 8;
  const Int_t nstrip_tsc = 8;
  const Int_t nch_esc = 20;//ESC1-18,FSC1,FSC2
  const Int_t nt_b = 2;//bottom/top
  const Int_t nch_trig1 = 6;
  const Int_t nch_trig2 = 6;

  //coordinate system
  const Double_t yc_RPC1 = +1.65;//mm w.r.t the beam height (y=0)
  const Double_t yc_RPC2 = -2.0;//mm
  const Double_t t_RPC1 = 27;
  const Double_t t_RPC2 = 26;
  const Double_t r_RPC1 = 1325.7-t_RPC1/2.;
  const Double_t r_RPC2 = 1345.5+t_RPC2/2.;
  const Double_t the_RPC1_deg = 119.7;
  const Double_t the_RPC2_deg = 120.2;

  const Double_t dothe_RPC1_deg = -0.5799;//z-x orientation (clockwize = +)
  const Double_t dothe_RPC2_deg = +1.16;
  const Double_t dothe_TSC_deg = dothe_RPC1_deg;

  const Double_t yc_TSC = -4;
  const Double_t r_TSC = r_RPC1-39.9;
  const Double_t drtheta_TSC = -7.4;//mm w.r.t the_RPC1_deg
  const Double_t dtheta_TSC = drtheta_TSC/r_TSC/PI*180.;
  const Double_t the_TSC_deg=the_RPC1_deg+dtheta_TSC;
  //scintillator refractive index
  const Double_t nsci = 1.58;
  const Double_t clight = 299.792458;//mm/ns
  //  const Double_t vl_TSC = clight/nsci;
  //default

  
  const Double_t mt_rpc1_min = 228.0;
  //    const Double_t mt_rpc1_max = 234;
  const Double_t mt_rpc1_max = 240;

  const Double_t wt_rpc1_min = 0.8;
  const Double_t wt_rpc1_max = 7.0;
  
  const Double_t mt_rpc2_min = 228.5;
  //    const Double_t mt_rpc2_max = 234;
  const Double_t mt_rpc2_max = 240;
  
  //2021/7/7 test
  const Double_t wt_rpc2_min = 1.0;
  const Double_t wt_rpc2_max = 10.0;

  //was def    const Double_t mt_tsc_min = 225.8;
  const Double_t mt_tsc_min = 220.; 
  const Double_t mt_tsc_max = 233.2;
  

  //2021/7/7 test
  const Double_t wt_tsc_min = 1.5;
  const Double_t wt_tsc_max = 12.;

  //additional cuts
  Double_t lt_tsc0_line_cut0 = 0;
  Double_t lt_tsc0_line_cut1 = 0;


  //loose cuts
  //  const Double_t lt_sc_min_cut = 60.7;
  Double_t lt_sc_min_lcut = 58;
  Double_t lt_sc_max_lcut = 62.0;
  Double_t wt_sc_min_lcut = 1.8;
  Double_t wt_sc_max_lcut = 6.6;

  //  Double_t lb_sc_min_lcut = 59.8;
  Double_t lb_sc_min_lcut = 57.4;
  Double_t lb_sc_max_lcut = 60.6;
  Double_t wb_sc_min_lcut = 2.0;
  Double_t wb_sc_max_lcut = 6.5;

  Double_t lt_tsc0_min_lcut = 59.3;
  Double_t lt_tsc0_max_lcut = 61.7;
  Double_t wt_tsc0_min_lcut = 3.0;
  Double_t wt_tsc0_max_lcut = 7.4;
  //  Double_t wt_tsc0_min_lcut = 4.7;
  //  Double_t wt_tsc0_max_lcut = 7.4;

  Double_t lb_tsc0_min_lcut = 59.0;
  Double_t lb_tsc0_max_lcut = 61.7;
  //  Double_t wb_tsc0_min_lcut = 3.8;
  //  Double_t wb_tsc0_max_lcut = 7.8;
  Double_t wb_tsc0_min_lcut = 2.0;
  Double_t wb_tsc0_max_lcut = 7.8;

  Double_t lt_tsc1_min_lcut = 58.7;
  Double_t lt_tsc1_max_lcut = 59.3;
  Double_t wt_tsc1_min_lcut = 3.5;
  Double_t wt_tsc1_max_lcut = 7.5;

  Double_t lb_tsc1_min_lcut = 59.8;
  Double_t lb_tsc1_max_lcut = 60.8;
  Double_t wb_tsc1_min_lcut = 2.75;
  Double_t wb_tsc1_max_lcut = 9.0;

  //tight cuts
  //  Double_t lt_sc_min_cut = 60.7;
  Double_t lt_sc_min_cut = 57.5;
  Double_t lt_sc_max_cut = 60.8;
  //  Double_t wt_sc_min_cut = 1.0;
  //  Double_t wt_sc_max_cut = 7.0;
  Double_t wt_sc_min_cut = 7.0;
  Double_t wt_sc_max_cut = 13.0;

  Double_t lb_sc_min_cut = 58.3;
  Double_t lb_sc_max_cut = 61.5;
  Double_t wb_sc_min_cut = 7.5;
  Double_t wb_sc_max_cut = 13.0;

  //  Double_t lt_tsc0_min_cut = 59.0;
  Double_t lt_tsc0_min_cut = 58.2;
  //  Double_t lt_tsc0_max_cut = 61.7;
  Double_t lt_tsc0_max_cut = 60.4;
  Double_t wt_tsc0_min_cut = 5.0;
  Double_t wt_tsc0_max_cut = 11.5;
  //  Double_t wt_tsc0_max_cut = 8.0;
  //  Double_t wt_tsc0_min_cut = 4.7;
  //  Double_t wt_tsc0_max_cut = 7.4;

  Double_t lb_tsc0_min_cut = 58.0;
  Double_t lb_tsc0_max_cut = 61.0;
  Double_t wb_tsc0_min_cut = 5.0;
  //  Double_t wb_tsc0_max_cut = 7.5;
  Double_t wb_tsc0_max_cut = 10.0;

  Double_t lt_tsc1_min_cut = 58.7;
  Double_t lt_tsc1_max_cut = 59.3;
  Double_t wt_tsc1_min_cut = 1.0;
  Double_t wt_tsc1_max_cut = 7.5;

  Double_t lb_tsc1_min_cut = 59.8;
  Double_t lb_tsc1_max_cut = 60.8;
  Double_t wb_tsc1_min_cut = 0;
  Double_t wb_tsc1_max_cut = 16;
  //dt cuts
  Double_t dt_sc_mean = -1.11198e+00;
  Double_t dt_sc_sigma = 3.38099e-01;

  Double_t dt_tsc0_mean = -3.73862e-02;
  Double_t dt_tsc0_sigma = 3.51155e-01;

  Double_t dt_tsc1_mean = -1.68290e+00;
  Double_t dt_tsc1_sigma = 4.54266e-01;

  Double_t dt_tsc2_mean = -1.00181e+00;
  Double_t dt_tsc2_sigma = 6.87325e-01;

   Double_t dt_cut_factor = 1.0;//1 sigma
  
  //slewing corrections
  Double_t lt_sc_slew[2] = {57.9672,0.120074};
  Double_t lt_sc_slew_w0 = (1.0+7.0)/2.;

  Double_t lb_sc_slew[2] = {59.2808,0.197411};
  Double_t lb_sc_slew_w0 = (2.0+6.0)/2.;

  Double_t lt_tsc0_slew[2] = {57.4856,0.589614};
  Double_t lt_tsc0_slew_w0 = (2.5+5.0)/2.;

  Double_t lb_tsc0_slew[2] = {58.0713,0.367384};
  Double_t lb_tsc0_slew_w0 = (3.5+7.0)/2.;

  Double_t lt_tsc1_slew[2] = {58.9543,-0.045654};
  Double_t lt_tsc1_slew_w0 = (2.5+5.0)/2.;

  Double_t lb_tsc1_slew[2] = {60.1574,0.0091059};
  Double_t lb_tsc1_slew_w0 = 6.0;

 
  //dt corrections
  Double_t mt_sc_dtcor[4];
  mt_sc_dtcor[0] = 60.2762;
  mt_sc_dtcor[1] = 0.168968;  
  mt_sc_dtcor[2] = 0;
  mt_sc_dtcor[3] = 0;
  Double_t dt0_sc_dtcor = -1.15;

  Double_t mt_tsc0_dtcor[4];
  mt_tsc0_dtcor[0] = 60.3278;
  mt_tsc0_dtcor[1] = -0.30933;
  mt_tsc0_dtcor[2] = 0;
  mt_tsc0_dtcor[3] = 0;
  Double_t dt0_tsc0_dtcor = 0;

  Double_t mt_tsc1_dtcor[4];
  mt_tsc1_dtcor[0] = 55.0517;
  mt_tsc1_dtcor[1] = -1.04511;
  mt_tsc1_dtcor[2] = -0.121112;
  mt_tsc1_dtcor[3] = 0;
  Double_t dt0_tsc1_dtcor = -1.02109e+00;

 
  //tof dt corrections
  Double_t stsc0_tof_dt[2] = {0.0450296,0.00189021};
  Double_t stsc0_tof_dt0 = -2;
  Double_t stsc1_tof_dt[2] = {1.6999,0.514776};
  Double_t stsc1_tof_dt0 = -2;
  Double_t tsc01_tof_dt[2] = {1.65546,0.51316};
  Double_t tsc01_tof_dt0 = -2;

  //tof wm corrections
  Double_t stsc0_tof_wm_sc[2] = {-0.547112,0.100808};
  Double_t stsc0_tof_wm_sc_0 = 6.1;
  Double_t stsc0_tof_wm_tsc0[2] = {0.297009,-0.0102623};
  Double_t stsc0_tof_wm_tsc0_0 = 24.75;
  Double_t stsc1_tof_wm_sc[2] = {-0.104465,0.129758};
  Double_t stsc1_tof_wm_sc_0 = 6.1;
  Double_t stsc1_tof_wm_tsc1[2] = {0.886593,-0.0313818};
  Double_t stsc1_tof_wm_tsc1_0 = 7.1;
  Double_t tsc01_tof_wm_tsc0[2] = {0.398637,0.00920436};
  Double_t tsc01_tof_wm_tsc0_0 = 24.5;
  Double_t tsc01_tof_wm_tsc1[2] = {0.841069,-0.0312133};
  Double_t tsc01_tof_wm_tsc1_0 = 7.1;

  //TSC efficiency
  const Double_t xpd_tsc_min = 1100;
  const Double_t xpd_tsc_max = 1116.5;
  
  const Double_t ypd_tsc_min = -30;
  const Double_t ypd_tsc_max =  30;
  
  const Double_t ypd_tsc_min2 = -20;
  const Double_t ypd_tsc_max2 =  20;
  
  //probably no resolution to resolve TSC slat
  const Double_t spd_tsc_min1 = 0.5;
  const Double_t spd_tsc_max1 = 2.5;
  const Double_t spd_tsc_min2 = 6.5;
  const Double_t spd_tsc_max2 = 7.5;
   
  //matching w/ loose vertex cut
  const Double_t xres_tsc_min = -67;
  const Double_t xres_tsc_max =  67;
  
  const Double_t sres_tsc_mean = -4.78661e+00;
  const Double_t sres_tsc_sigma = 9.61232e+00;
	 
  const Double_t sres_tsc_min = sres_tsc_mean-2.*sres_tsc_sigma;//-24.;
  const Double_t sres_tsc_max = sres_tsc_mean+2.*sres_tsc_sigma; //14.;
  
  const Double_t yres_tsc_mean = -8.51193e+00;
  const Double_t yres_tsc_sigma = 3.00864e+01;
  const Double_t yres_tsc_min = yres_tsc_mean-3.*yres_tsc_sigma;
  const Double_t yres_tsc_max = yres_tsc_mean+3.*yres_tsc_sigma;

  //2021/3/18
  const Double_t vz_tsc_min = -2000;
  const Double_t vz_tsc_max = 2600;
  
  //2021/6/30
  const Double_t vy_tsc_min  = 184.324-2.*783.591;
  const Double_t vy_tsc_max  = 184.324+2.*783.591;
  const Double_t vy_tsc_min2 = 184.324-1.*783.591;
  const Double_t vy_tsc_max2 = 184.324+1.*783.591;
  
  const Double_t dt_0_RPC1=0;
  //end cuts

  Double_t dt1_r1[8] = {-3.30833,-3.15,-3.8,-3.5,-3.30833,-2.85,-3.4,-3.15};
  Double_t dt1_r2[8] = {-3.65833,-4.2,-3.4,-3.4,-3.65,-3.85,-3.45,-3.65833};
  Double_t dt1_ts[8] = {-2.85,-2.15,-1.375,-2.4,-1.4,-1.525,0.075,-1.275};
  Double_t dt2_r1[8] = {4.78333,4.9,4.2,5.1,4.78333,5.15,4.6,4.75};
  Double_t dt2_r2[8] = {4.75,4.05,5.65,5.05,4.85,4.35,4.55,4.75};
  Double_t dt2_ts[8] = {-0.075,0.575,1.325,0.25,1.5,1.15,2.675,1.375};
  Double_t tm_r1[8] = {0.7375,0.875,0.2,0.8,0.7375,1.15,0.6,0.8};
  Double_t tm_r2[8] = {0.545833,-0.075,1.125,0.825,0.6,0.25,0.55,0.545833};
  Double_t tm_ts[8] = {-1.4625,-0.7875,-0.025,-1.075,0.05,-0.1875,1.375,0.05};
  Double_t vl_r1[8] = {181.563,182.36,183.5,170.698,181.563,183.5,183.5,185.823};
  Double_t vl_r2[8] = {174.851,177.939,162.21,173.728,172.706,179.024,183.5,174.851};
  Double_t vl_ts[8] = {72.0721,73.3945,74.0741,75.4717,68.9655,74.7664,76.9231,75.4717};
  Double_t dt_r1[8] = {8.09167,8.05,8,8.6,8.09167,8,8,7.9};
  Double_t dt_r2[8] = {8.40833,8.25,9.05,8.45,8.5,8.2,8,8.40833};
  Double_t dt_ts[8] = {2.775,2.725,2.7,2.65,2.9,2.675,2.6,2.65};
  Double_t dt1_r1_mean = -3.30833;
  Double_t dt2_r1_mean = 4.78333;
  Double_t dt_r1_mean = 8.09167;
  Double_t dt_r1_cut = (dt_r1_mean*0.8)/2.;
  Double_t tm_r1_mean = 0.7375;
  Double_t vl_r1_mean = 181.563;
  Double_t dt1_r2_mean = -3.65833;
  Double_t dt2_r2_mean = 4.75;
  Double_t dt_r2_mean = 8.40833;
  Double_t dt_r2_cut = (dt_r2_mean*0.8)/2.;
  Double_t tm_r2_mean = 0.545833;
  Double_t vl_r2_mean = 174.851;
  Double_t dt1_ts_mean = -1.6125;
  Double_t dt2_ts_mean = 1.09688;
  Double_t dt_ts_mean = 2.70938;
  Double_t dt_ts_cut = (dt_ts_mean*0.8)/2.;
  Double_t tm_ts_mean = -0.257812;
  Double_t vl_ts_mean = 73.8924;


  //run0b
  const Double_t vl_TSC = vl_ts_mean;//50mm/ns from data
  const Double_t vl_RPC1 = vl_r1_mean;//mm/ns from data
  const Double_t vl_RPC2 = vl_r2_mean;//mm/ns from data
  
  
  const Double_t ds_RPC1 = 25.5;//strip width
  const Double_t ds_RPC2 = 25.5;//strip width
  const Double_t ds_TSC  = 4.3;//strip width
  const Double_t dt0_TSC = -3.66766e-02;//ns
  const Double_t dt0_RPC1 = -1.38002e+00;//ns
  // is (strip_id)
  // z_rpc1 = zc-((double)is-3.5)*ds_RPC1*cos((120.-90.+dothe_RPC1_deg)/PI)
  // x_rpc1 = xc-((double)is-3.5)*ds_RPC1*sin((120.-90.+dothe_RPC1_deg)/PI)
  // y_rpc1 = yc+(t_u-t_d)*vl_RPC
  const Double_t zc_RPC1 = r_RPC1*cos(the_RPC1_deg/180.*PI);
  const Double_t xc_RPC1 = r_RPC1*sin(the_RPC1_deg/180.*PI);
  const Double_t zc_RPC2 = r_RPC2*cos(the_RPC2_deg/180.*PI);
  const Double_t xc_RPC2 = r_RPC2*sin(the_RPC2_deg/180.*PI);
  const Double_t zc_TSC = r_TSC*cos(the_TSC_deg/180.*PI);
  const Double_t xc_TSC = r_TSC*sin(the_TSC_deg/180.*PI);
  
  //  TTree *htree = new TTree("htree","all hits");
  Int_t hnh_rpc1[nstrip_rpc][nt_b];
  Int_t hnh_rpc2[nstrip_rpc][nt_b];
  Int_t hnh_tsc[nstrip_tsc][nt_b];
  Int_t hnh_hsc;
  Int_t hnh_esc[nch_esc];
  Int_t hnh_trig1[nch_trig1];
  Int_t hnh_trig2[nch_trig2];
  Int_t hnh_fsc_disc1;
  Int_t hnh_fsc_disc2;
    
  Double_t hlt_rpc1[nstrip_rpc][nt_b][max_nh];
  Double_t htt_rpc1[nstrip_rpc][nt_b][max_nh];
  Double_t hlt_rpc2[nstrip_rpc][nt_b][max_nh];
  Double_t htt_rpc2[nstrip_rpc][nt_b][max_nh];
  Double_t hlt_tsc[nstrip_tsc][nt_b][max_nh];
  Double_t htt_tsc[nstrip_tsc][nt_b][max_nh];
  Double_t hlt_hsc[max_nh];
  Double_t htt_hsc[max_nh];
  Double_t hlt_esc[nch_esc][max_nh];
  Double_t htt_esc[nch_esc][max_nh];
  Double_t hlt_trig1[nch_trig1][max_nh];
  Double_t htt_trig1[nch_trig1][max_nh];
  Double_t hlt_trig2[nch_trig2][max_nh];
  Double_t htt_trig2[nch_trig2][max_nh];
  
  Double_t hlt_fsc_disc1[max_nh];
  Double_t htt_fsc_disc1[max_nh];
  Double_t hlt_fsc_disc2[max_nh];
  Double_t htt_fsc_disc2[max_nh];


  htree->SetBranchAddress("ievt",&hev);
  htree->SetBranchAddress("irun",&hrun);//2021/7/1
  htree->SetBranchAddress("nh_rpc1",hnh_rpc1);//[strip][up_down]
  htree->SetBranchAddress("nh_rpc2",hnh_rpc2);//[strip][up_down]
  htree->SetBranchAddress("nh_tsc",hnh_tsc);//[strip][up_down]
  htree->SetBranchAddress("nh_hsc",&hnh_hsc);//[ch]
  htree->SetBranchAddress("nh_esc",hnh_esc);//[ch]
  htree->SetBranchAddress("nh_trig1",hnh_trig1);//[ch]
  htree->SetBranchAddress("nh_trig2",hnh_trig2);//[ch]
  htree->SetBranchAddress("nh_fsc_disc1",&hnh_fsc_disc1);//[ch]
  htree->SetBranchAddress("nh_fsc_disc2",&hnh_fsc_disc2);//[ch]
    
  htree->SetBranchAddress("lt_rpc1",hlt_rpc1);//[strip][up_down][max_hit]
  htree->SetBranchAddress("lt_rpc2",hlt_rpc2);
  htree->SetBranchAddress("lt_tsc",hlt_tsc);
  htree->SetBranchAddress("lt_hsc",hlt_hsc);
  htree->SetBranchAddress("lt_esc",hlt_esc);
  htree->SetBranchAddress("lt_trig1",hlt_trig1);//trig1(L1/ESC/MRPC/TSC/HSC)
  htree->SetBranchAddress("lt_trig2",hlt_trig2);//trig1(L1)
  htree->SetBranchAddress("lt_fsc_disc1",hlt_fsc_disc1);//FSC1 disc[TDC1]
  htree->SetBranchAddress("lt_fsc_disc2",hlt_fsc_disc2);//FSC1 disc[TDC2]
  
  htree->SetBranchAddress("tt_rpc1",htt_rpc1);
  htree->SetBranchAddress("tt_rpc2",htt_rpc2);
  htree->SetBranchAddress("tt_tsc",htt_tsc);
  htree->SetBranchAddress("tt_esc",htt_esc);
  
  htree->SetBranchAddress("tt_hsc",htt_hsc);
  htree->SetBranchAddress("tt_trig1",htt_trig1);//trig1(L1/ESC/MRPC/TSC/HSC)
  htree->SetBranchAddress("tt_trig2",htt_trig2);//trig1(L1)
  htree->SetBranchAddress("tt_fsc_disc1",htt_fsc_disc1);//FSC1 disc[TDC1]
  htree->SetBranchAddress("tt_fsc_disc2",htt_fsc_disc2);//FSC1 disc[TDC2]


  //histo


  Int_t nt_bin_SC = 2500;
  Int_t t_min_SC = 40.;
  Int_t t_max_SC = 70.;
  //  Int_t nt_bin_SC = 1500;
  //    Int_t t_min_SC = 0.;
  //    Int_t t_max_SC = 150.;
  Int_t ndt_bin_SC = 40;
  Int_t dt_min_SC = -8.;
  Int_t dt_max_SC = 8.;
  //  Int_t nw_bin_SC = 1600;
  Int_t nw_bin_SC = 80;
  Int_t w_min_SC = 0.;
  Int_t w_max_SC = 40.;
  Int_t ntof_bin_SC = 300;
  Int_t tof_min_SC = -10.;
  Int_t tof_max_SC = 20.;

  TH1F *hsc_nh_tb = new TH1F("hsc_nh_tb","hsc_nh_tb",20,-0.5,19.5);
  TH1F *hsc_nh[2] = {0};
  for (Int_t itb=0;itb<2;itb++) {
    hsc_nh[itb] = new TH1F(Form("hsc_nh_tb%d",itb),Form("hsc_nh_tb%d",itb),20,-0.5,19.5);
  }
  TH1F *hsc_mt = new TH1F("hsc_mt","hsc_mt",nt_bin_SC,t_min_SC,t_max_SC);
  TH1F *hsc_dt = new TH1F("hsc_dt","hsc_dt",ndt_bin_SC*4,dt_min_SC,dt_max_SC);
  TH1F *hsc_dts = new TH1F("hsc_dts","hsc_dts",ndt_bin_SC*4,dt_min_SC,dt_max_SC);

  TH1F *hsc_mt_g = new TH1F("hsc_mt_g","hsc_mt_g",nt_bin_SC,t_min_SC,t_max_SC);
  TH1F *hsc_dt_g = new TH1F("hsc_dt_g","hsc_dt_g",ndt_bin_SC*4,dt_min_SC,dt_max_SC);

  TH1F *hsc_wm = new TH1F("hsc_wm","hsc_wm",nw_bin_SC,w_min_SC,w_max_SC);
  TH2F *hsc_mt_wm = new TH2F("hsc_mt_wm","hsc_mt_wm",nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *hsc_dt_wm = new TH2F("hsc_dt_wm","hsc_dt_wm",ndt_bin_SC,dt_min_SC,dt_max_SC,nw_bin_SC,w_min_SC,w_max_SC);
  TH2F *hsc_dt_w0 = new TH2F("hsc_dt_w0","hsc_dt_w0",ndt_bin_SC,dt_min_SC,dt_max_SC,nw_bin_SC,w_min_SC,w_max_SC);
  TH2F *hsc_dt_w1 = new TH2F("hsc_dt_w1","hsc_dt_w1",ndt_bin_SC,dt_min_SC,dt_max_SC,nw_bin_SC,w_min_SC,w_max_SC);
  TH1F *hsc_lt[2] = {0};
  TH1F *hsc_tt[2] = {0};
  TH1F *hsc_w[2] = {0};
  for (Int_t itb=0;itb<2;itb++) {
    hsc_lt[itb] = new TH1F(Form("hsc_lt%d",itb),Form("hsc_lt%d",itb),nt_bin_SC,t_min_SC,t_max_SC);
    hsc_tt[itb] = new TH1F(Form("hsc_tt%d",itb),Form("hsc_tt%d",itb),nt_bin_SC,t_min_SC,t_max_SC);
    hsc_w[itb] = new TH1F(Form("hsc_w%d",itb),Form("hsc_w%d",itb),nw_bin_SC,w_min_SC,w_max_SC);
  }

  TH2F *hsc_lt_wt = new TH2F(Form("hsc_lt_wt"),Form("hsc_lt_wt"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *hsc_lt_wb = new TH2F(Form("hsc_lt_wb"),Form("hsc_lt_wb"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *hsc_lt_wt_g = new TH2F(Form("hsc_lt_wt_g"),Form("hsc_lt_wt_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *hsc_lt_wb_g = new TH2F(Form("hsc_lt_wb_g"),Form("hsc_lt_wb_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *hsc_lts_wt_g = new TH2F(Form("hsc_lts_wt_g"),Form("hsc_lts_wt_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *hsc_lts_wb_g = new TH2F(Form("hsc_lts_wb_g"),Form("hsc_lts_wb_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *hsc_lt_wt_gall = new TH2F(Form("hsc_lt_wt_gall"),Form("hsc_lt_wt_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *hsc_lt_wb_gall = new TH2F(Form("hsc_lt_wb_gall"),Form("hsc_lt_wb_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *hsc_lts_wt_gall = new TH2F(Form("hsc_lts_wt_gall"),Form("hsc_lts_wt_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *hsc_lts_wb_gall = new TH2F(Form("hsc_lts_wb_gall"),Form("hsc_lts_wb_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);


  //correlations
  TH2F *hsc_tsc0_lt_lg = new TH2F(Form("hsc_tsc0_lt_lg"),Form("hsc_tsc0_lt_lg"),nt_bin_SC,t_min_SC,t_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *hsc_tsc0_lb_lg = new TH2F(Form("hsc_tsc0_lb_lg"),Form("hsc_tsc0_lb_lg"),nt_bin_SC,t_min_SC,t_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *hsc_tsc1_lt_lg = new TH2F(Form("hsc_tsc1_lt_lg"),Form("hsc_tsc1_lt_lg"),nt_bin_SC,t_min_SC,t_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *hsc_tsc1_lb_lg = new TH2F(Form("hsc_tsc1_lb_lg"),Form("hsc_tsc1_lb_lg"),nt_bin_SC,t_min_SC,t_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc0_tsc1_lt_lg = new TH2F(Form("htsc0_tsc1_lt_lg"),Form("htsc0_tsc1_lt_lg"),nt_bin_SC,t_min_SC,t_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc0_tsc1_lb_lg = new TH2F(Form("htsc0_tsc1_lb_lg"),Form("htsc0_tsc1_lb_lg"),nt_bin_SC,t_min_SC,t_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  //width correlation
  TH2F *hsc_tsc0_wt_lg = new TH2F(Form("hsc_tsc0_wt_lg"),Form("hsc_tsc0_wt_lg"),nw_bin_SC,w_min_SC,w_max_SC,nw_bin_SC,w_min_SC,w_max_SC);

  TH2F *hsc_tsc1_wt_lg = new TH2F(Form("hsc_tsc1_wt_lg"),Form("hsc_tsc1_wt_lg"),nw_bin_SC,w_min_SC,w_max_SC,nw_bin_SC,w_min_SC,w_max_SC);

  TH2F *htsc0_tsc1_wt_lg = new TH2F(Form("htsc0_tsc1_wt_lg"),Form("htsc0_tsc1_wt_lg"),nw_bin_SC,w_min_SC,w_max_SC,nw_bin_SC,w_min_SC,w_max_SC);

  TH2F *hsc_tsc0_wb_lg = new TH2F(Form("hsc_tsc0_wb_lg"),Form("hsc_tsc0_wb_lg"),nw_bin_SC,w_min_SC,w_max_SC,nw_bin_SC,w_min_SC,w_max_SC);

  TH2F *hsc_tsc1_wb_lg = new TH2F(Form("hsc_tsc1_wb_lg"),Form("hsc_tsc1_wb_lg"),nw_bin_SC,w_min_SC,w_max_SC,nw_bin_SC,w_min_SC,w_max_SC);

  TH2F *htsc0_tsc1_wb_lg = new TH2F(Form("htsc0_tsc1_wb_lg"),Form("htsc0_tsc1_wb_lg"),nw_bin_SC,w_min_SC,w_max_SC,nw_bin_SC,w_min_SC,w_max_SC);

  TH2F *hsc_dt_mt = new TH2F(Form("hsc_dt_mt"),Form("hsc_dt_mt"),ndt_bin_SC*4,dt_min_SC,dt_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc0_dt_mt = new TH2F(Form("htsc0_dt_mt"),Form("htsc0_dt_mt"),ndt_bin_SC*4,dt_min_SC,dt_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc1_dt_mt = new TH2F(Form("htsc1_dt_mt"),Form("htsc1_dt_mt"),ndt_bin_SC*4,dt_min_SC,dt_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *hsc_dts_mts = new TH2F(Form("hsc_dts_mts"),Form("hsc_dts_mts"),ndt_bin_SC*4,dt_min_SC,dt_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc0_dts_mts = new TH2F(Form("htsc0_dts_mts"),Form("htsc0_dts_mts"),ndt_bin_SC*4,dt_min_SC,dt_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc1_dts_mts = new TH2F(Form("htsc1_dts_mts"),Form("htsc1_dts_mts"),ndt_bin_SC*4,dt_min_SC,dt_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *hsc_dts_mtd = new TH2F(Form("hsc_dts_mtd"),Form("hsc_dts_mtd"),ndt_bin_SC*4,dt_min_SC,dt_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc0_dts_mtd = new TH2F(Form("htsc0_dts_mtd"),Form("htsc0_dts_mtd"),ndt_bin_SC*4,dt_min_SC,dt_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc1_dts_mtd = new TH2F(Form("htsc1_dts_mtd"),Form("htsc1_dts_mtd"),ndt_bin_SC*4,dt_min_SC,dt_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htofs_stsc0_dt_tsc0 = new TH2F("htofs_stsc0_dt_tsc0","htofs_stsc0_dt_tsc0",ndt_bin_SC*4,dt_min_SC,dt_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofs_stsc1_dt_tsc0 = new TH2F("htofs_stsc1_dt_tsc0","htofs_stsc1_dt_tsc0",ndt_bin_SC*4,dt_min_SC,dt_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofs_tsc01_dt_tsc0 = new TH2F("htofs_tsc01_dt_tsc0","htofs_tsc01_dt_tsc0",ndt_bin_SC*4,dt_min_SC,dt_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);

  TH2F *htofs_stsc0_dt_tsc1 = new TH2F("htofs_stsc0_dt_tsc1","htofs_stsc0_dt_tsc1",ndt_bin_SC*4,dt_min_SC,dt_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofs_stsc1_dt_tsc1 = new TH2F("htofs_stsc1_dt_tsc1","htofs_stsc1_dt_tsc1",ndt_bin_SC*4,dt_min_SC,dt_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofs_tsc01_dt_tsc1 = new TH2F("htofs_tsc01_dt_tsc1","htofs_tsc01_dt_tsc1",ndt_bin_SC*4,dt_min_SC,dt_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);

  TH2F *htofd_stsc0_dt_tsc1 = new TH2F("htofd_stsc0_dt_tsc1","htofd_stsc0_dt_tsc1",ndt_bin_SC*4,dt_min_SC,dt_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofd_stsc1_dt_tsc1 = new TH2F("htofd_stsc1_dt_tsc1","htofd_stsc1_dt_tsc1",ndt_bin_SC*4,dt_min_SC,dt_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofd_tsc01_dt_tsc1 = new TH2F("htofd_tsc01_dt_tsc1","htofd_tsc01_dt_tsc1",ndt_bin_SC*4,dt_min_SC,dt_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);

  TH2F *htofd_stsc0_wm_sc = new TH2F("htofd_stsc0_wm_sc","htofd_stsc0_wm_sc",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofd_stsc1_wm_sc = new TH2F("htofd_stsc1_wm_sc","htofd_stsc1_wm_sc",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofd_tsc01_wm_tsc0 = new TH2F("htofd_tsc01_wm_tsc0","htofd_tsc01_wm_tsc0",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);

  TH2F *htofd_stsc0_wm_tsc0 = new TH2F("htofd_stsc0_wm_tsc0","htofd_stsc0_wm_tsc0",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofd_stsc1_wm_tsc1 = new TH2F("htofd_stsc1_wm_tsc1","htofd_stsc1_wm_tsc1",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofd_tsc01_wm_tsc1 = new TH2F("htofd_tsc01_wm_tsc1","htofd_tsc01_wm_tsc1",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);

  TH2F *htofdw_stsc0_wm_sc = new TH2F("htofdw_stsc0_wm_sc","htofdw_stsc0_wm_sc",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofdw_stsc1_wm_sc = new TH2F("htofdw_stsc1_wm_sc","htofdw_stsc1_wm_sc",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofdw_tsc01_wm_tsc0 = new TH2F("htofdw_tsc01_wm_tsc0","htofdw_tsc01_wm_tsc0",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);

  TH2F *htofdw_stsc0_wm_tsc0 = new TH2F("htofdw_stsc0_wm_tsc0","htofdw_stsc0_wm_tsc0",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofdw_stsc1_wm_tsc1 = new TH2F("htofdw_stsc1_wm_tsc1","htofdw_stsc1_wm_tsc1",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH2F *htofdw_tsc01_wm_tsc1 = new TH2F("htofdw_tsc01_wm_tsc1","htofdw_tsc01_wm_tsc1",nw_bin_SC*2,w_min_SC,w_max_SC,ntof_bin_SC, tof_min_SC,tof_max_SC);

  TH1F *htsc_nh_tb[3]={0};
  TH1F *htsc_nh[3][2] = {0};
  TH1F *htsc_mt[3]={0}; 
  TH1F *htsc_dt[3]={0}; 
  TH1F *htsc_dts[3]={0}; 
  TH1F *htsc_mt_g[3]={0}; 
  TH1F *htsc_dt_g[3]={0}; 
  TH1F *htsc_wm[3]={0}; 
  TH2F *htsc_mt_wm[3]={0}; 
  TH2F *htsc_dt_wm[3]={0}; 
  TH2F *htsc_dt_w0[3]={0}; 
  TH2F *htsc_dt_w1[3]={0}; 
  TH1F *htsc_lt[3][2] = {0};
  TH1F *htsc_tt[3][2] = {0};
  TH1F *htsc_w[3][2] = {0};

  for (Int_t itsc=0;itsc<3;itsc++) {
    htsc_nh_tb[itsc] = new TH1F(Form("htsc%d_nh_tb",itsc),Form("htsc%d_nh_tb",itsc),20,-0.5,19.5);
    for (Int_t itb=0;itb<2;itb++) {
      htsc_nh[itsc][itb] = new TH1F(Form("htsc%d_nh_tb%d",itsc,itb),Form("htsc%d_nh_tb%d",itsc,itb),20,-0.5,19.5);
    }
    htsc_mt[itsc] = new TH1F(Form("htsc%d_mt",itsc),Form("htsc%d_mt",itsc),nt_bin_SC,t_min_SC,t_max_SC);
    htsc_dt[itsc] = new TH1F(Form("htsc%d_dt",itsc),Form("htsc%d_dt",itsc),ndt_bin_SC*4,dt_min_SC,dt_max_SC);
    htsc_dts[itsc] = new TH1F(Form("htsc%d_dts",itsc),Form("htsc%d_dts",itsc),ndt_bin_SC*4,dt_min_SC,dt_max_SC);
    htsc_mt_g[itsc] = new TH1F(Form("htsc%d_mt_g",itsc),Form("htsc%d_mt_g",itsc),nt_bin_SC,t_min_SC,t_max_SC);
    htsc_dt_g[itsc] = new TH1F(Form("htsc%d_dt_g",itsc),Form("htsc%d_dt_g",itsc),ndt_bin_SC*4,dt_min_SC,dt_max_SC);
    htsc_wm[itsc] = new TH1F(Form("htsc%d_wm",itsc),Form("htsc%d_wm",itsc),nw_bin_SC,w_min_SC,w_max_SC);
    htsc_mt_wm[itsc] = new TH2F(Form("htsc%d_mt_wm",itsc),Form("htsc%d_mt_wm",itsc),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
    htsc_dt_wm[itsc] = new TH2F(Form("htsc%d_dt_wm",itsc),Form("htsc%d_dt_wm",itsc),ndt_bin_SC,dt_min_SC,dt_max_SC,nw_bin_SC,w_min_SC,w_max_SC);
    htsc_dt_w0[itsc] = new TH2F(Form("htsc%d_dt_w0",itsc),Form("htsc%d_dt_w0",itsc),ndt_bin_SC,dt_min_SC,dt_max_SC,nw_bin_SC,w_min_SC,w_max_SC);
    htsc_dt_w1[itsc] = new TH2F(Form("htsc%d_dt_w1",itsc),Form("htsc%d_dt_w1",itsc),ndt_bin_SC,dt_min_SC,dt_max_SC,nw_bin_SC,w_min_SC,w_max_SC);

    for (Int_t itb=0;itb<2;itb++) {
      htsc_lt[itsc][itb] = new TH1F(Form("htsc%d_lt%d",itsc,itb),Form("htsc%d_lt%d",itsc,itb),nt_bin_SC,t_min_SC,t_max_SC);
      htsc_tt[itsc][itb] = new TH1F(Form("htsc%d_tt%d",itsc,itb),Form("htsc%d_tt%d",itsc,itb),nt_bin_SC,t_min_SC,t_max_SC);
      htsc_w[itsc][itb] = new TH1F(Form("htsc%d_w%d",itsc,itb),Form("htsc%d_w%d",itsc,itb),nw_bin_SC,w_min_SC,w_max_SC);
    }
  }

  TH2F *htsc0_lt_wt = new TH2F(Form("htsc0_lt_wt"),Form("htsc0_lt_wt"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc0_lt_wb = new TH2F(Form("htsc0_lt_wb"),Form("htsc0_lt_wb"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc0_lt_wt_g = new TH2F(Form("htsc0_lt_wt_g"),Form("htsc0_lt_wt_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc0_lt_wb_g = new TH2F(Form("htsc0_lt_wb_g"),Form("htsc0_lt_wb_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc0_lts_wt_g = new TH2F(Form("htsc0_lts_wt_g"),Form("htsc0_lts_wt_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc0_lts_wb_g = new TH2F(Form("htsc0_lts_wb_g"),Form("htsc0_lts_wb_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc0_lt_wt_gall = new TH2F(Form("htsc0_lt_wt_gall"),Form("htsc0_lt_wt_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc0_lt_wb_gall = new TH2F(Form("htsc0_lt_wb_gall"),Form("htsc0_lt_wb_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc0_lts_wt_gall = new TH2F(Form("htsc0_lts_wt_gall"),Form("htsc0_lts_wt_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc0_lts_wb_gall = new TH2F(Form("htsc0_lts_wb_gall"),Form("htsc0_lts_wb_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc1_lt_wt = new TH2F(Form("htsc1_lt_wt"),Form("htsc1_lt_wt"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc1_lt_wb = new TH2F(Form("htsc1_lt_wb"),Form("htsc1_lt_wb"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc1_lt_wt_g = new TH2F(Form("htsc1_lt_wt_g"),Form("htsc1_lt_wt_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc1_lt_wb_g = new TH2F(Form("htsc1_lt_wb_g"),Form("htsc1_lt_wb_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc1_lts_wt_g = new TH2F(Form("htsc1_lts_wt_g"),Form("htsc1_lts_wt_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc1_lts_wb_g = new TH2F(Form("htsc1_lts_wb_g"),Form("htsc1_lts_wb_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc1_lt_wt_gall = new TH2F(Form("htsc1_lt_wt_gall"),Form("htsc1_lt_wt_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc1_lt_wb_gall = new TH2F(Form("htsc1_lt_wb_gall"),Form("htsc1_lt_wb_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc1_lts_wt_gall = new TH2F(Form("htsc1_lts_wt_gall"),Form("htsc1_lts_wt_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc1_lts_wb_gall = new TH2F(Form("htsc1_lts_wb_gall"),Form("htsc1_lts_wb_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc2_lt_wt = new TH2F(Form("htsc2_lt_wt"),Form("htsc2_lt_wt"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc2_lt_wb = new TH2F(Form("htsc2_lt_wb"),Form("htsc2_lt_wb"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc2_lt_wt_g = new TH2F(Form("htsc2_lt_wt_g"),Form("htsc2_lt_wt_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc2_lt_wb_g = new TH2F(Form("htsc2_lt_wb_g"),Form("htsc2_lt_wb_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc2_lts_wt_g = new TH2F(Form("htsc2_lts_wt_g"),Form("htsc2_lts_wt_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc2_lts_wb_g = new TH2F(Form("htsc2_lts_wb_g"),Form("htsc2_lts_wb_g"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc2_lt_wt_gall = new TH2F(Form("htsc2_lt_wt_gall"),Form("htsc2_lt_wt_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc2_lt_wb_gall = new TH2F(Form("htsc2_lt_wb_gall"),Form("htsc2_lt_wb_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);

  TH2F *htsc2_lts_wt_gall = new TH2F(Form("htsc2_lts_wt_gall"),Form("htsc2_lts_wt_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);
  TH2F *htsc2_lts_wb_gall = new TH2F(Form("htsc2_lts_wb_gall"),Form("htsc2_lts_wb_gall"),nw_bin_SC,w_min_SC,w_max_SC,nt_bin_SC,t_min_SC,t_max_SC);



  TH1F *htof_sc_tsc0 = new TH1F("htof_sc_tsc0","htof_sc_tsc0",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_sc_tsc1 = new TH1F("htof_sc_tsc1","htof_sc_tsc1",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_sc_tsc0_g = new TH1F("htof_sc_tsc0_g","htof_sc_tsc0_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_sc_tsc1_g = new TH1F("htof_sc_tsc1_g","htof_sc_tsc1_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofs_sc_tsc0_g = new TH1F("htofs_sc_tsc0_g","htofs_sc_tsc0_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofs_sc_tsc1_g = new TH1F("htofs_sc_tsc1_g","htofs_sc_tsc1_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofs_sc_tsc0_lg = new TH1F("htofs_sc_tsc0_lg","htofs_sc_tsc0_lg",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofs_sc_tsc1_lg = new TH1F("htofs_sc_tsc1_lg","htofs_sc_tsc1_lg",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_sc_tsc2 = new TH1F("htof_sc_tsc2","htof_sc_tsc2",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_sc_tsc2_g = new TH1F("htof_sc_tsc2_g","htof_sc_tsc2_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_sc_tsc2_lg = new TH1F("htof_sc_tsc2_lg","htof_sc_tsc2_gl",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_tsc01 = new TH1F("htof_tsc01","htof_tsc01",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_tsc01_g = new TH1F("htof_tsc01_g","htof_tsc01_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofs_tsc01_g = new TH1F("htofs_tsc01_g","htofs_tsc01_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofs_tsc01_lg = new TH1F("htofs_tsc01_lg","htofs_tsc01_lg",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_tsc02 = new TH1F("htof_tsc02","htof_tsc02",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_tsc12 = new TH1F("htof_tsc12","htof_tsc12",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_tsc02_g = new TH1F("htof_tsc02_g","htof_tsc02_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_tsc02_lg = new TH1F("htof_tsc02_lg","htof_tsc02_lg",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofs_tsc02_g = new TH1F("htofs_tsc02_g","htofs_tsc02_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofs_tsc02_lg = new TH1F("htofs_tsc02_lg","htofs_tsc02_lg",ntof_bin_SC, tof_min_SC,tof_max_SC);

  TH1F *htofd_sc_tsc0_g = new TH1F("htofd_sc_tsc0_g","htofd_sc_tsc0_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofd_sc_tsc1_g = new TH1F("htofd_sc_tsc1_g","htofd_sc_tsc1_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofd_tsc01_g = new TH1F("htofd_tsc01_g","htofd_tsc01_g",ntof_bin_SC, tof_min_SC,tof_max_SC);

  TH1F *htofdw_sc_tsc0_g = new TH1F("htofdw_sc_tsc0_g","htofdw_sc_tsc0_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofdw_sc_tsc1_g = new TH1F("htofdw_sc_tsc1_g","htofdw_sc_tsc1_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofdw_tsc01_g = new TH1F("htofdw_tsc01_g","htofdw_tsc01_g",ntof_bin_SC, tof_min_SC,tof_max_SC);


  TH1F *htof_tsc12_g = new TH1F("htof_tsc12_g","htof_tsc12_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htof_tsc12_lg = new TH1F("htof_tsc12_lg","htof_tsc12_lg",ntof_bin_SC, tof_min_SC,tof_max_SC);

  TH1F *htofs_tsc12_g = new TH1F("htofs_tsc12_g","htofs_tsc12_g",ntof_bin_SC, tof_min_SC,tof_max_SC);
  TH1F *htofs_tsc12_lg = new TH1F("htofs_tsc12_lg","htofs_tsc12_lg",ntof_bin_SC, tof_min_SC,tof_max_SC);

  Int_t ngood_trig = 0;
  Int_t ngood_rpc_top = 0;
  Int_t ngood_rpc_top0 = 0;
  Int_t ngood_rpc_top2 = 0;
  Int_t ngood_rpc_topall = 0;
  Int_t ngood_rpc_bot = 0;
  Int_t ngood_rpc_bot0 = 0;
  Int_t ngood_rpc_bot2 = 0;
  Int_t ngood_rpc_botall = 0;
  Int_t ngood_c1 = 0;
  Int_t ngood_c2 = 0;
  Int_t ngood_s1 = 0;
  Int_t ngood_s2 = 0;
  Int_t ngood_s3 = 0;
  Int_t ngood_s4 = 0;
  Int_t ngood_trig_c1 = 0;
  Int_t ngood_trig_c2 = 0;
  Int_t ngood_trig_c1_c2 = 0;
  Int_t ngood_c1_trig_c2 = 0;
  Int_t ngood_c2_trig_c1 = 0;
  Int_t ngood_rpc_top_trig_c1 = 0;
  Int_t ngood_rpc_bot_trig_c1 = 0;
  Int_t ngood_rpc_top_trig_c2 = 0;
  Int_t ngood_rpc_bot_trig_c2 = 0;
  Int_t ngood_rpc_top_trig_c1_c2 = 0;
  Int_t ngood_rpc_bot_trig_c1_c2 = 0;

  const Int_t rpc_strip_test = 3;
  const Int_t rpc_strip_test0 = 2;
  const Int_t rpc_strip_test2 = 4;

  Int_t nentries = (Int_t)htree->GetEntries();
  for (Int_t i=0; i<nentries; i++) {
    if ((i%10000) == 0) {
      cout << "event " << i << endl;
    }

    htree->GetEntry(i);

    const Int_t max_nhh = 100;
    //TDC data
    //SC1,2
    Int_t nh_SC[2]={0};//test counter (SC) above TSC
    Double_t lt_SC[2][max_nhh]={0};//[iside]
    Double_t tt_SC[2][max_nhh]={0};//[iside]
    Double_t lts_SC[2][max_nhh]={0};//[iside]
    //TSC1,2,3 (top/middle/bottom)
    Int_t nh_TSC[3][2]={0};//[itsc][iside]
    Double_t lt_TSC[3][2][max_nhh]={0};//test counter (SC) above TSC
    Double_t tt_TSC[3][2][max_nhh]={0};//test counter (SC) above TSC
    Double_t lts_TSC[3][2][max_nhh]={0};//test counter (SC) above TSC


    Bool_t good_trig = false;
    //select good trigger signals
    if ((hnh_trig1[0]>0)&&(hnh_trig1[1]>0)&&(hnh_trig1[2]>0)&&(hnh_trig1[3]>0)) {
      if ((trig_tmin<=hlt_trig1[0][0])&&(hlt_trig1[0][0]<=trig_tmax)
	  &&(trig_tmin<=hlt_trig1[1][0])&&(hlt_trig1[1][0]<=trig_tmax)
	  &&(trig_tmin<=hlt_trig1[2][0])&&(hlt_trig1[2][0]<=trig_tmax)
	  &&(trig_tmin<=hlt_trig1[3][0])&&(hlt_trig1[3][0]<=trig_tmax)) {
	good_trig = true;
      }
    }
    if (good_trig) ngood_trig++;

    Bool_t good_c1 = false;
    if (hnh_trig1[4]>0) {
	if ((c1_tmin<=hlt_trig1[4][0])&&(hlt_trig1[4][0]<=c1_tmax)) {
	good_c1 = true;
      }
    }
    if (good_c1) ngood_c1++;

    Bool_t good_c2 = false;
    if (hnh_trig1[5]>0) {
	if ((c2_tmin<=hlt_trig1[5][0])&&(hlt_trig1[5][0]<=c2_tmax)) {
	good_c2 = true;
      }
    }
    if (good_c2) ngood_c2++;


    if ((good_trig)&&(good_c2)) {
      ngood_trig_c2++;
      if (good_c1) {
	ngood_c1_trig_c2++;
      }
    }
    if ((good_trig)&&(good_c1)) {
      ngood_trig_c1++;
      if (good_c2) {
	ngood_c2_trig_c1++;
      }
    }

    Double_t dt_s12=0;
    Double_t dt_s34=0;
    Double_t mt_s12=0;
    Double_t mt_s34=0;
    Double_t mts_s12=0;//slewing
    Double_t mts_s34=0;//slewing
    Bool_t good_dt_s12 = false;
    Bool_t good_dt_s34 = false;

    Bool_t good_wt_s12 = false;
    Bool_t good_wt_s34 = false;

    Bool_t good_s1 = false;
    Double_t lt_s1 = 0;
    Double_t wt_s1 = 0;

    if (hnh_trig2[0]>0) {
      if ((s1_tmin<=hlt_trig2[0][0])&&(hlt_trig2[0][0]<=s1_tmax)) {
	lt_s1 = hlt_trig2[0][0];
	wt_s1 = hlt_trig2[0][0]-htt_trig2[0][0];
	good_s1 = true;
      }
    }
    if (good_s1) {
      ngood_s1++;
      hlt_s1->Fill(lt_s1);
      hlt_wt_s1->Fill(wt_s1,lt_s1);
    }

    Bool_t good_s2 = false;
    Double_t lt_s2 = 0;
    Double_t wt_s2 = 0;
    if (hnh_trig2[1]>0) {
	if ((s2_tmin<=hlt_trig2[1][0])&&(hlt_trig2[1][0]<=s2_tmax)) {
	lt_s2 = hlt_trig2[1][0];
	wt_s2 = hlt_trig2[1][0]-htt_trig2[1][0];
	//	lts_s2 = lt_s2 - (wt_s2-32.5)*0.0129646;
	good_s2 = true;
      }
    }
    if (good_s2) {
      ngood_s2++;
      hlt_s2->Fill(lt_s2);
      hlt_wt_s2->Fill(wt_s2,lt_s2);
    }

    Bool_t good_s3 = false;
    Double_t lt_s3 = 0;
    Double_t wt_s3 = 0;
    if (hnh_trig2[2]>0) {
      if ((s3_tmin<=hlt_trig2[2][0])&&(hlt_trig2[2][0]<=s3_tmax)) {
	lt_s3 = hlt_trig2[2][0];
	wt_s3 = hlt_trig2[2][0]-htt_trig2[2][0];
	good_s3 = true;
      }
    }
    if (good_s3) {
      ngood_s3++;
      hlt_s3->Fill(lt_s3);
      hlt_wt_s3->Fill(wt_s3,lt_s3);
    }

    Bool_t good_s4 = false;
    Double_t lt_s4 = 0;
    Double_t wt_s4 = 0;
    if (hnh_trig2[3]>0) {
	if ((s4_tmin<=hlt_trig2[3][0])&&(hlt_trig2[3][0]<=s4_tmax)) {
	lt_s4 = hlt_trig2[3][0];
	wt_s4 = hlt_trig2[3][0]-htt_trig2[3][0];
	good_s4 = true;
      }
    }
    if (good_s4) {
      ngood_s4++;
      hlt_s4->Fill(lt_s4);
      hlt_wt_s4->Fill(wt_s4,lt_s4);
    }

    if (good_s1&&good_s2) {
      mt_s12 = (lt_s1+lt_s2)/2.;
      //      mts_s12 = mt_s12 - ((wt_s1+wt_s2)/2.-32.)*0.00184045;
      //      mts_s12 = mt_s12 - ((wt_s1+wt_s2)/2.-32.)*0.001267;
      mts_s12 = mt_s12 - ((wt_s1+wt_s2)/2.-32.)*0.00111763;

      hmt_s12->Fill((lt_s1+lt_s2)/2.);
      hmt_wt_s12->Fill((wt_s1+wt_s2)/2.,(lt_s1+lt_s2)/2.);
      if (good_c1&&good_c2) {
	hmt_wt_s12_c12->Fill((wt_s1+wt_s2)/2.,(lt_s1+lt_s2)/2.);
      }
      hdt_s12->Fill(lt_s1-lt_s2);
      dt_s12 = lt_s1-lt_s2;
      if (fabs(dt_s12-s12_mean)<s12_tcut) {
	good_dt_s12 = true;
      }
      if ((s12_wtmin<=(wt_s1+wt_s2)/2.)&&((wt_s1+wt_s2)/2.<=s12_wtmax)) {
	good_wt_s12 = true;
      }
    }
    if (good_s3&&good_s4) {
      mt_s34 = (lt_s3+lt_s4)/2.;
      //      mts_s34 = mt_s34 - ((wt_s3+wt_s4)/2.-32.5)*0.0129646;
      mts_s34 = mt_s34 - ((wt_s3+wt_s4)/2.-32.5)*0.01492;

      hmt_s34->Fill((lt_s3+lt_s4)/2.);
      hmt_wt_s34->Fill((wt_s3+wt_s4)/2.,(lt_s3+lt_s4)/2.);
      if (good_c1&&good_c2) {
	hmt_wt_s34_c12->Fill((wt_s3+wt_s4)/2.,(lt_s3+lt_s4)/2.);
      }
      hdt_s34->Fill(lt_s3-lt_s4);
      dt_s34 = lt_s3-lt_s4;
      if (fabs(dt_s34-s34_mean)<s34_tcut) {
	good_dt_s34 = true;
      }
      if ((s34_wtmin<=(wt_s3+wt_s4)/2.)&&((wt_s3+wt_s4)/2.<=s34_wtmax)) {
	good_wt_s34 = true;
      }
    }


    //calc RPC efficiency
    Bool_t good_rpc_top = false;
    Bool_t good_rpc_top0 = false;
    Bool_t good_rpc_top2 = false;
    Bool_t good_rpc_topall = false;
    Bool_t good_rpc_bot = false;
    Bool_t good_rpc_bot0 = false;
    Bool_t good_rpc_bot2 = false;
    Bool_t good_rpc_botall = false;

    Double_t dt_r12=0;
    Double_t mt_r12=0;
    Double_t mts_r12=0;//slew
    Bool_t good_dt_r12 = false;
    Bool_t good_wt_r12 = false;
    
    Double_t lt_r1 = 0;
    Double_t lt_r2 = 0;

    Double_t wt_r1 = 0;
    Double_t wt_r2 = 0;

    if (hnh_rpc1[rpc_strip_test][1]>0) {//top
      if ((rpc_tmin<=hlt_rpc1[rpc_strip_test][1][0])&&(hlt_rpc1[rpc_strip_test][1][0]<rpc_tmax)) {
	lt_r1 = hlt_rpc1[rpc_strip_test][1][0];
	wt_r1 = hlt_rpc1[rpc_strip_test][1][0]-htt_rpc1[rpc_strip_test][1][0];
	good_rpc_top = true;
	good_rpc_topall = true;
	hlt_r1->Fill(lt_r1);
	hlt_wt_r1->Fill(wt_r1,lt_r1);
      }
    }
    if (hnh_rpc1[rpc_strip_test0][1]>0) {//top
      if ((rpc_tmin<=hlt_rpc1[rpc_strip_test0][1][0])&&(hlt_rpc1[rpc_strip_test0][1][0]<rpc_tmax)) {
	good_rpc_top0 = true;
	good_rpc_topall = true;
      }
    }
    if (hnh_rpc1[rpc_strip_test2][1]>0) {//top
      if ((rpc_tmin<=hlt_rpc1[rpc_strip_test2][1][0])&&(hlt_rpc1[rpc_strip_test2][1][0]<rpc_tmax)) {
	good_rpc_top2 = true;
	good_rpc_topall = true;
      }
    }
    if (hnh_rpc1[rpc_strip_test][0]>0) {//bot
      if ((rpc_tmin<=hlt_rpc1[rpc_strip_test][0][0])&&(hlt_rpc1[rpc_strip_test][0][0]<rpc_tmax)) {
	lt_r2 = hlt_rpc1[rpc_strip_test][0][0];
	wt_r2 = hlt_rpc1[rpc_strip_test][0][0]-htt_rpc1[rpc_strip_test][0][0];
	good_rpc_bot = true;
	good_rpc_botall = true;
	hlt_r2->Fill(lt_r2);
	hlt_wt_r2->Fill(wt_r2,lt_r2);
      }
    }
    if (hnh_rpc1[rpc_strip_test0][0]>0) {//bot
      if ((rpc_tmin<=hlt_rpc1[rpc_strip_test0][0][0])&&(hlt_rpc1[rpc_strip_test0][0][0]<rpc_tmax)) {
	good_rpc_bot0 = true;
	good_rpc_botall = true;
      }
    }
    if (hnh_rpc1[rpc_strip_test2][0]>0) {//bot
      if ((rpc_tmin<=hlt_rpc1[rpc_strip_test2][0][0])&&(hlt_rpc1[rpc_strip_test2][0][0]<rpc_tmax)) {
	good_rpc_bot2 = true;
	good_rpc_botall = true;
      }
    }
    if (good_rpc_top&&good_rpc_bot) {
      mt_r12 = (lt_r1+lt_r1)/2.;
      //      mts_r12 = mt_r12 - ((wt_r1+wt_r2)/2.-4.5)*0.0636609;
      //      mts_r12 = mt_r12 - ((wt_r1+wt_r2)/2.-4.5)*0.06501;
      mts_r12 = mt_r12 - ((wt_r1+wt_r2)/2.-4.5)*0.0126831;

      hmt_r12->Fill((lt_r1+lt_r2)/2.);
      hmt_wt_r12->Fill((wt_r1+wt_r2)/2.,(lt_r1+lt_r2)/2.);
      if (good_c1&&good_c2) {
	hmt_wt_r12_c12->Fill((wt_r1+wt_r2)/2.,(lt_r1+lt_r2)/2.);
      }
      hdt_r12->Fill(lt_r1-lt_r2);

      if ((r12_wtmin<=(wt_r1+wt_r2)/2.)&&((wt_r1+wt_r2)/2.<=r12_wtmax)) {
	good_wt_r12 = true;
      }

    }

    if ((good_rpc_top&&good_rpc_bot)&&
	(good_s1&&good_s2)&&
	(good_s3&&good_s4)) {
      Double_t tof_r12_s12 = (lt_r1+lt_r2)/2.-(lt_s1+lt_s2)/2.;
      Double_t tof_r12_s34 = (lt_r1+lt_r2)/2.-(lt_s3+lt_s4)/2.;
      Double_t tof_s12_s34 = (lt_s1+lt_s2)/2.-(lt_s3+lt_s4)/2.;
      //slewing
      Double_t tofs_r12_s12 = mts_r12-mts_s12;
      Double_t tofs_r12_s34 = mts_r12-mts_s34;
      Double_t tofs_s12_s34 = mts_s12-mts_s34;
      htof_r12_s12->Fill(tof_r12_s12);
      htof_r12_s34->Fill(tof_r12_s34);
      htof_s12_s34->Fill(tof_s12_s34);

      if (good_dt_s12&&good_dt_s34) {
	htof_r12_s12_sl->Fill(tof_r12_s12);
	htof_r12_s34_sl->Fill(tof_r12_s34);
	htof_s12_s34_sl->Fill(tof_s12_s34);
	if (good_wt_r12&&good_wt_s12&&good_wt_s34) {
	  htof_r12_s12_slg->Fill(tof_r12_s12);
	  htof_r12_s34_slg->Fill(tof_r12_s34);
	  htof_s12_s34_slg->Fill(tof_s12_s34);
	  //slew
	  htofs_r12_s12_slg->Fill(tofs_r12_s12);
	  htofs_r12_s34_slg->Fill(tofs_r12_s34);
	  htofs_s12_s34_slg->Fill(tofs_s12_s34);
	}
      }
      if (good_c1&&good_c2) {
	htof_r12_s12_c12->Fill(tof_r12_s12);
	htof_r12_s34_c12->Fill(tof_r12_s34);
	htof_s12_s34_c12->Fill(tof_s12_s34);
	if (good_wt_r12&&good_wt_s12&&good_wt_s34) {
	  htof_r12_s12_c12g->Fill(tof_r12_s12);
	  htof_r12_s34_c12g->Fill(tof_r12_s34);
	  htof_s12_s34_c12g->Fill(tof_s12_s34);
	  //slew
	  htofs_r12_s12_c12g->Fill(tofs_r12_s12);
	  htofs_r12_s34_c12g->Fill(tofs_r12_s34);
	  htofs_s12_s34_c12g->Fill(tofs_s12_s34);
	}
      }
      if (good_c1) {
	htof_r12_s12_c1->Fill(tof_r12_s12);
	htof_r12_s34_c1->Fill(tof_r12_s34);
	htof_s12_s34_c1->Fill(tof_s12_s34);
      }
      if (good_c2) {
	htof_r12_s12_c2->Fill(tof_r12_s12);
	htof_r12_s34_c2->Fill(tof_r12_s34);
	htof_s12_s34_c2->Fill(tof_s12_s34);
      }

    }
				     

    if (good_rpc_top) ngood_rpc_top++;
    if (good_rpc_topall) ngood_rpc_topall++;
    if (good_rpc_top0) ngood_rpc_top0++;
    if (good_rpc_top2) ngood_rpc_top2++;

    if (good_rpc_bot) ngood_rpc_bot++;
    if (good_rpc_botall) ngood_rpc_botall++;
    if (good_rpc_bot0) ngood_rpc_bot0++;
    if (good_rpc_bot2) ngood_rpc_bot2++;

    if ((good_trig)&&(good_c1)&&(good_c2)) {
      ngood_trig_c1_c2++;
      if (good_rpc_top) {
	ngood_rpc_top_trig_c1_c2++;
      }
      if (good_rpc_bot) {
	ngood_rpc_bot_trig_c1_c2++;
      }
    }

    if ((good_trig)&&(good_c1)) {
      if (good_rpc_top) {
	ngood_rpc_top_trig_c1++;
      }
      if (good_rpc_bot) {
	ngood_rpc_bot_trig_c1++;
      }
    }
    if ((good_trig)&&(good_c2)) {
      if (good_rpc_top) {
	ngood_rpc_top_trig_c2++;
      }
      if (good_rpc_bot) {
	ngood_rpc_bot_trig_c2++;
      }
    }


      cout << "ev: good_trig, good_rpc_top, good_rpc_bot, good_c1, good_c2 = " << i << " : " << good_trig << " " << good_rpc_top << " " << good_rpc_bot << " " << good_c1 << " " << good_c2 << endl;
  cout << "ev: ngood_trig, ngood_rpc_top, ngood_rpc_bot, ngood_c1, ngood_c2 = " << i << " : " << ngood_trig << " " << ngood_rpc_top << " " << ngood_rpc_bot << " " << ngood_c1 << " " << ngood_c2 << endl;


    for (Int_t itb=0;itb<2;itb++) {
      if (run<=20051) {
	nh_SC[itb]=hnh_rpc1[itb][1];
      } else if (run<=20120) {
	nh_SC[itb]=hnh_rpc1[0][itb];
      } else {
	if (itb==1) {
	  nh_SC[itb]=hnh_rpc1[1][itb];
	} else {
	  nh_SC[itb]=hnh_rpc1[0][itb];
	}
      }
      for (Int_t ih=0;ih<nh_SC[itb];ih++) {
	if (run<=20051) {
	  lt_SC[itb][ih] = hlt_rpc1[itb][1][ih]-t0_global;
	  tt_SC[itb][ih] = htt_rpc1[itb][1][ih]-t0_global;
	} else if (run<=20120) {
	  lt_SC[itb][ih] = hlt_rpc1[0][itb][ih]-t0_global;
	  tt_SC[itb][ih] = htt_rpc1[0][itb][ih]-t0_global;
	} else {
	  if (itb==1) {
	    lt_SC[itb][ih] = hlt_rpc1[1][itb][ih]-t0_global;
	    tt_SC[itb][ih] = htt_rpc1[1][itb][ih]-t0_global;
	  } else {
	    lt_SC[itb][ih] = hlt_rpc1[0][itb][ih]-t0_global;
	    tt_SC[itb][ih] = htt_rpc1[0][itb][ih]-t0_global;
	  }
	}
      }
    }

    for (Int_t itsc=0;itsc<3;itsc++) {
      for (Int_t itb=0;itb<2;itb++) {
	if (run<=20051) {
	  nh_TSC[itsc][itb]=hnh_rpc2[itsc*2+itb][1];
	  /*	} else if ((20135<=run)&&(run<=20136)) {
	  if (itsc==0) {
	    nh_TSC[itsc][itb]=hnh_rpc2[itsc/2][itb];
	  } else if (itsc==2) {
	    if (itb==1) {
	      nh_TSC[itsc][itb]=hnh_rpc1[2][itb];
	    } else {
	      nh_TSC[itsc][itb]=hnh_rpc1[1][itb];
	    }
	  } else {//itsc==1
	    nh_TSC[itsc][itb]=hnh_tsc[0][itb];
	    }*/

	} else if ((20137<=run)&&(run<=20148)) {
	  if ((itsc==0)||(itsc==2)) {
	    nh_TSC[itsc][itb]=hnh_rpc2[itsc/2][itb];
	  } else {
	    nh_TSC[itsc][itb]=hnh_tsc[0][itb];
	  }
	} else if (run<=20206) {
	  if (itsc==0) {
	    nh_TSC[itsc][itb]=hnh_rpc2[itsc/2][itb];
	  } else if (itsc==2) {
	    if (itb==1) {
	      nh_TSC[itsc][itb]=hnh_rpc1[2][itb];
	    } else {
	      nh_TSC[itsc][itb]=hnh_rpc1[1][itb];
	    }
	  } else {//itsc==1
	    nh_TSC[itsc][itb]=hnh_tsc[0][itb];
	  }

	} else {//>=20207
	  if (itsc==0) {
	    nh_TSC[itsc][itb]=hnh_rpc2[0][itb];
	  } else if (itsc==1) {
	    nh_TSC[itsc][itb]=hnh_rpc2[1][itb];
	  } else {//itsc==2
	    nh_TSC[itsc][itb]=hnh_tsc[0][itb];
	  }

	}
	for (Int_t ih=0;ih<nh_TSC[itsc][itb];ih++) {
	  if (run<=20051) {
	    lt_TSC[itsc][itb][ih] = hlt_rpc2[itsc*2+itb][1][ih]-t0_global;
	    tt_TSC[itsc][itb][ih] = htt_rpc2[itsc*2+itb][1][ih]-t0_global;

	  } else if ((20137<=run)&&(run<=20148)) {
	    if ((itsc==0)||(itsc==2)) {
	      lt_TSC[itsc][itb][ih] = hlt_rpc2[itsc/2][itb][ih]-t0_global;
	      tt_TSC[itsc][itb][ih] = htt_rpc2[itsc/2][itb][ih]-t0_global;
	    } else {//itsc==1
	      lt_TSC[itsc][itb][ih] = hlt_tsc[0][itb][ih]-t0_global;
	      tt_TSC[itsc][itb][ih] = htt_tsc[0][itb][ih]-t0_global;
	    }
	  } else if (run<=20206) {//run>=20149
	    if (itsc==0) {
	      lt_TSC[itsc][itb][ih] = hlt_rpc2[0][itb][ih]-t0_global;
	      tt_TSC[itsc][itb][ih] = htt_rpc2[0][itb][ih]-t0_global;
	    } else if (itsc==2) {//itsc==2
	      if (itb==1) {
		lt_TSC[itsc][itb][ih] = hlt_rpc1[2][itb][ih]-t0_global;
		tt_TSC[itsc][itb][ih] = htt_rpc1[2][itb][ih]-t0_global;
	      } else {
		lt_TSC[itsc][itb][ih] = hlt_rpc1[1][itb][ih]-t0_global;
		tt_TSC[itsc][itb][ih] = htt_rpc1[1][itb][ih]-t0_global;
	      }
	    } else {//itsc==1
	      lt_TSC[itsc][itb][ih] = hlt_tsc[0][itb][ih]-t0_global;
	      tt_TSC[itsc][itb][ih] = htt_tsc[0][itb][ih]-t0_global;
	    }
	  } else {//>=20207
	    if (itsc==0) {
	      lt_TSC[itsc][itb][ih] = hlt_rpc2[0][itb][ih]-t0_global;
	      tt_TSC[itsc][itb][ih] = htt_rpc2[0][itb][ih]-t0_global;
	    } else if (itsc==1) {
	      lt_TSC[itsc][itb][ih] = hlt_rpc2[1][itb][ih]-t0_global;
	      tt_TSC[itsc][itb][ih] = htt_rpc2[1][itb][ih]-t0_global;
	    } else {//itsc==2
	      lt_TSC[itsc][itb][ih] = hlt_tsc[0][itb][ih]-t0_global;
	      tt_TSC[itsc][itb][ih] = htt_tsc[0][itb][ih]-t0_global;
	    }
	  }
	}
      }
    }

    Double_t dt_SC[max_nhh]={0};
    Double_t dts_SC[max_nhh]={0};
    Double_t mt_SC[max_nhh]={0};
    Double_t mts_SC[max_nhh]={0};
    Double_t mtd_SC[max_nhh]={0};
    Double_t wm_SC[max_nhh]={0};
    Double_t w_SC[2][max_nhh]={0};
    Int_t nh_SC_tb = nh_SC[0];
    if (nh_SC[1]<nh_SC[0]) {
      nh_SC_tb = nh_SC[1];
    }
    for (Int_t itb=0;itb<2;itb++) {
      for (Int_t ih=0;ih<nh_SC[itb];ih++) {
	w_SC[itb][ih] = lt_SC[itb][ih]-tt_SC[itb][ih];
	if (itb==1) {
	  lts_SC[itb][ih] = lt_SC[itb][ih] + lt_sc_slew[1]*(lt_sc_slew_w0-w_SC[itb][ih]);
	} else {
	  lts_SC[itb][ih] = lt_SC[itb][ih] + lb_sc_slew[1]*(lb_sc_slew_w0-w_SC[itb][ih]);
	}
      }
    }
    for (Int_t ih=0;ih<nh_SC_tb;ih++) {
      dt_SC[ih] = lt_SC[1][ih]-lt_SC[0][ih];
      dts_SC[ih] = lts_SC[1][ih]-lts_SC[0][ih];
      mt_SC[ih] = (lt_SC[0][ih]+lt_SC[1][ih])/2.;
      mts_SC[ih] = (lts_SC[0][ih]+lts_SC[1][ih])/2.;
      mtd_SC[ih] = mts_SC[ih]-mt_sc_dtcor[1]*(dts_SC[ih]-dt0_sc_dtcor)
	-mt_sc_dtcor[2]*(dts_SC[ih]-dt0_sc_dtcor)*(dts_SC[ih]-dt0_sc_dtcor)
	-mt_sc_dtcor[3]*(dts_SC[ih]-dt0_sc_dtcor)*(dts_SC[ih]-dt0_sc_dtcor)*(dts_SC[ih]-dt0_sc_dtcor);
    }
    for (Int_t ih=0;ih<nh_SC_tb;ih++) {
      wm_SC[ih] = (w_SC[0][ih]+w_SC[1][ih])/2.;
    }

    Double_t dt_TSC[3][max_nhh]={0};
    Double_t dts_TSC[3][max_nhh]={0};
    Double_t mt_TSC[3][max_nhh]={0};
    Double_t mts_TSC[3][max_nhh]={0};
    Double_t mtd_TSC[3][max_nhh]={0};
    Double_t wm_TSC[3][max_nhh]={0};
    Double_t w_TSC[3][2][max_nhh]={0};
    Int_t nh_TSC_tb[3]={0};
    for (Int_t itsc=0;itsc<3;itsc++) {
      nh_TSC_tb[itsc] = nh_TSC[itsc][0];
      if (nh_TSC[itsc][1]<nh_TSC[itsc][0]) {
	nh_TSC_tb[itsc] = nh_TSC[itsc][1];
      }
      for (Int_t itb=0;itb<2;itb++) {
	for (Int_t ih=0;ih<nh_TSC[itsc][itb];ih++) {
	  w_TSC[itsc][itb][ih] = lt_TSC[itsc][itb][ih]-tt_TSC[itsc][itb][ih];
	  if (itb==1) {
	    if (itsc==0) {
	      lts_TSC[itsc][itb][ih] = lt_TSC[itsc][itb][ih] + lt_tsc0_slew[1]*(lt_tsc0_slew_w0-w_TSC[itsc][itb][ih]);
	    } else {
	      lts_TSC[itsc][itb][ih] = lt_TSC[itsc][itb][ih] + lt_tsc1_slew[1]*(lt_tsc1_slew_w0-w_TSC[itsc][itb][ih]);
	    }
	  } else {
	    if (itsc==0) {
	      lts_TSC[itsc][itb][ih] = lt_TSC[itsc][itb][ih] + lb_tsc0_slew[1]*(lb_tsc0_slew_w0-w_TSC[itsc][itb][ih]);
	    } else {
	      lts_TSC[itsc][itb][ih] = lt_TSC[itsc][itb][ih] + lb_tsc1_slew[1]*(lb_tsc1_slew_w0-w_TSC[itsc][itb][ih]);
	    }
	  }
	}
      }
      for (Int_t ih=0;ih<nh_TSC_tb[itsc];ih++) {
	dt_TSC[itsc][ih] = lt_TSC[itsc][1][ih]-lt_TSC[itsc][0][ih];
	dts_TSC[itsc][ih] = lts_TSC[itsc][1][ih]-lts_TSC[itsc][0][ih];
	mt_TSC[itsc][ih] = (lt_TSC[itsc][0][ih]+lt_TSC[itsc][1][ih])/2.;
	mts_TSC[itsc][ih] = (lts_TSC[itsc][0][ih]+lts_TSC[itsc][1][ih])/2.;
	if (itsc==0) {
	  mtd_TSC[itsc][ih] = mts_TSC[itsc][ih]-mt_tsc0_dtcor[1]*(dts_TSC[0][ih]-dt0_tsc0_dtcor)
	    -mt_tsc0_dtcor[2]*(dts_TSC[0][ih]-dt0_tsc0_dtcor)*(dts_TSC[0][ih]-dt0_tsc0_dtcor)
	    -mt_tsc0_dtcor[3]*(dts_TSC[0][ih]-dt0_tsc0_dtcor)*(dts_TSC[0][ih]-dt0_tsc0_dtcor)*(dts_TSC[0][ih]-dt0_tsc0_dtcor);
	} else if (itsc==1) {
	  mtd_TSC[itsc][ih] = mts_TSC[itsc][ih]-mt_tsc1_dtcor[1]*(dts_TSC[1][ih]-dt0_tsc1_dtcor)
	    -mt_tsc1_dtcor[2]*(dts_TSC[1][ih]-dt0_tsc1_dtcor)*(dts_TSC[1][ih]-dt0_tsc1_dtcor)
	    -mt_tsc1_dtcor[3]*(dts_TSC[1][ih]-dt0_tsc1_dtcor)*(dts_TSC[1][ih]-dt0_tsc1_dtcor)*(dts_TSC[1][ih]-dt0_tsc1_dtcor);
	} else {
	  mtd_TSC[itsc][ih] = mts_TSC[itsc][ih];
	}

      }

      for (Int_t ih=0;ih<nh_TSC_tb[itsc];ih++) {
	wm_TSC[itsc][ih] = (w_TSC[itsc][0][ih]+w_TSC[itsc][1][ih])/2.;
      }
    }

    //tof

    //check quality
    Bool_t sc_t_good = false;
    Bool_t sc_b_good = false;
    if ((lt_sc_min_cut<lt_SC[1][0])&&(lt_SC[1][0]<lt_sc_max_cut)
	&&(wt_sc_min_cut<(lt_SC[1][0]-tt_SC[1][0]))&&((lt_SC[1][0]-tt_SC[1][0])<wt_sc_max_cut)) {
      sc_t_good = true;
    }
    if ((lb_sc_min_cut<lt_SC[0][0])&&(lt_SC[0][0]<lb_sc_max_cut)
	&&(wb_sc_min_cut<(lt_SC[0][0]-tt_SC[0][0]))&&((lt_SC[0][0]-tt_SC[0][0])<wb_sc_max_cut)) {
      sc_b_good = true;
    }
    Bool_t tsc0_t_good = false;
    Bool_t tsc0_b_good = false;
    if ((lt_tsc0_min_cut<lt_TSC[0][1][0])&&(lt_TSC[0][1][0]<lt_tsc0_max_cut)
	&&(wt_tsc0_min_cut<(lt_TSC[0][1][0]-tt_TSC[0][1][0]))&&((lt_TSC[0][1][0]-tt_TSC[0][1][0])<wt_tsc0_max_cut)
	//	&&(lt_TSC[0][1][0]>((lt_TSC[0][1][0]-tt_TSC[0][1][0])*lt_tsc0_line_cut1+lt_tsc0_line_cut0))) {
	//	&&(lt_TSC[0][1][0]<=((lt_TSC[0][1][0]-tt_TSC[0][1][0])*lt_tsc0_line_cut1+lt_tsc0_line_cut0))) {
	){
      tsc0_t_good = true;
    }
    if ((lb_tsc0_min_cut<lt_TSC[0][0][0])&&(lt_TSC[0][0][0]<lb_tsc0_max_cut)
	&&(wb_tsc0_min_cut<(lt_TSC[0][0][0]-tt_TSC[0][0][0]))&&((lt_TSC[0][0][0]-tt_TSC[0][0][0])<wb_tsc0_max_cut)) {
      tsc0_b_good = true;
    }
    Bool_t tsc1_t_good = false;
    Bool_t tsc1_b_good = false;
    if ((lt_tsc1_min_cut<lt_TSC[1][1][0])&&(lt_TSC[1][1][0]<lt_tsc1_max_cut)
	&&(wt_tsc1_min_cut<(lt_TSC[1][1][0]-tt_TSC[1][1][0]))&&((lt_TSC[1][1][0]-tt_TSC[1][1][0])<wt_tsc1_max_cut)) {
      tsc1_t_good = true;
    }
    if ((lb_tsc1_min_cut<lt_TSC[1][0][0])&&(lt_TSC[1][0][0]<lb_tsc1_max_cut)
	&&(wb_tsc1_min_cut<(lt_TSC[1][0][0]-tt_TSC[1][0][0]))&&((lt_TSC[1][0][0]-tt_TSC[1][0][0])<wb_tsc1_max_cut)) {
      tsc1_b_good = true;
    }

    Bool_t all_good = false;
    if ((sc_t_good)&&(sc_b_good)&&(tsc0_t_good)&&(tsc0_b_good)&&(tsc1_t_good)&&(tsc1_b_good)) {
      all_good = true;
    }

    //dt cut
    Bool_t dt_sc_good = false;
    if (fabs(dt_SC[0]-dt_sc_mean)<dt_cut_factor*dt_sc_sigma) {
      dt_sc_good = true;
    }
    Bool_t dt_tsc0_good = false;
    if (fabs(dt_TSC[0][0]-dt_tsc0_mean)<dt_cut_factor*dt_tsc0_sigma) {
      dt_tsc0_good = true;
    }
    Bool_t dt_tsc1_good = false;
    if (fabs(dt_TSC[1][0]-dt_tsc1_mean)<dt_cut_factor*dt_tsc1_sigma) {
      dt_tsc1_good = true;
    }
    Bool_t dt_good = false;
    if ((dt_sc_good)&&(dt_tsc0_good)&&(dt_tsc1_good)) {
      dt_good = true;
    }

    Bool_t sc_t_lgood = false;
    Bool_t sc_b_lgood = false;
    if ((lt_sc_min_lcut<lt_SC[1][0])&&(lt_SC[1][0]<lt_sc_max_lcut)
	&&(wt_sc_min_lcut<(lt_SC[1][0]-tt_SC[1][0]))&&((lt_SC[1][0]-tt_SC[1][0])<wt_sc_max_lcut)) {
      sc_t_lgood = true;
    }
    if ((lb_sc_min_lcut<lt_SC[0][0])&&(lt_SC[0][0]<lb_sc_max_lcut)
	&&(wb_sc_min_lcut<(lt_SC[0][0]-tt_SC[0][0]))&&((lt_SC[0][0]-tt_SC[0][0])<wb_sc_max_lcut)) {
      sc_b_lgood = true;
    }
    Bool_t tsc0_t_lgood = false;
    Bool_t tsc0_b_lgood = false;
    if ((lt_tsc0_min_lcut<lt_TSC[0][1][0])&&(lt_TSC[0][1][0]<lt_tsc0_max_lcut)
	&&(wt_tsc0_min_lcut<(lt_TSC[0][1][0]-tt_TSC[0][1][0]))&&((lt_TSC[0][1][0]-tt_TSC[0][1][0])<wt_tsc0_max_lcut)) {
      tsc0_t_lgood = true;
    }
    if ((lb_tsc0_min_lcut<lt_TSC[0][0][0])&&(lt_TSC[0][0][0]<lb_tsc0_max_lcut)
	&&(wb_tsc0_min_lcut<(lt_TSC[0][0][0]-tt_TSC[0][0][0]))&&((lt_TSC[0][0][0]-tt_TSC[0][0][0])<wb_tsc0_max_lcut)) {
      tsc0_b_lgood = true;
    }
    Bool_t tsc1_t_lgood = false;
    Bool_t tsc1_b_lgood = false;
    if ((lt_tsc1_min_lcut<lt_TSC[1][1][0])&&(lt_TSC[1][1][0]<lt_tsc1_max_lcut)
	&&(wt_tsc1_min_lcut<(lt_TSC[1][1][0]-tt_TSC[1][1][0]))&&((lt_TSC[1][1][0]-tt_TSC[1][1][0])<wt_tsc1_max_lcut)) {
      tsc1_t_lgood = true;
    }
    if ((lb_tsc1_min_lcut<lt_TSC[1][0][0])&&(lt_TSC[1][0][0]<lb_tsc1_max_lcut)
	&&(wb_tsc1_min_lcut<(lt_TSC[1][0][0]-tt_TSC[1][0][0]))&&((lt_TSC[1][0][0]-tt_TSC[1][0][0])<wb_tsc1_max_lcut)) {
      tsc1_b_lgood = true;
    }

    Bool_t all_lgood = false;
    if ((sc_t_lgood)&&(sc_b_lgood)&&(tsc0_t_lgood)&&(tsc0_b_lgood)&&(tsc1_t_lgood)&&(tsc1_b_lgood)) {
      all_lgood = true;
    }

    Double_t tof_SC_TSC[3] = {-1000,-1000,-1000};
    Double_t tofs_SC_TSC[3] = {-1000,-1000,-1000};
    Double_t tofd_SC_TSC[3] = {-1000,-1000,-1000};
    Double_t tofdw_SC_TSC[3] = {-1000,-1000,-1000};
    for (Int_t itsc=0;itsc<3;itsc++) {
      if ((nh_SC_tb>0)&&(nh_TSC_tb[itsc]>0)) {
	tof_SC_TSC[itsc] = mt_SC[0]-mt_TSC[itsc][0];
	tofs_SC_TSC[itsc] = mts_SC[0]-mts_TSC[itsc][0];
	//	tofd_SC_TSC[itsc] = mtd_SC[0]-mtd_TSC[itsc][0];
	if (itsc==0) {
	  tofd_SC_TSC[itsc] = tofs_SC_TSC[itsc]-stsc0_tof_dt[1]*(dt_TSC[1][0]-stsc0_tof_dt0);
	  tofdw_SC_TSC[itsc] = tofd_SC_TSC[itsc]-stsc0_tof_wm_sc[1]*(wm_SC[0]-stsc0_tof_wm_sc_0)
	    -stsc0_tof_wm_tsc0[1]*(wm_TSC[0][0]-stsc0_tof_wm_tsc0_0);
	} else if (itsc==1) {
	  tofd_SC_TSC[itsc] = tofs_SC_TSC[itsc]-stsc1_tof_dt[1]*(dt_TSC[1][0]-stsc1_tof_dt0);
	  tofdw_SC_TSC[itsc] = tofd_SC_TSC[itsc]-stsc1_tof_wm_sc[1]*(wm_SC[0]-stsc1_tof_wm_sc_0)
	    -stsc1_tof_wm_tsc1[1]*(wm_TSC[1][0]-stsc1_tof_wm_tsc1_0);
	}
      }
    }
    Double_t tof_TSC01=-1000;
    Double_t tofs_TSC01=-1000;
    Double_t tofd_TSC01=-1000;
    Double_t tofdw_TSC01=-1000;
    Double_t tof_TSC02=-1000;
    Double_t tofs_TSC02=-1000;
    Double_t tof_TSC12=-1000;
    Double_t tofs_TSC12=-1000;
    if ((nh_TSC_tb[0]>0)&&(nh_TSC_tb[1]>0)) {
      tof_TSC01 = mt_TSC[0][0]-mt_TSC[1][0];
      tofs_TSC01 = mts_TSC[0][0]-mts_TSC[1][0];
      //      tofd_TSC01 = mtd_TSC[0][0]-mtd_TSC[1][0];
      tofd_TSC01 = tofs_TSC01-tsc01_tof_dt[1]*(dt_TSC[1][0]-tsc01_tof_dt0);
      tofdw_TSC01 = tofd_TSC01-tsc01_tof_wm_tsc0[1]*(wm_TSC[0][0]-tsc01_tof_wm_tsc0_0)
	-tsc01_tof_wm_tsc1[1]*(wm_TSC[1][0]-tsc01_tof_wm_tsc1_0);
    }
    if ((nh_TSC_tb[0]>0)&&(nh_TSC_tb[2]>0)) {
      tof_TSC02 = mt_TSC[0][0]-mt_TSC[2][0];
      tofs_TSC02 = mts_TSC[0][0]-mts_TSC[2][0];
    }
    if ((nh_TSC_tb[1]>0)&&(nh_TSC_tb[2]>0)) {
      tof_TSC12 = mt_TSC[1][0]-mt_TSC[2][0];
      tofs_TSC12 = mts_TSC[1][0]-mts_TSC[2][0];
    }

    //fill histos
    hsc_nh_tb->Fill(nh_SC_tb);
    if (nh_SC_tb>0){
      hsc_mt->Fill(mt_SC[0]);
      hsc_dt->Fill(dt_SC[0]);
      hsc_wm->Fill(wm_SC[0]);
      hsc_mt_wm->Fill(wm_SC[0],mt_SC[0]);
      hsc_dt_wm->Fill(dt_SC[0],wm_SC[0]);
      hsc_dt_w0->Fill(dt_SC[0],w_SC[0][0]);
      hsc_dt_w1->Fill(dt_SC[0],w_SC[1][0]);

      hsc_dt_mt->Fill(dt_SC[0],mt_SC[0]);
      hsc_dts_mts->Fill(dts_SC[0],mts_SC[0]);
      hsc_dts_mtd->Fill(dts_SC[0],mtd_SC[0]);

      if (all_good) {
	//      if (sc_tsc1_good) {
	hsc_mt_g->Fill(mt_SC[0]);
	hsc_dt_g->Fill(mt_SC[0]);
      }
    }
    for (Int_t itb=0;itb<2;itb++) {
      hsc_nh[itb]->Fill(nh_SC[itb]);
      if (nh_SC[itb]>0) {
	hsc_lt[itb]->Fill(lt_SC[itb][0]);
	hsc_tt[itb]->Fill(tt_SC[itb][0]);
	hsc_w[itb]->Fill(w_SC[itb][0]);
      }
    }
    hsc_lt_wb->Fill(w_SC[0][0],lt_SC[0][0]);
    hsc_lt_wt->Fill(w_SC[1][0],lt_SC[1][0]);
    if ((tsc0_t_good)&&(tsc0_b_good)&&(tsc1_t_good)&&(tsc1_b_good)) {
      hsc_lt_wb_g->Fill(w_SC[0][0],lt_SC[0][0]);
      hsc_lt_wt_g->Fill(w_SC[1][0],lt_SC[1][0]);

      hsc_lts_wb_g->Fill(w_SC[0][0],lts_SC[0][0]);
      hsc_lts_wt_g->Fill(w_SC[1][0],lts_SC[1][0]);
    }
    if (all_good) {
      hsc_lt_wb_gall->Fill(w_SC[0][0],lt_SC[0][0]);
      hsc_lt_wt_gall->Fill(w_SC[1][0],lt_SC[1][0]);

      hsc_lts_wb_gall->Fill(w_SC[0][0],lts_SC[0][0]);
      hsc_lts_wt_gall->Fill(w_SC[1][0],lts_SC[1][0]);
    }

    for (Int_t itsc=0;itsc<3;itsc++) {
      htsc_nh_tb[itsc]->Fill(nh_TSC_tb[itsc]);
      if (nh_TSC_tb[itsc]>0){
	htsc_mt[itsc]->Fill(mt_TSC[itsc][0]);
	htsc_dt[itsc]->Fill(dt_TSC[itsc][0]);
	if (itsc==0) {
	  htsc0_dt_mt->Fill(dt_TSC[0][0],mt_TSC[0][0]);
	  htsc0_dts_mts->Fill(dts_TSC[0][0],mts_TSC[0][0]);
	  htsc0_dts_mtd->Fill(dts_TSC[0][0],mtd_TSC[0][0]);
	} else if (itsc==1) {
	  htsc1_dt_mt->Fill(dt_TSC[1][0],mt_TSC[1][0]);
	  htsc1_dts_mts->Fill(dts_TSC[1][0],mts_TSC[1][0]);
	  htsc1_dts_mtd->Fill(dts_TSC[1][0],mtd_TSC[1][0]);
	}
	if ((itsc<=1)&&(all_good)) {
	  htsc_mt_g[itsc]->Fill(mt_TSC[itsc][0]);
	  htsc_dt_g[itsc]->Fill(dt_TSC[itsc][0]);
	}
	htsc_wm[itsc]->Fill(wm_TSC[itsc][0]);
	htsc_mt_wm[itsc]->Fill(wm_TSC[itsc][0],mt_TSC[itsc][0]);
	htsc_dt_wm[itsc]->Fill(dt_TSC[itsc][0],wm_TSC[itsc][0]);
	htsc_dt_w0[itsc]->Fill(dt_TSC[itsc][0],w_TSC[itsc][0][0]);
	htsc_dt_w1[itsc]->Fill(dt_TSC[itsc][0],w_TSC[itsc][1][0]);
      }
      for (Int_t itb=0;itb<2;itb++) {
	htsc_nh[itsc][itb]->Fill(nh_TSC[itsc][itb]);
	if (nh_TSC[itsc][itb]>0) {
	  htsc_lt[itsc][itb]->Fill(lt_TSC[itsc][itb][0]);
	  htsc_tt[itsc][itb]->Fill(tt_TSC[itsc][itb][0]);
	  htsc_w[itsc][itb]->Fill(w_TSC[itsc][itb][0]);
	}
      }
    }
  

    htsc0_lt_wb->Fill(w_TSC[0][0][0],lt_TSC[0][0][0]);
    htsc0_lt_wt->Fill(w_TSC[0][1][0],lt_TSC[0][1][0]);
    if ((sc_t_good)&&(sc_b_good)&&(tsc1_t_good)&&(tsc1_b_good)) {
      htsc0_lt_wb_g->Fill(w_TSC[0][0][0],lt_TSC[0][0][0]);
      htsc0_lt_wt_g->Fill(w_TSC[0][1][0],lt_TSC[0][1][0]);

      htsc0_lts_wb_g->Fill(w_TSC[0][0][0],lts_TSC[0][0][0]);
      htsc0_lts_wt_g->Fill(w_TSC[0][1][0],lts_TSC[0][1][0]);
    }
    if (all_good) {
      htsc0_lt_wb_gall->Fill(w_TSC[0][0][0],lt_TSC[0][0][0]);
      htsc0_lt_wt_gall->Fill(w_TSC[0][1][0],lt_TSC[0][1][0]);

      htsc0_lts_wb_gall->Fill(w_TSC[0][0][0],lts_TSC[0][0][0]);
      htsc0_lts_wt_gall->Fill(w_TSC[0][1][0],lts_TSC[0][1][0]);
    }

    htsc1_lt_wb->Fill(w_TSC[1][0][0],lt_TSC[1][0][0]);
    htsc1_lt_wt->Fill(w_TSC[1][1][0],lt_TSC[1][1][0]);
    if ((sc_t_good)&&(sc_b_good)&&(tsc0_t_good)&&(tsc0_b_good)) {
      htsc1_lt_wb_g->Fill(w_TSC[1][0][0],lt_TSC[1][0][0]);
      htsc1_lt_wt_g->Fill(w_TSC[1][1][0],lt_TSC[1][1][0]);

      htsc1_lts_wb_g->Fill(w_TSC[1][0][0],lts_TSC[1][0][0]);
      htsc1_lts_wt_g->Fill(w_TSC[1][1][0],lts_TSC[1][1][0]);
    }
    if (all_good) {
      htsc1_lt_wb_gall->Fill(w_TSC[1][0][0],lt_TSC[1][0][0]);
      htsc1_lt_wt_gall->Fill(w_TSC[1][1][0],lt_TSC[1][1][0]);

      htsc1_lts_wb_gall->Fill(w_TSC[1][0][0],lts_TSC[1][0][0]);
      htsc1_lts_wt_gall->Fill(w_TSC[1][1][0],lts_TSC[1][1][0]);
    }

    htsc2_lt_wb->Fill(w_TSC[2][0][0],lt_TSC[2][0][0]);
    htsc2_lt_wt->Fill(w_TSC[2][1][0],lt_TSC[2][1][0]);
    if ((sc_t_good)&&(sc_b_good)&&(tsc0_t_good)&&(tsc0_b_good)) {
      htsc2_lt_wb_g->Fill(w_TSC[2][0][0],lt_TSC[2][0][0]);
      htsc2_lt_wt_g->Fill(w_TSC[2][1][0],lt_TSC[2][1][0]);

      htsc2_lts_wb_g->Fill(w_TSC[2][0][0],lts_TSC[2][0][0]);
      htsc2_lts_wt_g->Fill(w_TSC[2][1][0],lts_TSC[2][1][0]);
    }
    if (all_good) {
      htsc2_lt_wb_gall->Fill(w_TSC[2][0][0],lt_TSC[2][0][0]);
      htsc2_lt_wt_gall->Fill(w_TSC[2][1][0],lt_TSC[2][1][0]);

      htsc2_lts_wb_gall->Fill(w_TSC[2][0][0],lts_TSC[2][0][0]);
      htsc2_lts_wt_gall->Fill(w_TSC[2][1][0],lts_TSC[2][1][0]);
    }
    //TOF
    if (tof_SC_TSC[0]>-1000) {
      htof_sc_tsc0->Fill(tof_SC_TSC[0]);
      if (all_good) {
	htofs_stsc0_dt_tsc0->Fill(dt_TSC[0][0],tofs_SC_TSC[0]);
	htofs_stsc0_dt_tsc1->Fill(dt_TSC[1][0],tofs_SC_TSC[0]);
	htofd_stsc0_dt_tsc1->Fill(dt_TSC[1][0],tofd_SC_TSC[0]);
	htofd_stsc0_wm_sc->Fill(wm_SC[0],tofd_SC_TSC[0]);
	htofd_stsc0_wm_tsc0->Fill(wm_TSC[0][0],tofd_SC_TSC[0]);
	htofdw_stsc0_wm_sc->Fill(wm_SC[0],tofdw_SC_TSC[0]);
	htofdw_stsc0_wm_tsc0->Fill(wm_TSC[0][0],tofdw_SC_TSC[0]);
      }
      if (all_good&&dt_good) {
	//      if (sc_tsc1_good) {
	htof_sc_tsc0_g->Fill(tof_SC_TSC[0]);
	htofs_sc_tsc0_g->Fill(tofs_SC_TSC[0]);
	htofd_sc_tsc0_g->Fill(tofd_SC_TSC[0]);
	htofdw_sc_tsc0_g->Fill(tofdw_SC_TSC[0]);
      }
      if (all_lgood&&dt_good) {
	htofs_sc_tsc0_lg->Fill(tofs_SC_TSC[0]);
      }
      //      if (all_lgood) {
	hsc_tsc0_lt_lg->Fill(lt_SC[1][0],lt_TSC[0][1][0]);
	hsc_tsc0_lb_lg->Fill(lt_SC[0][0],lt_TSC[0][0][0]);
	hsc_tsc0_wt_lg->Fill(lt_SC[1][0]-tt_SC[1][0],lt_TSC[0][1][0]-tt_TSC[0][1][0]);
	hsc_tsc0_wb_lg->Fill(lt_SC[0][0]-tt_SC[0][0],lt_TSC[0][0][0]-tt_TSC[0][0][0]);

	//      }
    }
    if (tof_SC_TSC[1]>-1000) {
      htof_sc_tsc1->Fill(tof_SC_TSC[1]);
      if (all_good) {
	htofs_stsc1_dt_tsc0->Fill(dt_TSC[0][0],tofs_SC_TSC[1]);
	htofs_stsc1_dt_tsc1->Fill(dt_TSC[1][0],tofs_SC_TSC[1]);
	htofd_stsc1_dt_tsc1->Fill(dt_TSC[1][0],tofd_SC_TSC[1]);
	htofd_stsc1_wm_sc->Fill(wm_SC[0],tofd_SC_TSC[1]);
	htofd_stsc1_wm_tsc1->Fill(wm_TSC[1][0],tofd_SC_TSC[1]);
	htofdw_stsc1_wm_sc->Fill(wm_SC[0],tofdw_SC_TSC[1]);
	htofdw_stsc1_wm_tsc1->Fill(wm_TSC[1][0],tofdw_SC_TSC[1]);
      }

      if (all_good&&dt_good) {
	//      if (sc_tsc1_good) {
	htof_sc_tsc1_g->Fill(tof_SC_TSC[1]);
	htofs_sc_tsc1_g->Fill(tofs_SC_TSC[1]);
	htofd_sc_tsc1_g->Fill(tofd_SC_TSC[1]);
	htofdw_sc_tsc1_g->Fill(tofdw_SC_TSC[1]);
      }
      if (all_lgood&&dt_good) {
	htofs_sc_tsc1_lg->Fill(tofs_SC_TSC[1]);
      }
      //      if (all_lgood) {
	hsc_tsc1_lt_lg->Fill(lt_SC[1][0],lt_TSC[1][1][0]);
	hsc_tsc1_lb_lg->Fill(lt_SC[0][0],lt_TSC[1][0][0]);

	hsc_tsc1_wt_lg->Fill(lt_SC[1][0]-tt_SC[1][0],lt_TSC[1][1][0]-tt_TSC[1][1][0]);
	hsc_tsc1_wb_lg->Fill(lt_SC[0][0]-tt_SC[0][0],lt_TSC[1][0][0]-tt_TSC[1][0][0]);
	//      }

    }
    if (tof_SC_TSC[2]>-1000) {
      htof_sc_tsc2->Fill(tof_SC_TSC[2]);
    }
    if (tof_TSC01>-1000) {
      htof_tsc01->Fill(tof_TSC01);
      if (all_good) {
	htofs_tsc01_dt_tsc0->Fill(dt_TSC[0][0],tofs_TSC01);
	htofs_tsc01_dt_tsc1->Fill(dt_TSC[1][0],tofs_TSC01);
	htofd_tsc01_dt_tsc1->Fill(dt_TSC[1][0],tofd_TSC01);
	htofd_tsc01_wm_tsc0->Fill(wm_TSC[0][0],tofd_TSC01);
	htofd_tsc01_wm_tsc1->Fill(wm_TSC[1][0],tofd_TSC01);
	htofdw_tsc01_wm_tsc0->Fill(wm_TSC[0][0],tofdw_TSC01);
	htofdw_tsc01_wm_tsc1->Fill(wm_TSC[1][0],tofdw_TSC01);
      }

      if (all_good&&dt_good) {
	htof_tsc01_g->Fill(tof_TSC01);
	htofs_tsc01_g->Fill(tofs_TSC01);
	htofd_tsc01_g->Fill(tofd_TSC01);
	htofdw_tsc01_g->Fill(tofdw_TSC01);
      }
      if (all_lgood&&dt_good) {
	htofs_tsc01_lg->Fill(tofs_TSC01);
      }
      //      if (all_lgood) {
	htsc0_tsc1_lt_lg->Fill(lt_TSC[0][1][0],lt_TSC[1][1][0]);
	htsc0_tsc1_lb_lg->Fill(lt_TSC[0][0][0],lt_TSC[1][0][0]);
	htsc0_tsc1_wt_lg->Fill(lt_TSC[0][1][0]-tt_TSC[0][1][0],lt_TSC[1][1][0]-tt_TSC[1][1][0]);
	htsc0_tsc1_wb_lg->Fill(lt_TSC[0][0][0]-tt_TSC[0][0][0],lt_TSC[1][0][0]-tt_TSC[1][0][0]);

	//      }

    }
    if (tof_TSC02>-1000) {
      htof_tsc02->Fill(tof_TSC02);
      if (all_good&&dt_good) {
	htof_tsc02_g->Fill(tof_TSC02);
	htofs_tsc02_g->Fill(tofs_TSC02);
      }
      if (all_lgood&&dt_good) {
	htofs_tsc02_lg->Fill(tofs_TSC02);
      }
    }
    if (tof_TSC12>-1000) {
      htof_tsc12->Fill(tof_TSC12);
      if (all_good&&dt_good) {
	htof_tsc12_g->Fill(tof_TSC12);
	htofs_tsc12_g->Fill(tofs_TSC12);
      }
      if (all_lgood&&dt_good) {
	htofs_tsc12_lg->Fill(tofs_TSC12);
      }
    }
  }//event loop

  cout << "ngood_rpc_top, ngood_rpc_bot = " << ngood_rpc_top << " " <<  ngood_rpc_bot << endl;
  //rpc efficiency
  Double_t eff_rpc_top = (double)ngood_rpc_top/(double)ngood_trig;
  Double_t eff_rpc_bot = (double)ngood_rpc_bot/(double)ngood_trig;
  Double_t eeff_rpc_top = sqrt((double)ngood_trig-(double)ngood_rpc_top)/(double)ngood_trig;
  Double_t eeff_rpc_bot = sqrt((double)ngood_trig-(double)ngood_rpc_bot)/(double)ngood_trig;

  Double_t eff_rpc_topall = (double)ngood_rpc_topall/(double)ngood_trig;
  Double_t eff_rpc_botall = (double)ngood_rpc_botall/(double)ngood_trig;
  Double_t eeff_rpc_topall = sqrt((double)ngood_trig-(double)ngood_rpc_topall)/(double)ngood_trig;
  Double_t eeff_rpc_botall = sqrt((double)ngood_trig-(double)ngood_rpc_botall)/(double)ngood_trig;

  Double_t eff_c1 = (double)ngood_c1/(double)ngood_trig;
  Double_t eff_c2 = (double)ngood_c2/(double)ngood_trig;
  Double_t eeff_c1 = sqrt((double)ngood_trig-(double)ngood_c1)/(double)ngood_trig;
  Double_t eeff_c2 = sqrt((double)ngood_trig-(double)ngood_c2)/(double)ngood_trig;

  Double_t eff_c1_trig_c2 = (double)ngood_c1_trig_c2/(double)ngood_trig_c2;
  Double_t eff_c2_trig_c1 = (double)ngood_c2_trig_c1/(double)ngood_trig_c1;
  Double_t eeff_c1_trig_c2 = sqrt((double)ngood_trig_c2-(double)ngood_c1_trig_c2)/(double)ngood_trig_c2;
  Double_t eeff_c2_trig_c1 = sqrt((double)ngood_trig_c1-(double)ngood_c2_trig_c1)/(double)ngood_trig_c1;

  Double_t eff_rpc_top_trig_c1_c2 = (double)ngood_rpc_top_trig_c1_c2/(double)ngood_trig_c1_c2;
  Double_t eff_rpc_bot_trig_c1_c2 = (double)ngood_rpc_bot_trig_c1_c2/(double)ngood_trig_c1_c2;
  Double_t eeff_rpc_top_trig_c1_c2 = sqrt((double)ngood_trig_c1_c2-(double)ngood_rpc_top_trig_c1_c2)/(double)ngood_trig_c1_c2;
  Double_t eeff_rpc_bot_trig_c1_c2 = sqrt((double)ngood_trig_c1_c2-(double)ngood_rpc_bot_trig_c1_c2)/(double)ngood_trig_c1_c2;

  Double_t eff_rpc_top_trig_c1 = (double)ngood_rpc_top_trig_c1/(double)ngood_trig_c1;
  Double_t eff_rpc_bot_trig_c1 = (double)ngood_rpc_bot_trig_c1/(double)ngood_trig_c1;
  Double_t eeff_rpc_top_trig_c1 = sqrt((double)ngood_trig_c1-(double)ngood_rpc_top_trig_c1)/(double)ngood_trig_c1;
  Double_t eeff_rpc_bot_trig_c1 = sqrt((double)ngood_trig_c1-(double)ngood_rpc_bot_trig_c1)/(double)ngood_trig_c1;

  Double_t eff_rpc_top_trig_c2 = (double)ngood_rpc_top_trig_c2/(double)ngood_trig_c2;
  Double_t eff_rpc_bot_trig_c2 = (double)ngood_rpc_bot_trig_c2/(double)ngood_trig_c2;
  Double_t eeff_rpc_top_trig_c2 = sqrt((double)ngood_trig_c2-(double)ngood_rpc_top_trig_c2)/(double)ngood_trig_c2;
  Double_t eeff_rpc_bot_trig_c2 = sqrt((double)ngood_trig_c2-(double)ngood_rpc_bot_trig_c2)/(double)ngood_trig_c2;


  ofstream ofs(Form("./ana/ana_rpccosmic/eff_rpc_strip%d_run%d.out",rpc_strip_test,run));
  cout << "eff_rpc_top = " << eff_rpc_top << "+-"<<eeff_rpc_top << endl;
  cout << "eff_rpc_bot = " << eff_rpc_bot << "+-"<<eeff_rpc_bot << endl;
  cout << "eff_rpc_topall = " << eff_rpc_topall << "+-"<<eeff_rpc_topall << endl;
  cout << "eff_rpc_botall = " << eff_rpc_botall << "+-"<<eeff_rpc_botall << endl;
  cout << "eff_rpc_top_trig_c1_c2 = " << eff_rpc_top_trig_c1_c2 << "+-"<<eeff_rpc_top_trig_c1_c2 << endl;
  cout << "eff_rpc_bot_trig_c1_c2 = " << eff_rpc_bot_trig_c1_c2 << "+-"<<eeff_rpc_bot_trig_c1_c2 << endl;
  cout << "eff_rpc_top_trig_c1 = " << eff_rpc_top_trig_c1 << "+-"<<eeff_rpc_top_trig_c1 << endl;
  cout << "eff_rpc_bot_trig_c1 = " << eff_rpc_bot_trig_c1 << "+-"<<eeff_rpc_bot_trig_c1 << endl;
  cout << "eff_rpc_top_trig_c2 = " << eff_rpc_top_trig_c2 << "+-"<<eeff_rpc_top_trig_c2 << endl;
  cout << "eff_rpc_bot_trig_c2 = " << eff_rpc_bot_trig_c2 << "+-"<<eeff_rpc_bot_trig_c2 << endl;

  cout << "eff_c1 = " << eff_c1 << "+-"<<eeff_c1 << endl;
  cout << "eff_c2 = " << eff_c2 << "+-"<<eeff_c2 << endl;
  cout << "eff_c1_trig_c2 = " << eff_c1_trig_c2 << "+-"<<eeff_c1_trig_c2 << endl;
  cout << "eff_c2_trig_c1 = " << eff_c2_trig_c1 << "+-"<<eeff_c2_trig_c1 << endl;
  ofs << "eff_rpc_top = " << eff_rpc_top << "+-"<<eeff_rpc_top << endl;
  ofs << "eff_rpc_bot = " << eff_rpc_bot << "+-"<<eeff_rpc_bot << endl;
  ofs << "eff_rpc_topall = " << eff_rpc_topall << "+-"<<eeff_rpc_topall << endl;
  ofs << "eff_rpc_botall = " << eff_rpc_botall << "+-"<<eeff_rpc_botall << endl;
  ofs << "eff_rpc_top_trig_c1_c2 = " << eff_rpc_top_trig_c1_c2 << "+-"<<eeff_rpc_top_trig_c1_c2 << endl;
  ofs << "eff_rpc_bot_trig_c1_c2 = " << eff_rpc_bot_trig_c1_c2 << "+-"<<eeff_rpc_bot_trig_c1_c2 << endl;
  ofs << "eff_rpc_top_trig_c1 = " << eff_rpc_top_trig_c1 << "+-"<<eeff_rpc_top_trig_c1 << endl;
  ofs << "eff_rpc_bot_trig_c1 = " << eff_rpc_bot_trig_c1 << "+-"<<eeff_rpc_bot_trig_c1 << endl;
  ofs << "eff_rpc_top_trig_c2 = " << eff_rpc_top_trig_c2 << "+-"<<eeff_rpc_top_trig_c2 << endl;
  ofs << "eff_rpc_bot_trig_c2 = " << eff_rpc_bot_trig_c2 << "+-"<<eeff_rpc_bot_trig_c2 << endl;
  ofs << "eff_c1 = " << eff_c1 << "+-"<<eeff_c1 << endl;
  ofs << "eff_c2 = " << eff_c2 << "+-"<<eeff_c2 << endl;
  ofs << "eff_c1_trig_c2 = " << eff_c1_trig_c2 << "+-"<<eeff_c1_trig_c2 << endl;
  ofs << "eff_c2_trig_c1 = " << eff_c2_trig_c1 << "+-"<<eeff_c2_trig_c1 << endl;
  ofs.close();


  gStyle->SetOptStat(0);
  hlt_r1->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_r1_run%d.gif",run));
  c1->Update();
  hlt_r2->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_r2_run%d.gif",run));
  c1->Update();
  hlt_s1->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_s1_run%d.gif",run));
  c1->Update();
  hlt_s2->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_s2_run%d.gif",run));
  c1->Update();
  hlt_s3->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_s3_run%d.gif",run));
  c1->Update();
  hlt_s4->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_s4_run%d.gif",run));
  c1->Update();

  gStyle->SetOptStat(0);
  hlt_wt_r1->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_wt_r1_run%d.gif",run));
  c1->Update();
  hlt_wt_r2->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_wt_r2_run%d.gif",run));
  c1->Update();
  hlt_wt_s1->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_wt_s1_run%d.gif",run));
  c1->Update();
  hlt_wt_s2->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_wt_s2_run%d.gif",run));
  c1->Update();
  hlt_wt_s3->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_wt_s3_run%d.gif",run));
  c1->Update();
  hlt_wt_s4->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hlt_wt_s4_run%d.gif",run));
  c1->Update();

  hmt_r12->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hmt_r12_run%d.gif",run));
  c1->Update();
  hmt_s12->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hmt_s12_run%d.gif",run));
  c1->Update();
  hmt_s34->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hmt_s34_run%d.gif",run));
  c1->Update();
  hmt_wt_r12->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hmt_wt_r12_run%d.gif",run));
  c1->Update();
  hmt_wt_s12->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hmt_wt_s12_run%d.gif",run));
  c1->Update();
  hmt_wt_s34->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hmt_wt_s34_run%d.gif",run));
  c1->Update();
  hmt_wt_r12_c12->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hmt_wt_r12_c12_run%d.gif",run));
  c1->Update();
  hmt_wt_s12_c12->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hmt_wt_s12_c12_run%d.gif",run));
  c1->Update();
  hmt_wt_s34_c12->Draw("colz");
  c1->SaveAs(Form("ana/ana_rpccosmic/hmt_wt_s34_c12_run%d.gif",run));
  c1->Update();
  hdt_r12->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hdt_r12_run%d.gif",run));
  c1->Update();
  hdt_s12->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hdt_s12_run%d.gif",run));
  c1->Update();
  hdt_s34->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/hdt_s34_run%d.gif",run));
  c1->Update();
  htof_r12_s12->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_r12_s12_run%d.gif",run));
  c1->Update();
  htof_r12_s34->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_r12_s34_run%d.gif",run));
  c1->Update();
  htof_s12_s34->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_s12_s34_run%d.gif",run));
  c1->Update();
  htof_r12_s12_c12->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_r12_s12_c12_run%d.gif",run));
  c1->Update();
  htof_r12_s34_c12->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_r12_s34_c12_run%d.gif",run));
  c1->Update();
  htof_s12_s34_c12->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_s12_s34_c12_run%d.gif",run));
  c1->Update();

  htof_r12_s12_c1->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_r12_s12_c1_run%d.gif",run));
  c1->Update();
  htof_r12_s34_c1->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_r12_s34_c1_run%d.gif",run));
  c1->Update();
  htof_s12_s34_c1->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_s12_s34_c1_run%d.gif",run));
  c1->Update();

  htof_r12_s12_c2->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_r12_s12_c2_run%d.gif",run));
  c1->Update();
  htof_r12_s34_c2->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_r12_s34_c2_run%d.gif",run));
  c1->Update();
  htof_s12_s34_c2->Draw();
  c1->SaveAs(Form("ana/ana_rpccosmic/htof_s12_s34_c2_run%d.gif",run));
  c1->Update();


  TFile *fout = new TFile(argv[3],"RECREATE");

  hlt_r1->Write();
  hlt_r2->Write();
  hlt_s1->Write();
  hlt_s2->Write();
  hlt_s3->Write();
  hlt_s4->Write();

  hlt_wt_r1->Write();
  hlt_wt_r2->Write();
  hlt_wt_s1->Write();
  hlt_wt_s2->Write();
  hlt_wt_s3->Write();
  hlt_wt_s4->Write();

  hmt_r12->Write();
  hmt_s12->Write();
  hmt_s34->Write();
  hmt_wt_r12->Write();
  hmt_wt_s12->Write();
  hmt_wt_s34->Write();
  hdt_r12->Write();
  hdt_s12->Write();
  hdt_s34->Write();
  htof_r12_s12->Write();
  htof_r12_s34->Write();
  htof_s12_s34->Write();

  htof_r12_s12_sl->Write();
  htof_r12_s34_sl->Write();
  htof_s12_s34_sl->Write();
  htof_r12_s12_slg->Write();
  htof_r12_s34_slg->Write();
  htof_s12_s34_slg->Write();
  htofs_r12_s12_slg->Write();
  htofs_r12_s34_slg->Write();
  htofs_s12_s34_slg->Write();

  htof_r12_s12_c12->Write();
  htof_r12_s34_c12->Write();
  htof_s12_s34_c12->Write();
  htof_r12_s12_c12g->Write();
  htof_r12_s34_c12g->Write();
  htof_s12_s34_c12g->Write();
  htofs_r12_s12_c12g->Write();
  htofs_r12_s34_c12g->Write();
  htofs_s12_s34_c12g->Write();

  htof_r12_s12_c1->Write();
  htof_r12_s34_c1->Write();
  htof_s12_s34_c1->Write();

  htof_r12_s12_c2->Write();
  htof_r12_s34_c2->Write();
  htof_s12_s34_c2->Write();

  hsc_nh_tb->Write();
  hsc_mt->Write();
  hsc_dt->Write();
  hsc_dts->Write();
  hsc_mt_g->Write();
  hsc_dt_g->Write();
  hsc_wm->Write();
  hsc_mt_wm->Write();
  hsc_dt_wm->Write();
  hsc_dt_w0->Write();
  hsc_dt_w1->Write();
  for (Int_t itb=0;itb<2;itb++) {
    hsc_nh[itb]->Write();
    hsc_lt[itb]->Write();
    hsc_tt[itb]->Write();
    hsc_w[itb]->Write();
  }
  hsc_lt_wt->Write();
  hsc_lt_wb->Write();
  hsc_lt_wt_g->Write();
  hsc_lt_wb_g->Write();

  hsc_lts_wt_g->Write();
  hsc_lts_wb_g->Write();
  
  hsc_lt_wt_gall->Write();
  hsc_lt_wb_gall->Write();

  hsc_lts_wt_gall->Write();
  hsc_lts_wb_gall->Write();
  
  for (Int_t itsc=0;itsc<3;itsc++) {
    htsc_nh_tb[itsc]->Write();
    htsc_mt[itsc]->Write();
    htsc_dt[itsc]->Write();
    htsc_dts[itsc]->Write();
    htsc_mt_g[itsc]->Write();
    htsc_dt_g[itsc]->Write();
    htsc_wm[itsc]->Write();
    htsc_mt_wm[itsc]->Write();
    htsc_dt_wm[itsc]->Write();
    htsc_dt_w0[itsc]->Write();
    htsc_dt_w1[itsc]->Write();
    for (Int_t itb=0;itb<2;itb++) {
      htsc_nh[itsc][itb]->Write();
      htsc_lt[itsc][itb]->Write();
      htsc_tt[itsc][itb]->Write();
      htsc_w[itsc][itb]->Write();
    }
  }
  
  htsc0_lt_wt->Write();
  htsc0_lt_wb->Write();

  htsc0_lt_wt_g->Write();
  htsc0_lt_wb_g->Write();

  htsc0_lts_wt_g->Write();
  htsc0_lts_wb_g->Write();

  htsc0_lt_wt_gall->Write();
  htsc0_lt_wb_gall->Write();

  htsc0_lts_wt_gall->Write();
  htsc0_lts_wb_gall->Write();

  htsc1_lt_wt->Write();
  htsc1_lt_wb->Write();

  htsc1_lt_wt_g->Write();
  htsc1_lt_wb_g->Write();

  htsc1_lts_wt_g->Write();
  htsc1_lts_wb_g->Write();

  htsc1_lt_wt_gall->Write();
  htsc1_lt_wb_gall->Write();

  htsc1_lts_wt_gall->Write();
  htsc1_lts_wb_gall->Write();

  htsc2_lt_wt->Write();
  htsc2_lt_wb->Write();

  htsc2_lt_wt_g->Write();
  htsc2_lt_wb_g->Write();

  htsc2_lts_wt_g->Write();
  htsc2_lts_wb_g->Write();

  htsc2_lt_wt_gall->Write();
  htsc2_lt_wb_gall->Write();

  htsc2_lts_wt_gall->Write();
  htsc2_lts_wb_gall->Write();

  htof_sc_tsc0->Write();
  htof_sc_tsc1->Write();
  htof_sc_tsc0_g->Write();
  htof_sc_tsc1_g->Write();
  htofs_sc_tsc0_g->Write();
  htofs_sc_tsc1_g->Write();
  htofs_sc_tsc0_lg->Write();
  htofs_sc_tsc1_lg->Write();
  htof_sc_tsc2->Write();
  htof_tsc01->Write();
  htof_tsc01_g->Write();
  htofs_tsc01_g->Write();
  htofs_tsc01_lg->Write();
  htof_tsc02->Write();
  htof_tsc02_g->Write();
  htofs_tsc02_g->Write();
  htofs_tsc02_lg->Write();
  htof_tsc12->Write();
  htof_tsc12->Write();
  htof_tsc12->Write();
  htof_tsc12_g->Write();
  htofs_tsc12_g->Write();
  htofs_tsc12_lg->Write();


  hsc_tsc0_lt_lg->Write();
  hsc_tsc0_lb_lg->Write();
  hsc_tsc1_lt_lg->Write();
  hsc_tsc1_lb_lg->Write();
  htsc0_tsc1_lt_lg->Write();
  htsc0_tsc1_lb_lg->Write();


  hsc_tsc0_wt_lg->Write();
  hsc_tsc1_wt_lg->Write();
  htsc0_tsc1_wt_lg->Write();

  hsc_tsc0_wb_lg->Write();
  hsc_tsc1_wb_lg->Write();
  htsc0_tsc1_wb_lg->Write();

  hsc_dt_mt->Write();
  htsc0_dt_mt->Write();
  htsc1_dt_mt->Write();
  hsc_dts_mts->Write();
  htsc0_dts_mts->Write();
  htsc1_dts_mts->Write();
  hsc_dts_mtd->Write();
  htsc0_dts_mtd->Write();
  htsc1_dts_mtd->Write();
  htofs_stsc0_dt_tsc0->Write();
  htofs_stsc1_dt_tsc0->Write();
  htofs_tsc01_dt_tsc0->Write();
  htofs_stsc0_dt_tsc1->Write();
  htofs_stsc1_dt_tsc1->Write();
  htofs_tsc01_dt_tsc1->Write();
  htofd_stsc0_dt_tsc1->Write();
  htofd_stsc1_dt_tsc1->Write();
  htofd_tsc01_dt_tsc1->Write();

  htofd_sc_tsc0_g->Write();
  htofd_sc_tsc1_g->Write();
  htofd_tsc01_g->Write();

  htofdw_sc_tsc0_g->Write();
  htofdw_sc_tsc1_g->Write();
  htofdw_tsc01_g->Write();

  htofd_stsc0_wm_sc->Write();
  htofd_stsc1_wm_sc->Write();
  htofd_tsc01_wm_tsc0->Write();
    
  htofd_stsc0_wm_tsc0->Write();
  htofd_stsc1_wm_tsc1->Write();
  htofd_tsc01_wm_tsc1->Write();

  htofdw_stsc0_wm_sc->Write();
  htofdw_stsc1_wm_sc->Write();
  htofdw_tsc01_wm_tsc0->Write();
    
  htofdw_stsc0_wm_tsc0->Write();
  htofdw_stsc1_wm_tsc1->Write();
  htofdw_tsc01_wm_tsc1->Write();

  fout->Close();
}


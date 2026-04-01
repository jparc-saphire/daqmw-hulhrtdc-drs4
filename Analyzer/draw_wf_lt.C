#include <iostream>
#include <stdexcept>
#include <vector>
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

void draw_wf_lt(int run_number, int sub_run=0){

    gStyle -> SetOptStat(11111111);
    gStyle -> SetOptFit(1111);
    gStyle -> SetTitleXSize(0.05);
    gStyle -> SetTitleYSize(0.05);


    int ch1 = 5;//AC1
    int ch2 = 6;//AC2
    int ch3 = 7;//AC3
    int ch4 = 0;//AC4

    int ch1_tdc = 16+13;//AC1
    int ch2_tdc = 16+14;//AC2
    int ch3_tdc = 16+15;//AC3
    int ch4_tdc = 16+7;//AC4

    TFile *file0 = new TFile(Form("./data/run%06d_%d.root", run_number,sub_run), "read");
    TTree *tree0 = (TTree*)file0 -> Get("tree");

    std::vector<std::vector<double>> *amp = 0;
    std::vector<std::vector<double>> *ltdc2 = 0;
    std::vector<std::vector<double>> *ttdc2 = 0;
    std::vector<std::vector<double>> *WaveForm = 0;
    std::vector<std::vector<double>> *bl = 0;

    TBranch *Bamp = 0;
    TBranch *Bltdc2 = 0;
    TBranch *Bttdc2 = 0;
    TBranch *BWaveForm = 0;
    TBranch *Bbl = 0;

    Int_t nx=8;
    Int_t ny=6;

    tree0 -> SetBranchAddress("amp", &amp, &Bamp);
    tree0 -> SetBranchAddress("ltdc2", &ltdc2, &Bltdc2);
    tree0 -> SetBranchAddress("ttdc2", &ttdc2, &Bttdc2);
    tree0 -> SetBranchAddress("WaveForm", &WaveForm, &BWaveForm);
    tree0 -> SetBranchAddress("bl", &bl, &Bbl);

    int Nevent = tree0 -> GetEntries();

    double t[1024];
    double wave1[1024]={0.};
    double wave2[1024]={0.};
    double wave3[1024]={0.};
    double wave4[1024]={0.};
    for(int i = 0; i<1024; i++) t[i] = i;

    TCanvas *c1 = new TCanvas("c1", "c1", 1200, 900);
    TCanvas *c2 = new TCanvas("c2", "c2", 1200, 900);
    TCanvas *c3 = new TCanvas("c3", "c3", 1200, 900);
    TCanvas *c4 = new TCanvas("c4", "c4", 1200, 900);
    c1 -> Divide(nx,ny);
    c2 -> Divide(nx,ny);
    c3 -> Divide(nx,ny);
    c4 -> Divide(nx,ny);

    int count = 0;
    //    double threshold;
    //    if(run_number<74) threshold = -17.5;
    //    else{threshold = -35;}

    for(int i = 0; i<Nevent; i++){
        tree0 -> GetEntry(i);
        int wavesize1 = WaveForm -> at(ch1).size();
	//	cout << "wavesize1 = " << wavesize1 << endl;
	int wavesize2 = WaveForm -> at(ch2).size();
        int wavesize3 = WaveForm -> at(ch3).size();
	int wavesize4 = WaveForm -> at(ch4).size();
        int basesize1 = bl -> at(ch1).size();
	int basesize2 = bl -> at(ch2).size();
        int basesize3 = bl -> at(ch3).size();
	int basesize4 = bl -> at(ch4).size();

	//	const Double_t lt_min_ac1 = 1900.;
	//	const Double_t lt_max_ac1 = 2100.;
	//	const Double_t lt_min_ac4 = 2000.;
	//	const Double_t lt_max_ac4 = 2200.;
	/*
	const Double_t lt_min_ac1 = 1950.;
	const Double_t lt_max_ac1 = 2030.;
	const Double_t lt_min_ac4 = 2050.;
	const Double_t lt_max_ac4 = 2100.;
	*/
	const Double_t lt_min_ac4 = 2030.;
	const Double_t lt_max_ac4 = 2130.;
	const Double_t lt_min_ac1 = 1950.;
	const Double_t lt_max_ac1 = 2050.;
	//tdc for ACs
	Int_t nh_ch1_tdc = ltdc2->at(ch1_tdc).size();
	Bool_t good_ch1_tdc = false;
	if (nh_ch1_tdc>0) {
	  Double_t lt = ltdc2->at(ch1_tdc).at(0);
	  if ((lt_min_ac1 < lt)&&(lt < lt_max_ac1)) {
	    good_ch1_tdc = true;
	    cout << "good_ch1_tdc" << endl;
	  }
	}
	Int_t nh_ch2_tdc = ltdc2->at(ch2_tdc).size();
	Bool_t good_ch2_tdc = false;
	if (nh_ch2_tdc>0) {
	  Double_t lt = ltdc2->at(ch2_tdc).at(0);
	  if ((lt_min_ac1 < lt)&&(lt < lt_max_ac1)) {
	    good_ch2_tdc = true;
	    cout << "good_ch2_tdc" << endl;
	  }
	}
	Int_t nh_ch3_tdc = ltdc2->at(ch3_tdc).size();
	Bool_t good_ch3_tdc = false;
	if (nh_ch3_tdc>0) {
	  Double_t lt = ltdc2->at(ch3_tdc).at(0);
	  if ((lt_min_ac1 < lt)&&(lt < lt_max_ac1)) {
	    good_ch3_tdc = true;
	    cout << "good_ch3_tdc" << endl;
	  }
	}
	Int_t nh_ch4_tdc = ltdc2->at(ch4_tdc).size();
	Bool_t good_ch4_tdc = false;
	if (nh_ch4_tdc>0) {
	  Double_t lt = ltdc2->at(ch4_tdc).at(0);
	  if ((lt_min_ac4 < lt)&&(lt < lt_max_ac4)) {
	    good_ch4_tdc = true;
	    cout << "good_ch4_tdc" << endl;
	  }
	}

	//	cout << "test1" << endl;
	//	cout << "wavesize1, basesize1 " << wavesize1 << " " << basesize1 << endl;
	//	cout << "wavesize2, basesize2 " << wavesize2 << " " << basesize2 << endl;
        if(wavesize1<=0 || basesize1<=0) continue;

        double base1 = 0;
        for(int ii = 150; ii<200; ii++) base1+=WaveForm->at(ch1).at(ii);
        base1 = base1/50.;
        double base2 = 0;
        for(int ii = 150; ii<200; ii++) base2+=WaveForm->at(ch2).at(ii);
        base2 = base2/50.;
        double base3 = 0;
        for(int ii = 150; ii<200; ii++) base3+=WaveForm->at(ch3).at(ii);
        base3 = base3/50.;
        double base4 = 0;
        for(int ii = 150; ii<200; ii++) base4+=WaveForm->at(ch4).at(ii);
        base4 = base4/50.;

        for(int ii = 0; ii<wavesize1; ii++) wave1[ii] = -WaveForm->at(ch1).at(ii) + base1;
        for(int ii = 0; ii<wavesize2; ii++) wave2[ii] = -WaveForm->at(ch2).at(ii) + base2;
        for(int ii = 0; ii<wavesize3; ii++) wave3[ii] = -WaveForm->at(ch3).at(ii) + base3;
        for(int ii = 0; ii<wavesize4; ii++) wave4[ii] = -WaveForm->at(ch4).at(ii) + base4;

	TGraph *g_wf1 = new TGraph(1024, t, wave1);
        TGraph *g_wf2 = new TGraph(1024, t, wave2);
	TGraph *g_wf3 = new TGraph(1024, t, wave3);
        TGraph *g_wf4 = new TGraph(1024, t, wave4);

	if (good_ch1_tdc) {
	  c1 -> cd(count%32+1);
	  g_wf1 -> SetTitle(";(ns);(mV)");
	  g_wf1 -> GetXaxis() -> SetLimits(0, 1024);
	  g_wf1 -> Draw("apl");
	}

	if (good_ch2_tdc) {	
	  c2 -> cd(count%32+1);
	  g_wf2 -> SetTitle(";(ns);(mV)");
	  g_wf2 -> GetXaxis() -> SetLimits(0, 1024);
	  g_wf2 -> Draw("apl");
	}

	if (good_ch3_tdc) { 	
	  c3 -> cd(count%32+1);
	  g_wf3 -> SetTitle(";(ns);(mV)");
	  g_wf3 -> GetXaxis() -> SetLimits(0, 1024);
	  g_wf3 -> Draw("apl");
	}

	if (good_ch4_tdc) { 		
	  c4 -> cd(count%32+1);
	  g_wf4 -> SetTitle(";(ns);(mV)");
	  g_wf4 -> GetXaxis() -> SetLimits(0, 1024);
	  g_wf4 -> Draw("apl");
	}

        count++;

	TString input;
	if ((count>1)&&((count%(nx*ny))==1)) {
	  c1->Update();
	  c2->Update();
	  c3->Update();
	  c4->Update();
	  cout << "paused put some string to go next" << endl;
	  cin >> input;
	}
    }

    return;
}

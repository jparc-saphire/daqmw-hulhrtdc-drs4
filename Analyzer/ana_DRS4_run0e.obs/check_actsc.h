#ifndef check_actsc_h
#define check_actsc_h

#include <TFile.h>
#include <TTree.h>
#include <TString.h>

#include <vector>

#include <iostream>

using namespace std;

class check_actsc{
    public :

  void     fileopen0(Int_t run, Int_t subrun=0);
    void     Init0(TTree *tree);

    double   call(vector<vector<double> > *vec, Int_t arg1, Int_t arg2);

    void     check(Int_t run, Int_t subrun=0);

    void     count(Int_t Nevent, Int_t eventid);
    Int_t    percent_tmp;

    //-----object-----
    TFile   *file0;
    TTree   *tree0;

    // List of branches
    TBranch        *b_nh_ltdc0;
    TBranch        *b_nh_ttdc0;
    TBranch        *b_ltdc0;   
    TBranch        *b_ttdc0;   
    TBranch        *b_width0;  
  
    TBranch *b_amp0 = 0;
    TBranch *b_adc0 = 0;
    TBranch *b_wf0 = 0;
    TBranch *b_bl0 = 0;

    //-----raw data-----
    Int_t   nh_ltdc0[64];
    Int_t   nh_ttdc0[64];

    vector<vector<double> > *ltdc0 = 0;
    vector<vector<double> > *ttdc0 = 0;
    vector<vector<double> > *width0 = 0;
    vector<std::vector<double>> *amp0 = 0;
    vector<std::vector<double>> *adc0 = 0;
    std::vector<std::vector<double>> *wf0 = 0;
    std::vector<std::vector<double>> *bl0 = 0;


    check_actsc(Int_t run, Int_t subrun=0, TString basedir = "./data");

 private:
    TString base;
    static const int ntdc=1;
    //def    static const int nch=32;
    //sako temp
    static const int nch=64;
};
#endif

#ifdef check_actsc_cc

void check_actsc::fileopen0(Int_t run, Int_t subrun)
{
  file0 = new TFile(Form("%s/run%06d_%d.root", base.Data(), run, subrun), "read");

    if(!file0){
      cout << Form("Can't open run%06d_%d.root", run, subrun) << endl;;
        return;
    }
    tree0 = (TTree*)file0 -> Get("tree");
    Init0(tree0);
}

void check_actsc::Init0(TTree *tree){
    tree -> SetBranchAddress("ltdc", &ltdc0, &b_ltdc0);
    tree -> SetBranchAddress("ttdc", &ttdc0, &b_ttdc0);
    tree -> SetBranchAddress("width", &width0, &b_width0);
    tree -> SetBranchAddress("amp", &amp0, &b_amp0);
    tree -> SetBranchAddress("adc", &adc0, &b_adc0);
    tree -> SetBranchAddress("WaveForm", &wf0, &b_wf0);
    tree -> SetBranchAddress("bl", &bl0, &b_bl0);
}

void check_actsc::count(Int_t Nevent, Int_t eventid){

  cout << "eventid, Nevent = " << eventid << " " << Nevent << endl;
    Int_t percent = 100*eventid/Nevent;
    if(eventid==Nevent-1) cout << "\r" << "[##########] "   << 100 << "%" << string(10, ' ') << endl;
    if(percent == percent_tmp) return;

    if(percent<10)        cout << "\r" << "[__________]   " << percent << "%" << string(5, ' ') << flush;
    else if(percent<20)   cout << "\r" << "[#_________]  "  << percent << "%" << string(5, ' ') << flush;
    else if(percent<30)   cout << "\r" << "[##________]  "  << percent << "%" << string(5, ' ') << flush;
    else if(percent<40)   cout << "\r" << "[###_______]  "  << percent << "%" << string(5, ' ') << flush;
    else if(percent<50)   cout << "\r" << "[####______]  "  << percent << "%" << string(5, ' ') << flush;
    else if(percent<60)   cout << "\r" << "[#####_____]  "  << percent << "%" << string(5, ' ') << flush;
    else if(percent<70)   cout << "\r" << "[######____]  "  << percent << "%" << string(5, ' ') << flush;
    else if(percent<80)   cout << "\r" << "[#######___]  "  << percent << "%" << string(5, ' ') << flush;
    else if(percent<90)   cout << "\r" << "[########__]  "  << percent << "%" << string(5, ' ') << flush;
    else if(percent<100)  cout << "\r" << "[#########_]  "  << percent << "%" << string(5, ' ') << flush;

    percent_tmp = percent;
    // if(eventid == Nevent-1) cout << endl;
}

#endif

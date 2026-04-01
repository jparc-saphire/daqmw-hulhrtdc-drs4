#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"

void tcorr2()
{
  TFile *fin = new TFile("../../test_run011.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double amp1[16],amp2[16],amp3[16],amp4[16];
  double tdc1[16],tdc2[16],tdc3[16],tdc4[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("amp3", amp3);
  tree->SetBranchAddress("amp4", amp4);
  tree->SetBranchAddress("tdc1", tdc1);
  tree->SetBranchAddress("tdc2", tdc2);
  tree->SetBranchAddress("tdc3", tdc3);
  tree->SetBranchAddress("tdc4", tdc4);

  TH2F *hist[4];
  hist[0] = new TH2F("h1", "TOF2vsAMP1", 500, 0, 500, 1000, -5, 5);
  hist[1] = new TH2F("h2", "TOF2vsAMP2", 500, 0, 500, 1000, -5, 5);
  hist[2] = new TH2F("h3", "TOF2vsAMP3", 900, 0, 900, 1000, -5, 5);
  hist[3] = new TH2F("h4", "TOF2vsAMP4", 900, 0, 900, 1000, -5, 5);

  double a01,b01,c01;
  double a03,b03,c03;
  double a04,b04,c04;
  char check[256];
  ifstream ifs1("corr1.dat");
  
  while(ifs1.getline(check,256)){
    stringstream st1;
    st1<<check;
    if(strlen(check)==0) continue;
    else{
      st1 >> a01 >> b01 >> c01 
	  >> a03 >> b03 >> c03 
	  >> a04 >> b04 >> c04; 
    }
  }
  
  cout<< a01 << "  " << b01 << "  " << c01 << endl;
  cout<< a03 << "  " << b03 << "  " << c03 << endl;
  cout<< a04 << "  " << b04 << "  " << c04 << endl;

  double a11,b11,c11;
  double a13,b13,c13;
  double a14,b14,c14;
  char check2[256];
  ifstream ifs2("corr2.dat");
  
  while(ifs2.getline(check2,256)){
    stringstream st2;
    st2<<check2;
    if(strlen(check2)==0) continue;
    else{
      st2 >> a11 >> b11 >> c11 
	  >> a13 >> b13 >> c13 
	  >> a14 >> b14 >> c14; 
    }
  }
  
  cout<< "****" << endl;
  cout<< a11 << "  " << b11 << "  " << c11 << endl;
  cout<< a13 << "  " << b13 << "  " << c13 << endl;
  cout<< a14 << "  " << b14 << "  " << c14 << endl;

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if( tdc1[0]>0&&tdc3[0]>0&&tdc4[0]>0&&
	amp1[0]>0&&amp3[0]>0&&amp4[0]>0){

      double T1 =  a01+b01*pow(amp1[0]-70.0,0.5)+c01*(amp1[0]-70.0);
      double T3 =  a03+b03*pow(amp3[0]-50.0,0.5)+c03*(amp3[0]-50.0);
      double T4 =  a04+b04*pow(amp4[0]-60.0,0.5)+c04*(amp4[0]-60.0);
      
      double TOF1 = (tdc1[0]-(tdc3[0]+tdc4[0])/2.-6.24)-T1-(T3+T4)/2.;

      double T11 =  a11+b11/pow(amp1[0]-70.0,0.5)+c11*(amp1[0]-70.0);
      double T13 =  a13+b13/pow(amp3[0]-50.0,0.5)+c13*(amp3[0]-50.0);
      double T14 =  a14+b14/pow(amp4[0]-60.0,0.5)+c14*(amp4[0]-60.0);
      
      double TOF2 = TOF1-T11-(T13+T14)/2.;

      double Amp1 = amp1[0]-70.0;
      double Amp3 = amp3[0]-50.0;
      double Amp4 = amp4[0]-60.0;
      
      if( Amp1>50 && Amp3>50 && Amp4>50){
	hist[0]->Fill((amp1[0]-70.0),TOF2);
	hist[0]->GetXaxis()->SetTitle("AMP1");
	hist[0]->GetYaxis()->SetTitle("TOF2");
	hist[2]->Fill((amp3[0]-50.0),TOF2);
	hist[2]->GetXaxis()->SetTitle("AMP3");
	hist[2]->GetYaxis()->SetTitle("TOF2");
	hist[3]->Fill((amp4[0]-60.0),TOF2);
	hist[3]->GetXaxis()->SetTitle("AMP4");
	hist[3]->GetYaxis()->SetTitle("TOF2");
      }
    }
  }

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(2,2);
  for(int i=0; i<4; ++i)
    {
      c1->cd(i+1);
      hist[i]->Draw("colz");
    }

  c1->Print("tcorr2.pdf)");

}

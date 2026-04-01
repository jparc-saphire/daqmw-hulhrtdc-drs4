#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"

void tcorr3()
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
  hist[0] = new TH2F("h1", "TOF2vsAMP1",  600, 0,  600, 1000, -3, 3);
  hist[1] = new TH2F("h2", "TOF2vsAMP2",  600, 0,  600, 1000, -3, 3);
  hist[2] = new TH2F("h3", "TOF2vsAMP3", 1000, 0, 1000, 1000, -3, 3);
  hist[3] = new TH2F("h4", "TOF2vsAMP4", 1000, 0, 1000, 1000, -3, 3);

  double a01,b01,c01;
  double a02,b02,c02;
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
	  >> a02 >> b02 >> c02 
	  >> a03 >> b03 >> c03 
	  >> a04 >> b04 >> c04; 
    }
  }
  
  cout<< a01 << "  " << b01 << "  " << c01 << endl;
  cout<< a02 << "  " << b02 << "  " << c02 << endl;
  cout<< a03 << "  " << b03 << "  " << c03 << endl;
  cout<< a04 << "  " << b04 << "  " << c04 << endl;

  double a11,b11,c11;
  double a12,b12,c12;
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
	  >> a12 >> b12 >> c12 
	  >> a13 >> b13 >> c13 
	  >> a14 >> b14 >> c14; 
    }
  }
  
  cout<< "****" << endl;
  cout<< a11 << "  " << b11 << "  " << c11 << endl;
  cout<< a12 << "  " << b12 << "  " << c12 << endl;
  cout<< a13 << "  " << b13 << "  " << c13 << endl;
  cout<< a14 << "  " << b14 << "  " << c14 << endl;

  double a21,b21,c21;
  double a22,b22,c22;
  double a23,b23,c23;
  double a24,b24,c24;
  char check3[256];
  ifstream ifs3("corr3.dat");
  
  while(ifs3.getline(check3,256)){
    stringstream st3;
    st3<<check3;
    if(strlen(check3)==0) continue;
    else{
      st3 >> a21 >> b21 >> c21 
	  >> a22 >> b22 >> c22 
	  >> a23 >> b23 >> c23 
	  >> a24 >> b24 >> c24; 
    }
  }
  
  cout<< "****" << endl;
  cout<< a21 << "  " << b21 << "  " << c21 << endl;
  cout<< a22 << "  " << b22 << "  " << c22 << endl;
  cout<< a23 << "  " << b23 << "  " << c23 << endl;
  cout<< a24 << "  " << b24 << "  " << c24 << endl;

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if( tdc1[0]>0&&tdc2[0]>0&&tdc3[0]>0&&tdc4[0]>0&&
	amp1[0]>0&&amp2[0]>0&&amp3[0]>0&&amp4[0]>0){

      double T1 =  a01+b01/pow(amp1[0],0.5)+c01*(amp1[0]);
      double T2 =  a02+b02/pow(amp2[0],0.5)+c02*(amp2[0]);
      double T3 =  a03+b03/pow(amp3[0],0.5)+c03*(amp3[0]);
      double T4 =  a04+b04/pow(amp4[0],0.5)+c04*(amp4[0]);
      
      double TOFa = ((tdc1[0]+tdc2[0])/2.-(tdc3[0]+tdc4[0])/2.-5.94)-T1;
      double TOFb = TOFa-T2;
      double TOFc = TOFb-(T3+T4)/2.;
      double TOF1 = TOFc;//-T4;

      double T11 =  a11+b11/pow(amp1[0],0.5)+c11*(amp1[0]);
      double T12 =  a12+b12/pow(amp2[0],0.5)+c12*(amp2[0]);
      double T13 =  a13+b13/pow(amp3[0],0.5)+c13*(amp3[0]);
      double T14 =  a14+b14/pow(amp4[0],0.5)+c14*(amp4[0]);

      double TOFa2 = TOF1-T11;
      double TOFb2 = TOFa2-T12;
      double TOFc2 = TOFb2-(T13+T14)/2.;
      double TOF2  = TOFc2;

      double T21 =  a21+b21/pow(amp1[0],0.5)+c21*(amp1[0]);
      double T22 =  a22+b22/pow(amp2[0],0.5)+c22*(amp2[0]);
      double T23 =  a23+b23/pow(amp3[0],0.5)+c23*(amp3[0]);
      double T24 =  a24+b24/pow(amp4[0],0.5)+c24*(amp4[0]);
      
      double TOFa3 = TOF2-T21;
      double TOFb3 = TOFa3-T22;
      double TOFc3 = TOFb3-(T23+T24)/2.;
      double TOF   = TOFc3;

      double Amp1 = amp1[0];
      double Amp2 = amp2[0];
      double Amp3 = amp3[0];
      double Amp4 = amp4[0];
      
      if( Amp1>100 && Amp2>100 && Amp3>100 && Amp4>100){
	hist[0]->Fill(amp1[0],TOF+1.78);
	hist[0]->GetXaxis()->SetTitle("AMP1");
	hist[0]->GetYaxis()->SetTitle("TOF");
	hist[1]->Fill(amp2[0],TOF+1.78);
	hist[1]->GetXaxis()->SetTitle("AMP2");
	hist[1]->GetYaxis()->SetTitle("TOF");
	hist[2]->Fill(amp3[0],TOF+1.78);
	hist[2]->GetXaxis()->SetTitle("AMP3");
	hist[2]->GetYaxis()->SetTitle("TOF");
	hist[3]->Fill(amp4[0],TOF+1.78);
	hist[3]->GetXaxis()->SetTitle("AMP4");
	hist[3]->GetYaxis()->SetTitle("TOF");
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

  c1->Print("tcorr3.pdf)");

}

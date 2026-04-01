#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <fstream>   

void resolution()
{
  TFile *fin = new TFile("../../test_run018.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double ltdc1[16],ltdc2[16];
  double ttdc1[16],ttdc2[16];

  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);
  tree->SetBranchAddress("ttdc1", ttdc1);
  tree->SetBranchAddress("ttdc2", ttdc2);

  TH1F *hist[1];
  hist[0] = new TH1F("h1", "TOF1", 1000, -3, 3);

  double a01,b01,c01;
  double a02,b02,c02;
  char check[256];
  ifstream ifs1("corr1.dat");
  
  while(ifs1.getline(check,256)){
    stringstream st1;
    st1<<check;
    if(strlen(check)==0) continue;
    else{
      st1 >> a01 >> b01 >> c01 
	  >> a02 >> b02 >> c02; 
    }
  }
  
  cout<< a01 << "  " << b01 << "  " << c01 <<endl;
  cout<< a02 << "  " << b02 << "  " << c02 <<endl;

  double a11,b11,c11;
  double a12,b12,c12;
  char check2[256];
  ifstream ifs2("corr2.dat");
  
  while(ifs2.getline(check2,256)){
    stringstream st2;
    st2<<check2;
    if(strlen(check2)==0) continue;
    else{
      st2 >> a11 >> b11 >> c11 
	  >> a12 >> b12 >> c12; 
    }
  }
  
  cout<< "****" << endl;
  cout<< a11 << "  " << b11 << "  " << c11 << endl;
  cout<< a12 << "  " << b12 << "  " << c12 << endl;

  double a21,b21,c21;
  double a22,b22,c22;
  char check3[256];
  ifstream ifs3("corr3.dat");
  
  while(ifs3.getline(check3,256)){
    stringstream st3;
    st3<<check3;
    if(strlen(check3)==0) continue;
    else{
      st3 >> a21 >> b21 >> c21 
	  >> a22 >> b22 >> c22; 
    }
  }
  
  cout<< "****" << endl;
  cout<< a21 << "  " << b21 << "  " << c21 << endl;
  cout<< a22 << "  " << b22 << "  " << c22 << endl;

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if(181.0<ltdc1[0]&&ltdc1[0]<190.0&&181.0<ltdc2[0]&&ltdc2[0]<190.0&&
       177.0<ttdc1[0]&&ttdc1[0]<186.0&&177.0<ttdc2[0]&&ttdc2[0]<186.0){

      double w1 = ltdc1[0]-ttdc1[0]-2.5;
      double w2 = ltdc2[0]-ttdc2[0]-2.0;
      
      double T1 =  a01+b01*w1+c01*w1*w1;
      double T2 =  a02+b02*w2+c02*w2*w2;

      double TOFa = ((ltdc1[0]-ltdc2[0])+0.61)-T1;
      double TOF1 = TOFa-T2;

      double T11 =  a11+b11*w1+c11*w1*w1;
      double T12 =  a12+b12*w2+c12*w2*w2;
      
      double TOFa2 = TOF1-T11;
      double TOF2  = TOFa2-T12;

      double T21 =  a21+b21*w1+c21*w1*w1;
      double T22 =  a22+b22*w2+c22*w2*w2;
      
      double TOFa3 = TOF2-T21;
      double TOF   = TOFa3-T22;

      if(w1>0&&w2>0){
	hist[0]->Fill(TOF-0.04);
      }
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(1,1);
  for(int i=0; i<1; ++i)
    {
      c1->cd(i+1);
      hist[i]->Draw();
    }
  
  hist[0]->Fit("gaus","","",-0.25,0.25);

  hist[0]->GetXaxis()->SetTitle("time[ns]");
  hist[0]->GetYaxis()->SetTitle("counts");

  c1 -> Print("resolution.pdf)");
}

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <fstream>   

void resolution2()
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

  const int Nh = 8;
  TH1F *hist[Nh];
  hist[0] = new TH1F("h1", "TOF1", 300, -1, 1);
  hist[1] = new TH1F("h2", "TOF2", 300, -1, 1);
  hist[2] = new TH1F("h3", "TOF3", 300, -1, 1);
  hist[3] = new TH1F("h4", "TOF4", 300, -1, 1);
  hist[4] = new TH1F("h5", "TOF5", 100, -1, 1);
  hist[5] = new TH1F("h6", "TOF6", 100, -1, 1);
  hist[6] = new TH1F("h7", "TOF7",  50, -1, 1);
  hist[7] = new TH1F("h8", "TOF8",  50, -1, 1);

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
  
  cout<< a01 << "  " << b01 << "  " << c01 <<endl;
  cout<< a02 << "  " << b02 << "  " << c02 <<endl;
  cout<< a03 << "  " << b03 << "  " << c03 <<endl;
  cout<< a04 << "  " << b04 << "  " << c04 <<endl;

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
      
      if( Amp1>50 && Amp2>50 && Amp3>100 && Amp4>100){
	hist[0]->Fill(TOF+1.78);
      }
      if( Amp1>100 && Amp2>100 && Amp3>100 && Amp4>100){
	hist[1]->Fill(TOF+1.78);
      }
      if( Amp1>150 && Amp2>150 && Amp3>100 && Amp4>100){
	hist[2]->Fill(TOF+1.78);
      }
      if( Amp1>200 && Amp2>200 && Amp3>100 && Amp4>100){
	hist[3]->Fill(TOF+1.79);
      }
      if( Amp1>250 && Amp2>250 && Amp3>100 && Amp4>100){
	hist[4]->Fill(TOF+1.81);
      }
      if( Amp1>300 && Amp2>300 && Amp3>100 && Amp4>100){
	hist[5]->Fill(TOF+1.81);
      }
      if( Amp1>350 && Amp2>350 && Amp3>100 && Amp4>100){
	hist[6]->Fill(TOF+1.81);
      }
      if( Amp1>400 && Amp2>400 && Amp3>100 && Amp4>100){
	hist[7]->Fill(TOF+1.80);
      }
    }
  }
  
  double resol[Nh], resolE[Nh];
  double cut[Nh] = { 50., 100., 150., 200., 250., 300., 350., 400.};
  double cutE[Nh] = {0., 0., 0., 0., 0., 0., 0., 0.};

  TF1 *fit1 = new TF1("fit1","gaus");

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(4,2);
  for(int i=0; i<Nh; ++i){
    c1->cd(i+1);
    hist[i]->Draw();
    
    hist[i]->Fit("fit1", "", "", -0.17, 0.17);

    resol[i] = (fit1->GetParameter(2))*1000.0;
    resolE[i] = (fit1->GetParError(2))*1000.0;
    
    hist[i]->GetXaxis()->SetTitle("time[ns]");
    hist[i]->GetYaxis()->SetTitle("counts");
  }
  for(int i=0; i<Nh; ++i){
    cout << "Resolution = " << resol[i] << " +- " << resolE[i] << " [ps]" << endl;
  }

  TCanvas *c2 = new TCanvas("c2","c2"); 
  c2->cd(1)->SetLeftMargin(0.15);
  c2->cd(1)->SetRightMargin(0.1);
  c2->cd(1)->SetTopMargin(0.1);
  c2->cd(1)->SetBottomMargin(0.15);
  c2->cd(1)->SetGrid();
  TH1F* Hc1=c2->cd(1)->DrawFrame( 0.0, 60.0, 450.0, 120.0 );
  Hc1->GetXaxis()->SetLabelSize(0.040);
  Hc1->GetYaxis()->SetLabelSize(0.040);
  Hc1->GetYaxis()->SetTitle("Resolution [ps]");
  Hc1->GetYaxis()->SetTitleSize(0.05);
  Hc1->GetYaxis()->SetTitleOffset(1.0);
  Hc1->GetXaxis()->SetTitle("Pulse Height Cut Position [mV]");
  Hc1->GetXaxis()->SetTitleSize(0.05);
  
  TGraphErrors *graph1 = new TGraphErrors(Nh, cut, resol, cutE, resolE);
  graph1->SetMarkerStyle(8);
  graph1->SetMarkerColor(1);
  graph1->Draw("PEsame");

  c1 -> Print("resolution2.pdf)");
  c2 -> Print("resolution2g.pdf)");
}

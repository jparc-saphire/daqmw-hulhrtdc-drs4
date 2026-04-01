#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <fstream>   

void resolution2()
{
  TFile *fin = new TFile("../../test_run023.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double amp1[16],amp2[16];
  double tdc1[16],tdc2[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("tdc1", tdc1);
  tree->SetBranchAddress("tdc2", tdc2);

  const int Nh = 8;
  TH1F *hist[Nh];
  hist[0] = new TH1F("h1", "TOF1", 300, -1, 1);
  hist[1] = new TH1F("h2", "TOF2", 200, -1, 1);
  hist[2] = new TH1F("h3", "TOF3", 300, -1, 1);
  hist[3] = new TH1F("h4", "TOF4", 300, -1, 1);
  hist[4] = new TH1F("h5", "TOF5", 150, -1, 1);
  hist[5] = new TH1F("h6", "TOF6", 150, -1, 1);
  hist[6] = new TH1F("h7", "TOF7", 120, -1, 1);
  hist[7] = new TH1F("h8", "TOF8", 120, -1, 1);

  TH1F *hist21[Nh], *hist22[Nh];
  hist21[0] = new TH1F("h211", "PE211", 300, 0, 30);
  hist21[1] = new TH1F("h212", "PE212", 300, 0, 30);
  hist21[2] = new TH1F("h213", "PE213", 300, 0, 30);
  hist21[3] = new TH1F("h214", "PE214", 300, 0, 30);
  hist21[4] = new TH1F("h215", "PE215", 300, 0, 30);
  hist21[5] = new TH1F("h216", "PE216", 300, 0, 30);
  hist21[6] = new TH1F("h217", "PE217", 300, 0, 30);
  hist21[7] = new TH1F("h218", "PE218", 300, 0, 30);

  hist22[0] = new TH1F("h221", "PE221", 300, 0, 30);
  hist22[1] = new TH1F("h222", "PE222", 300, 0, 30);
  hist22[2] = new TH1F("h223", "PE223", 300, 0, 30);
  hist22[3] = new TH1F("h224", "PE224", 300, 0, 30);
  hist22[4] = new TH1F("h225", "PE225", 300, 0, 30);
  hist22[5] = new TH1F("h226", "PE226", 300, 0, 30);
  hist22[6] = new TH1F("h227", "PE227", 300, 0, 30);
  hist22[7] = new TH1F("h228", "PE228", 300, 0, 30);

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
    if( tdc1[0]>0&&tdc2[0]>0&&amp1[0]>0&&amp2[0]>0){

      double pe1 = 0.0856*amp1[0]+2.09;
      double pe2 = 0.0876*amp2[0]+2.25;

      double Amp1 = pe1-3.5;
      double Amp2 = pe2-3.7;
      
      double T1 =  a01+b01/pow(Amp1,0.5)+c01*(Amp1);
      double T2 =  a02+b02/pow(Amp2,0.5)+c02*(Amp2);
      
      double TOFa = ((tdc1[0]-tdc2[0])-0.55)-T1;
      double TOF1 = TOFa-T2;

      double T11 =  a11+b11/pow(Amp1,0.5)+c11*(Amp1);
      double T12 =  a12+b12/pow(Amp2,0.5)+c12*(Amp2);

      double TOFa2 = TOF1-T11;
      double TOF2  = TOFa2-T12;

      double T21 =  a21+b21/pow(Amp1,0.5)+c21*(Amp1);
      double T22 =  a22+b22/pow(Amp2,0.5)+c22*(Amp2);
      
      double TOFa3 = TOF2-T21;
      double TOF   = TOFa3-T22;
 
      double rA1 = Amp1+3.5;
      double rA2 = Amp2+3.7;

      if( rA1>2.5 && rA2>2.5 ){
	hist[0]->Fill(TOF-0.021);
	hist21[0]->Fill(rA1);
	hist22[0]->Fill(rA2);
      }
      if( rA1>5.0 && rA2>5.0){
	hist[1]->Fill(TOF-0.022);
	hist21[1]->Fill(rA1);
	hist22[1]->Fill(rA2);
      }
      if( rA1>7.5 && rA2>7.5){
	hist[2]->Fill(TOF-0.016);
	hist21[2]->Fill(rA1);
	hist22[2]->Fill(rA2);
      }
      if( rA1>10.0 && rA2>10.0){
	hist[3]->Fill(TOF-0.018);
	hist21[3]->Fill(rA1);
	hist22[3]->Fill(rA2);
      }
      if( rA1>12.5 && rA2>12.5){
	hist[4]->Fill(TOF-0.016);
	hist21[4]->Fill(rA1);
	hist22[4]->Fill(rA2);
      }
      if( rA1>15.0 && rA2>15.0){
	hist[5]->Fill(TOF-0.016);
	hist21[5]->Fill(rA1);
	hist22[5]->Fill(rA2);
      }
      if( rA1>17.5 && rA2>17.5){
	hist[6]->Fill(TOF-0.019);
	hist21[6]->Fill(rA1);
	hist22[6]->Fill(rA2);
      }
      if( rA1>20.0 && rA2>20.0){
	hist[7]->Fill(TOF-0.011);
	hist21[7]->Fill(rA1);
	hist22[7]->Fill(rA2);
      }
    }
  }

  double resol[Nh], resolE[Nh];
  double iresol[Nh], iresolE[Nh];
  double mean[Nh], mean1[Nh], mean2[Nh];
  double meanE[Nh] = {0., 0., 0., 0., 0., 0., 0., 0.};

  TF1 *fit1 = new TF1("fit1","gaus");

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(4,2);
  for(int i=0; i<Nh; ++i){
    c1->cd(i+1);
    hist[i]->Draw();
    
    hist[i]->Fit("fit1", "", "", -0.24+0.016*i, 0.24-0.016*i);

    resol[i] = (fit1->GetParameter(2))*1000.0;
    resolE[i] = (fit1->GetParError(2))*1000.0;
    mean1[i] = hist21[i]->GetMean();
    mean2[i] = hist22[i]->GetMean();
    mean[i] = (mean1[i]+mean2[i])/2.;

    iresol[i] = resol[i]/sqrt(2.0);
    iresolE[i] = resolE[i]/sqrt(2.0);

    hist[i]->GetXaxis()->SetTitle("time[ns]");
    hist[i]->GetYaxis()->SetTitle("counts");
  }

  for(int i=0; i<Nh; ++i){
    cout<< "****" << endl;
    cout << "Resolution = " << resol[i] << " +- " << resolE[i] << " [ps]" << endl;
    cout << "Mean1:Mean2 = " << mean1[i] << ":" << mean2[i] << endl;
    cout << "--->Mean = " << mean[i] << endl;
  }

  TCanvas *c2 = new TCanvas("c2","c2"); 
  c2->cd(1)->SetLeftMargin(0.15);
  c2->cd(1)->SetRightMargin(0.1);
  c2->cd(1)->SetTopMargin(0.1);
  c2->cd(1)->SetBottomMargin(0.15);
  c2->cd(1)->SetGrid();
  TH1F* Hc1=c2->cd(1)->DrawFrame( 5.0,  0.0, 25.0, 170.0 );
  Hc1->GetXaxis()->SetLabelSize(0.040);
  Hc1->GetYaxis()->SetLabelSize(0.040);
  Hc1->GetYaxis()->SetTitle("Resolution [ps]");
  Hc1->GetYaxis()->SetTitleSize(0.05);
  Hc1->GetYaxis()->SetTitleOffset(1.0);
  Hc1->GetXaxis()->SetTitle("#of Mean P.E");
  Hc1->GetXaxis()->SetTitleSize(0.05);
  
  TGraphErrors *graph1 = new TGraphErrors(Nh, mean, resol, meanE, resolE);
  graph1->SetMarkerStyle(8);
  graph1->SetMarkerColor(1);
  graph1->Draw("PEsame");

  TGraphErrors *graph2 = new TGraphErrors(Nh, mean, iresol, meanE, iresolE);
  graph2->SetMarkerStyle(8);
  graph2->SetMarkerColor(2);
  graph2->Draw("PEsame");

  c1 -> Print("resolution2.pdf");
  c2 -> Print("resolution2g.pdf");
}

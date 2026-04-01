#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"

void timewalkcorr3()
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
  hist[0] = new TH2F("h1", "TOF1vsAMP1",  600, 0,  600, 1000, -3, 3);
  hist[1] = new TH2F("h2", "TOF1vsAMP2",  600, 0,  600, 1000, -3, 3);
  hist[2] = new TH2F("h3", "TOF1vsAMP3", 1000, 0, 1000, 1000, -3, 3);
  hist[3] = new TH2F("h4", "TOF1vsAMP4", 1000, 0, 1000, 1000, -3, 3);

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

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if( tdc1[0]>0.&&tdc2[0]>0.&&tdc3[0]>0.&&tdc4[0]>0.&&
	amp1[0]>0.&&amp2[0]>0.&&amp3[0]>0.&&amp4[0]>0.){

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
      double TOFc2 = TOFb2-(T13+T14);
      double TOF   = TOFc2;
      
      double Amp1 = amp1[0];
      double Amp2 = amp2[0];
      double Amp3 = amp3[0];
      double Amp4 = amp4[0];
      

      if( Amp1>100 && Amp2>100 && Amp3>100 && Amp4>100){
	hist[0]->Fill(amp1[0],TOF);
	hist[0]->GetXaxis()->SetTitle("AMP1");
	hist[0]->GetYaxis()->SetTitle("TOF");
	hist[1]->Fill(amp2[0],TOF);
	hist[1]->GetXaxis()->SetTitle("AMP2");
	hist[1]->GetYaxis()->SetTitle("TOF");
	hist[2]->Fill(amp3[0],TOF);
	hist[2]->GetXaxis()->SetTitle("AMP3");
	hist[2]->GetYaxis()->SetTitle("TOF");
	hist[3]->Fill(amp4[0],TOF);
	hist[3]->GetXaxis()->SetTitle("AMP4");
	hist[3]->GetYaxis()->SetTitle("TOF");
      } 
    }
  }

  ///////////////////////////////////////////////////
  //Plot
  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(2,2);
  for(int i=0; i<4; ++i){
    c1->cd(i+1);
    hist[i]->Draw("colz");
  }
  
  ///////////////////////////////////////////////////
  //Fit1

  int xNofBin1 = 500;

  TCanvas *c2 = new TCanvas("c2","c2");
  c2->Divide(6,5);
  TF1 *fit1 = new TF1("fit1","gaus");
  int NofProject1 = 30;
  int stepProject1 = xNofBin1/NofProject1;

  std::vector<double> xval1;
  std::vector<double> yval1;
  xval1.resize(NofProject1);
  yval1.resize(NofProject1);

  for(int i = 0; i<NofProject1; ++i){
    c2->cd(1+i);
    int bin_min1 = i*stepProject1+1;
    int bin_max1 = (i+1)*stepProject1;
    
    // cout<< "*****" << endl;
    // cout<< bin_min1 << ":" << bin_max1 << endl;
    
    TH1D *tmp1 = (TH1D*) hist[0]->ProjectionY("", bin_min1, bin_max1)->Clone();
    double center1 = tmp1->GetBinCenter(tmp1->GetMaximumBin());
    tmp1->Fit("fit1", "Q", "", center1-2.0, center1+2.0);

    TH1D *tmp2 = (TH1D*) hist[0]->ProjectionX("", bin_min1, bin_max1)->Clone();
    
    double x_min1 = tmp2->GetBinCenter(bin_min1);
    double x_max1 = tmp2->GetBinCenter(bin_max1);
    double x_center1 = (x_max1 + x_min1)/2.0;
    
    // cout<< "-->" << x_min1 << ":" << x_max1 << endl;
    // cout<< "-->" << x_center1 << endl;
        
    xval1[i] = x_center1;
    yval1[i] = fit1->GetParameter(1);
    
    //cout<< "------>" << xval1[i] << ":" << yval1[i] << endl;
  }
  
  c1->cd(1);
  TGraph *graph1 = new TGraph(NofProject1, &(xval1[0]), &(yval1[0]));
  graph1->SetMarkerStyle(8);
  graph1->SetMarkerColor(1);
  graph1->Draw("psame");
  
  TF1 *fn1 = new TF1("fn1","[0]+[1]/sqrt(x)+[2]*x");
  fn1->SetParNames("a21","b21","c21");
  graph1->Fit("fn1","","", 100,450);
  double a21 = fn1->GetParameter(0);
  double b21 = fn1->GetParameter(1);
  double c21 = fn1->GetParameter(2);

  ///////////////////////////////////////////////////
  //Fit2

  int xNofBin2 = 500;
  
  TCanvas *c3 = new TCanvas("c3","c3");
  c3->Divide(6,5);
  TF1 *fit2 = new TF1("fit2","gaus");
  int NofProject2 =30;
  int stepProject2 = xNofBin2/NofProject2;

  std::vector<double> xval2;
  std::vector<double> yval2;
  xval2.resize(NofProject2);
  yval2.resize(NofProject2);
  
  for(int i = 0; i<NofProject2; ++i){
    c3->cd(1+i);
    int bin_min1 = i*stepProject2+1;
    int bin_max1 = (i+1)*stepProject2;

    TH1D *tmp1 = (TH1D*) hist[1]->ProjectionY("", bin_min1, bin_max1)->Clone();
    double center1 = tmp1->GetBinCenter(tmp1->GetMaximumBin());
    tmp1->Fit("fit2", "Q", "", center1-2.0, center1+2.0);
    
    TH1D *tmp2 = (TH1D*) hist[1]->ProjectionX("", bin_min1, bin_max1)->Clone();
    
    double x_min1 = tmp2->GetBinCenter(bin_min1);
    double x_max1 = tmp2->GetBinCenter(bin_max1);
    double x_center1 = (x_max1 + x_min1)/2.0;
            
    xval2[i] = x_center1;
    yval2[i] = fit2->GetParameter(1);
  }
  
  c1->cd(2);
  TGraph *graph2 = new TGraph(NofProject2, &(xval2[0]), &(yval2[0]));
  graph2->SetMarkerStyle(8);
  graph2->SetMarkerColor(1);
  graph2->Draw("psame");

  TF1 *fn2 = new TF1("fn2","[0]+[1]/sqrt(x)+[2]*x");
  fn2->SetParNames("a22","b22","c22");
  graph2->Fit("fn2","","", 100,450);  
  double a22 = fn2->GetParameter(0);
  double b22 = fn2->GetParameter(1);
  double c22 = fn2->GetParameter(2);

  ///////////////////////////////////////////////////
  //Fit3

  int xNofBin3 = 1000;

  TCanvas *c4 = new TCanvas("c4","c4");
  c4->Divide(6,5);
  TF1 *fit3 = new TF1("fit3","gaus");
  int NofProject3 =30;
  int stepProject3 = xNofBin3/NofProject3;
  
  std::vector<double> xval3;
  std::vector<double> yval3;
  xval3.resize(NofProject3);
  yval3.resize(NofProject3);

  for(int i = 0; i<NofProject3; ++i){
    c4->cd(1+i);
    int bin_min1 = i*stepProject3+1;
    int bin_max1 = (i+1)*stepProject3;
         
    TH1D *tmp1 = (TH1D*) hist[2]->ProjectionY("", bin_min1, bin_max1)->Clone();
    double center1 = tmp1->GetBinCenter(tmp1->GetMaximumBin());
    tmp1->Fit("fit3", "Q", "", center1-2.0, center1+2.0);
    
    TH1D *tmp2 = (TH1D*) hist[2]->ProjectionX("", bin_min1, bin_max1)->Clone();
    
    double x_min1 = tmp2->GetBinCenter(bin_min1);
    double x_max1 = tmp2->GetBinCenter(bin_max1);
    double x_center1 = (x_max1 + x_min1)/2.0;
            
    xval3[i] = x_center1;
    yval3[i] = fit3->GetParameter(1);
  }
  
  c1->cd(3);
  TGraph *graph3 = new TGraph(NofProject3, &(xval3[0]), &(yval3[0]));
  graph3->SetMarkerStyle(8);
  graph3->SetMarkerColor(1);
  graph3->Draw("psame");

  TF1 *fn3 = new TF1("fn3","[0]+[1]/sqrt(x)+[2]*x");
  fn3->SetParNames("a23","b23","c23");
  graph3->Fit("fn3","","", 100,800);  
  double a23 = fn3->GetParameter(0);
  double b23 = fn3->GetParameter(1);
  double c23 = fn3->GetParameter(2);

  ///////////////////////////////////////////////////
  //Fit4

  int xNofBin4 = 1000;

  TCanvas *c5 = new TCanvas("c5","c5");
  c5->Divide(6,5);
  TF1 *fit4 = new TF1("fit4","gaus");
  int NofProject4 =30;
  int stepProject4 = xNofBin4/NofProject4;

  std::vector<double> xval4;
  std::vector<double> yval4;
  xval4.resize(NofProject4);
  yval4.resize(NofProject4);

  for(int i = 0; i<NofProject4; ++i){
    c5->cd(1+i);
    int bin_min1 = i*stepProject4+1;
    int bin_max1 = (i+1)*stepProject4;

    TH1D *tmp1 = (TH1D*) hist[3]->ProjectionY("", bin_min1, bin_max1)->Clone();
    double center1 = tmp1->GetBinCenter(tmp1->GetMaximumBin());
    tmp1->Fit("fit4", "Q", "", center1-2.0, center1+2.0);
    
    TH1D *tmp2 = (TH1D*) hist[3]->ProjectionX("", bin_min1, bin_max1)->Clone();
    
    double x_min1 = tmp2->GetBinCenter(bin_min1);
    double x_max1 = tmp2->GetBinCenter(bin_max1);
    double x_center1 = (x_max1 + x_min1)/2.0;
            
    xval4[i] = x_center1;
    yval4[i] = fit4->GetParameter(1);
  }
  
  c1->cd(4);
  TGraph *graph4 = new TGraph(NofProject4, &(xval4[0]), &(yval4[0]));
  graph4->SetMarkerStyle(8);
  graph4->SetMarkerColor(1);
  graph4->Draw("psame");

  TF1 *fn4 = new TF1("fn4","[0]+[1]/sqrt(x)+[2]*x");
  fn4->SetParNames("a24","b24","c24");
  graph4->Fit("fn4","","", 100,900);  
  double a24 = fn4->GetParameter(0);
  double b24 = fn4->GetParameter(1);
  double c24 = fn4->GetParameter(2);

  TString fout_name = ("corr3.dat");
  ofstream fout(fout_name.Data());
  fout << a21 << "  " << b21 << "  " << c21 << "  " 
       << a22 << "  " << b22 << "  " << c22 << "  " 
       << a23 << "  " << b23 << "  " << c23 << "  " 
       << a24 << "  " << b24 << "  " << c24 << "  " 
       << endl;

  c1->Print("timewalkcorr3.pdf");
}

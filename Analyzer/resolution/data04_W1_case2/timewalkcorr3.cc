#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"

void timewalkcorr3()
{
  TFile *fin = new TFile("../../test_run076.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double amp1[16],amp2[16];
  double ltdc1[16],ltdc2[16],ltdc3[16],ltdc4[16];
  double ttdc1[16],ttdc2[16],ttdc3[16],ttdc4[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);
  tree->SetBranchAddress("ltdc3", ltdc3);
  tree->SetBranchAddress("ltdc4", ltdc4);
  tree->SetBranchAddress("ttdc1", ttdc1);
  tree->SetBranchAddress("ttdc2", ttdc2);
  tree->SetBranchAddress("ttdc3", ttdc3);
  tree->SetBranchAddress("ttdc4", ttdc4);

  TH2F *hist[2];
  hist[0] = new TH2F("h1", "TOF1vsWIDTH1", 100, 0, 6, 50, -3, 3);
  hist[1] = new TH2F("h2", "TOF1vsWIDTH2", 100, 0, 6, 50, -3, 3);

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
  
  cout<< a01 << "  " << b01 << "  " << c01 << endl;
  cout<< a02 << "  " << b02 << "  " << c02 << endl;

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

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if(172.0<ltdc1[0]&&ltdc1[0]<182.0&&172.0<ltdc2[0]&&ltdc2[0]<182.0&& 
       160.0<ltdc3[0]&&ltdc3[0]<170.0&&159.0<ltdc4[0]&&ltdc4[0]<169.0&&
       150.0<ttdc3[0]&&ttdc3[0]<165.0&&151.0<ttdc4[0]&&ttdc4[0]<166.0){
                
      double pe1 = 0.109*amp1[0]+0.80;
      double pe2 = 0.102*amp2[0]+1.24;

      double w1 = ltdc3[0]-ttdc3[0]-6.8;
      double w2 = ltdc4[0]-ttdc4[0]-6.2;
      
      // double T1 =  a01+b01*w1+c01*w1*w1;
      // double T2 =  a02+b02*w2+c02*w2*w2;
      double T1 =  a01+b01/pow(w1,0.5)+c01*(w1);
      double T2 =  a02+b02/pow(w2,0.5)+c02*(w2);

      double TOFa = ((ltdc1[0]-ltdc2[0])+0.23)-T1;
      double TOF1 = TOFa-T2;

      // double T11 =  a11+b11*w1+c11*w1*w1;
      // double T12 =  a12+b12*w2+c12*w2*w2;
      double T11 =  a11+b11/pow(w1,0.5)+c11*(w1);
      double T12 =  a12+b12/pow(w2,0.5)+c12*(w2);

      double TOFa2 = TOF1-T11;
      double TOF   = TOFa2-T12;

      if( w1>0.0 && w2>0.0 && pe1>10.0 && pe2>10.0  ){
	hist[0]->Fill(w1,TOF);
	hist[0]->GetXaxis()->SetTitle("WIDTH1");
	hist[0]->GetYaxis()->SetTitle("TOF");
	hist[1]->Fill(w2,TOF);
	hist[1]->GetXaxis()->SetTitle("WIDTH2");
	hist[1]->GetYaxis()->SetTitle("TOF");
      }
    }
  }

  ///////////////////////////////////////////////////
  //Plot
  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(1,2);
  for(int i=0; i<2; ++i){
    c1->cd(i+1);
    hist[i]->Draw("colz");
  }
  
  ///////////////////////////////////////////////////
  //Fit range

  double r11 =  0.3;
  double r12 =  5.2;
  double r21 =  0.3;
  double r22 =  5.2;

  int h =  6;
  int v =  5;
  int np= 30;
  double range = 0.5;

  ///////////////////////////////////////////////////
  //Fit1

  int xNofBin1 = 100;

  TCanvas *c2 = new TCanvas("c2","c2");
  c2->Divide(h,v);
  TF1 *fit1 = new TF1("fit1","gaus");
  int NofProject1 = np;
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
    tmp1->Fit("fit1", "Q", "", center1-range, center1+range);

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
  graph1->Fit("fn1","","", r11,r12);
  double a21 = fn1->GetParameter(0);
  double b21 = fn1->GetParameter(1);
  double c21 = fn1->GetParameter(2);

  // TF1 *fn1 = new TF1("fn1","[0]+[1]*x+[2]*x*x");
  // fn1->SetParNames("a21","b21","c21");
  // graph1->Fit("fn1","","", r11,r12);
  // double a21 = fn1->GetParameter(0);
  // double b21 = fn1->GetParameter(1);
  // double c21 = fn1->GetParameter(2);

  ///////////////////////////////////////////////////
  //Fit2

  int xNofBin2 = 100;
  
  TCanvas *c3 = new TCanvas("c3","c3");
  c3->Divide(h,v);
  TF1 *fit2 = new TF1("fit2","gaus");
  int NofProject2 = np;
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
    tmp1->Fit("fit2", "Q", "", center1-range, center1+range);
    
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
  graph2->Fit("fn2","","", r21,r22);  
  double a22 = fn2->GetParameter(0);
  double b22 = fn2->GetParameter(1);
  double c22 = fn2->GetParameter(2);

  // TF1 *fn2 = new TF1("fn2","[0]+[1]*x+[2]*x*x");
  // fn2->SetParNames("a22","b22","c22");
  // graph2->Fit("fn2","","", r21,r22);  
  // double a22 = fn2->GetParameter(0);
  // double b22 = fn2->GetParameter(1);
  // double c22 = fn2->GetParameter(2);

  TString fout_name = ("corr3.dat");
  ofstream fout(fout_name.Data());
  fout << a21 << "  " << b21 << "  " << c21 << "  " 
       << a22 << "  " << b22 << "  " << c22 << "  " 
       << endl;

  c1->Print("timewalkcorr3.pdf");
}

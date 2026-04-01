#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"

void timewalkcorr2()
{
  TFile *fin = new TFile("../../test_run031.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double ltdc1[16],ltdc2[16];
  double ttdc1[16],ttdc2[16];

  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);
  tree->SetBranchAddress("ttdc1", ttdc1);
  tree->SetBranchAddress("ttdc2", ttdc2);

  TH2F *hist[2];
  hist[0] = new TH2F("h1", "TOF1vsWIDTH1", 1000, 0, 5, 50, -3, 3);
  hist[1] = new TH2F("h2", "TOF1vsWIDTH2", 1000, 0, 5, 50, -3, 3);

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

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if(180.0<ltdc1[0]&&ltdc1[0]<190.0&&180.0<ltdc2[0]&&ltdc2[0]<190.0&&
       176.0<ttdc1[0]&&ttdc1[0]<186.0&&176.0<ttdc2[0]&&ttdc2[0]<186.0){
      
      double w1 = exp(ltdc1[0]-ttdc1[0])/100.;
      double w2 = exp(ltdc2[0]-ttdc2[0])/100.;
      
      double T1 =  a01+b01/pow(w1,0.5)+c01*w1;
      double T2 =  a02+b02/pow(w2,0.5)+c02*w2;
      // double T1 =  a01+b01*w1+c01*w1*w1;
      // double T2 =  a02+b02*w2+c02*w2*w2;

      double TOFa = ((ltdc1[0]-ltdc2[0])+0.58)-T1;
      double TOF  = TOFa-T2;
      
      hist[0]->Fill(w1,TOF);
      hist[0]->GetXaxis()->SetTitle("WIDTH1");
      hist[0]->GetYaxis()->SetTitle("TOF");
      hist[1]->Fill(w2,TOF);
      hist[1]->GetXaxis()->SetTitle("WIDTH2");
      hist[1]->GetYaxis()->SetTitle("TOF");
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

  double r11 =  0.4;
  double r12 =  4.5;
  double r21 =  0.4;
  double r22 =  4.5;

  int h =  5;
  int v =  6;
  int np= 30;
  double range = 0.5;

  ///////////////////////////////////////////////////
  //Fit1

  int xNofBin1 = 1000;

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
  
  // TF1 *fn1 = new TF1("fn1","[0]+[1]*x+[2]*x*x");
  // fn1->SetParNames("a11","b11","c11");
  // graph1->Fit("fn1","","", r11,r12);
  // double a11 = fn1->GetParameter(0);
  // double b11 = fn1->GetParameter(1);
  // double c11 = fn1->GetParameter(2);
  
  TF1 *fn1 = new TF1("fn1","[0]+[1]/sqrt(x)+[2]*x");
  fn1->SetParNames("a11","b11","c11");
  graph1->Fit("fn1","","", r11, r12);
  double a11 = fn1->GetParameter(0);
  double b11 = fn1->GetParameter(1);
  double c11 = fn1->GetParameter(2);

  // TF1 *fn1 = new TF1("fn1","[0]+[1]*x+[2]*x*x");
  // fn1->SetParNames("a11","b11","c11");
  // graph1->Fit("fn1","","", r11,r12);
  // double a11 = fn1->GetParameter(0);
  // double b11 = fn1->GetParameter(1);
  // double c11 = fn1->GetParameter(2);

  ///////////////////////////////////////////////////
  //Fit2

  int xNofBin2 = 1000;
  
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

  // TF1 *fn2 = new TF1("fn2","[0]+[1]*x+[2]*x*x");
  // fn2->SetParNames("a12","b12","c12");
  // graph2->Fit("fn2","","", r21,r22);  
  // double a12 = fn2->GetParameter(0);
  // double b12 = fn2->GetParameter(1);
  // double c12 = fn2->GetParameter(2);

  TF1 *fn2 = new TF1("fn2","[0]+[1]/sqrt(x)+[2]*x");
  fn2->SetParNames("a12","b12","c12");
  graph2->Fit("fn2","","",r21,r22);  
  double a12 = fn2->GetParameter(0);
  double b12 = fn2->GetParameter(1);
  double c12 = fn2->GetParameter(2);

  // TF1 *fn2 = new TF1("fn2","[0]+[1]*x+[2]*x*x");
  // fn2->SetParNames("a12","b12","c12");
  // graph2->Fit("fn2","","", r21,r22);  
  // double a12 = fn2->GetParameter(0);
  // double b12 = fn2->GetParameter(1);
  // double c12 = fn2->GetParameter(2);

  TString fout_name = ("corr2.dat");
  ofstream fout(fout_name.Data());
  fout << a11 << "  " << b11 << "  " << c11 << "  " 
       << a12 << "  " << b12 << "  " << c12 << "  " 
       << endl;

  c1->Print("timewalkcorr2.pdf");
}

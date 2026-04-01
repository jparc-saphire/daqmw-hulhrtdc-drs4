#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"

void timewalkcorr1()
{
  TFile *fin = new TFile("../../test_run012.root");
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
  hist[0] = new TH2F("h1", "TOF1vsAMP1",  300, 0,  300, 100, -5, 5);
  hist[1] = new TH2F("h2", "TOF1vsAMP2",  300, 0,  300, 100, -5, 5);
  hist[2] = new TH2F("h3", "TOF1vsAMP3",  500, 0,  500, 100, -5, 5);
  hist[3] = new TH2F("h4", "TOF1vsAMP4",  500, 0,  500, 100, -5, 5);

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if( tdc1[0]>0.&&tdc2[0]>0.&&tdc3[0]>0.&&tdc4[0]>0.&&
	amp1[0]>0.&&amp2[0]>0.&&amp3[0]>0.&&amp4[0]>0.){

      double Amp1 = amp1[0]-12.0;
      double Amp2 = amp2[0]-12.0;
      double Amp3 = amp3[0]-12.0;
      double Amp4 = amp4[0]-12.0;
      
      if( Amp1>25 && Amp2>25 && Amp3>25 && Amp4>25){
	hist[0]->Fill(amp1[0]-12.0,(tdc1[0]+tdc2[0])/2.0-(tdc3[0]+tdc4[0])/2.0-7.41);
	hist[0]->GetXaxis()->SetTitle("AMP1");
	hist[0]->GetYaxis()->SetTitle("TOF1");
	
	hist[1]->Fill(amp2[0]-12.0,(tdc1[0]+tdc2[0])/2.0-(tdc3[0]+tdc4[0])/2.0-7.41);
	hist[1]->GetXaxis()->SetTitle("AMP2");
	hist[1]->GetYaxis()->SetTitle("TOF1");
	
	hist[2]->Fill(amp3[0]-12.0,(tdc1[0]+tdc2[0])/2.0-(tdc3[0]+tdc4[0])/2.0-7.41);
	hist[2]->GetXaxis()->SetTitle("AMP3");
	hist[2]->GetYaxis()->SetTitle("TOF1");
	
	hist[3]->Fill(amp4[0]-12.0,(tdc1[0]+tdc2[0])/2.0-(tdc3[0]+tdc4[0])/2.0-7.41);
	hist[3]->GetXaxis()->SetTitle("AMP4");
	hist[3]->GetYaxis()->SetTitle("TOF1");
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

  int xNofBin1 = 300;

  TCanvas *c2 = new TCanvas("c2","c2");
  c2->Divide(5,4);
  TF1 *fit1 = new TF1("fit1","gaus");
  int NofProject1 = 20;
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
        
    xval1[i] = tmp2->GetBinCenter(x_center1);
    yval1[i] = fit1->GetParameter(1);
    
    //cout<< "------>" << xval1[i] << ":" << yval1[i] << endl;
  }
  
  c1->cd(1);
  TGraph *graph1 = new TGraph(NofProject1, &(xval1[0]), &(yval1[0]));
  graph1->SetMarkerStyle(8);
  graph1->SetMarkerColor(1);
  graph1->Draw("psame");
  
  // TF1 *fn1 = new TF1("fn1","[0]+[1]/sqrt(x)");
  // fn1->SetParNames("a01","b01");
  // graph1->Fit("fn1","","", 20, 200);
  // double a01 = fn1->GetParameter(0);
  // double b01 = fn1->GetParameter(1);
  // double c01 = 0.0;

  TF1 *fn1 = new TF1("fn1","[0]+[1]*sqrt(x)+[2]*x");
  fn1->SetParNames("a01","b01","c01");
  graph1->Fit("fn1","","", 15, 300);
  double a01 = fn1->GetParameter(0);
  double b01 = fn1->GetParameter(1);
  double c01 = fn1->GetParameter(2);

  ///////////////////////////////////////////////////
  //Fit2

  int xNofBin2 = 300;
  
  TCanvas *c3 = new TCanvas("c3","c3");
  c3->Divide(5,4);
  TF1 *fit2 = new TF1("fit2","gaus");
  int NofProject2 =20;
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
            
    xval2[i] = tmp2->GetBinCenter(x_center1);
    yval2[i] = fit2->GetParameter(1);
  }
  
  c1->cd(2);
  TGraph *graph2 = new TGraph(NofProject2, &(xval2[0]), &(yval2[0]));
  graph2->SetMarkerStyle(8);
  graph2->SetMarkerColor(1);
  graph2->Draw("psame");

  // TF1 *fn2 = new TF1("fn2","[0]+[1]/sqrt(x)");
  // fn2->SetParNames("a02","b02");
  // graph2->Fit("fn2","","",20,200);  
  // double a02 = fn2->GetParameter(0);
  // double b02 = fn2->GetParameter(1);
  // double c02 = 0.0;

  TF1 *fn2 = new TF1("fn2","[0]+[1]*sqrt(x)+[2]*x");
  fn2->SetParNames("a02","b02","c02");
  graph2->Fit("fn2","","",15,300);  
  double a02 = fn2->GetParameter(0);
  double b02 = fn2->GetParameter(1);
  double c02 = fn2->GetParameter(2);

  ///////////////////////////////////////////////////
  //Fit3

  int xNofBin3 = 500;

  TCanvas *c4 = new TCanvas("c4","c4");
  c4->Divide(5,4);
  TF1 *fit3 = new TF1("fit3","gaus");
  int NofProject3 =20;
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
            
    xval3[i] = tmp2->GetBinCenter(x_center1);
    yval3[i] = fit3->GetParameter(1);
  }
  
  c1->cd(3);
  TGraph *graph3 = new TGraph(NofProject3, &(xval3[0]), &(yval3[0]));
  graph3->SetMarkerStyle(8);
  graph3->SetMarkerColor(1);
  graph3->Draw("psame");

  // TF1 *fn3 = new TF1("fn3","[0]+[1]/sqrt(x)");
  // fn3->SetParNames("a03","b03");
  // graph3->Fit("fn3","","",30,450);  
  // double a03 = fn3->GetParameter(0);
  // double b03 = fn3->GetParameter(1);
  // double c03 = 0.0;

  TF1 *fn3 = new TF1("fn3","[0]+[1]*sqrt(x)+[2]*x");
  fn3->SetParNames("a03","b03","c03");
  graph3->Fit("fn3","","",15,480);  
  double a03 = fn3->GetParameter(0);
  double b03 = fn3->GetParameter(1);
  double c03 = fn3->GetParameter(2);

  ///////////////////////////////////////////////////
  //Fit4

  int xNofBin4 = 500;

  TCanvas *c5 = new TCanvas("c5","c5");
  c5->Divide(5,4);
  TF1 *fit4 = new TF1("fit4","gaus");
  int NofProject4 =20;
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
            
    xval4[i] = tmp2->GetBinCenter(x_center1);
    yval4[i] = fit4->GetParameter(1);
  }
  
  c1->cd(4);
  TGraph *graph4 = new TGraph(NofProject4, &(xval4[0]), &(yval4[0]));
  graph4->SetMarkerStyle(8);
  graph4->SetMarkerColor(1);
  graph4->Draw("psame");

  // TF1 *fn4 = new TF1("fn4","[0]+[1]/sqrt(x)");
  // fn4->SetParNames("a04","b04");
  // graph4->Fit("fn4","","", 30,450);  
  // double a04 = fn4->GetParameter(0);
  // double b04 = fn4->GetParameter(1);
  // double c04 = 0.0;

  TF1 *fn4 = new TF1("fn4","[0]+[1]*sqrt(x)+[2]*x");
  fn4->SetParNames("a04","b04","c04");
  graph4->Fit("fn4","","", 15,480);  
  double a04 = fn4->GetParameter(0);
  double b04 = fn4->GetParameter(1);
  double c04 = fn4->GetParameter(2);

  TString fout_name = ("corr1.dat");
  ofstream fout(fout_name.Data());
  fout << a01 << "   " << b01 << "   " << c01 << "   " 
       << a02 << "   " << b02 << "   " << c02 << "   " 
       << a03 << "   " << b03 << "   " << c03 << "   " 
       << a04 << "   " << b04 << "   " << c04 << "   " 
       << endl;

  c1->Print("timewalkcorr1.pdf");
}

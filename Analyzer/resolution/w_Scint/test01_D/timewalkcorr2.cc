#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"

void timewalkcorr2()
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
  hist[0] = new TH2F("h1", "TOF1vsAMP1", 500, 0, 500, 100, -3, 3);
  hist[1] = new TH2F("h2", "TOF1vsAMP2", 500, 0, 500, 100, -3, 3);
  hist[2] = new TH2F("h3", "TOF1vsAMP3", 900, 0, 900, 100, -3, 3);
  hist[3] = new TH2F("h4", "TOF1vsAMP4", 900, 0, 900, 100, -3, 3);

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
      st1 >> a02 >> b02 >> c02 
	  >> a03 >> b03 >> c03 
	  >> a04 >> b04 >> c04; 
    }
  }
  
  cout<< a02 << "  " << b02 << "  " << c02 << endl;
  cout<< a03 << "  " << b03 << "  " << c03 << endl;
  cout<< a04 << "  " << b04 << "  " << c04 << endl;

  int n = tree->GetEntries();
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);
    if( tdc2[0]>0.&&tdc3[0]>0.&&tdc4[0]>0.&&
	amp2[0]>0.&&amp3[0]>0.&&amp4[0]>0.){

      double T2 =  a02+b02*pow(amp2[0]-60.0,0.5)+c02*(amp2[0]-60.0);
      double T3 =  a03+b03*pow(amp3[0]-50.0,0.5)+c03*(amp3[0]-50.0);
      double T4 =  a04+b04*pow(amp4[0]-60.0,0.5)+c04*(amp4[0]-60.0);
      
      double TOF1 = (tdc2[0]-(tdc3[0]+tdc4[0])/2.-5.73)-T2-(T3+T4)/2.;

      double Amp2 = amp2[0]-60.0;
      double Amp3 = amp3[0]-50.0;
      double Amp4 = amp4[0]-60.0;
      
      if( Amp2>50 && Amp3>50 && Amp4>50){
	hist[1]->Fill((amp2[0]-60.0),TOF1);
	hist[1]->GetXaxis()->SetTitle("AMP2");
	hist[1]->GetYaxis()->SetTitle("TOF1");
	hist[2]->Fill((amp3[0]-50.0),TOF1);
	hist[2]->GetXaxis()->SetTitle("AMP3");
	hist[2]->GetYaxis()->SetTitle("TOF1");
	hist[3]->Fill((amp4[0]-60.0),TOF1);
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

  // int xNofBin1 = 500;

  // TCanvas *c2 = new TCanvas("c2","c2");
  // c2->Divide(5,4);
  // TF1 *fit1 = new TF1("fit1","gaus");
  // int NofProject1 = 20;
  // int stepProject1 = xNofBin1/NofProject1;

  // std::vector<double> xval1;
  // std::vector<double> yval1;
  // xval1.resize(NofProject1);
  // yval1.resize(NofProject1);

  // for(int i = 0; i<NofProject1; ++i){
  //   c2->cd(1+i);
  //   int bin_min1 = i*stepProject1+1;
  //   int bin_max1 = (i+1)*stepProject1;
    
  //   //cout<< bin_min1 << ":" << bin_max1 << endl;
    
  //   TH1D *tmp1 = (TH1D*) hist[0]->ProjectionY("", bin_min1, bin_max1)->Clone();
  //   double center1 = tmp1->GetBinCenter(tmp1->GetMaximumBin());
  //   tmp1->Fit("fit1", "Q", "", center1-2.0, center1+2.0);
    
  //   //double x_min1 = tmp1->GetBinCenter(bin_min1*5.0);
  //   //double x_max1 = tmp1->GetBinCenter(bin_max1*5.0);
  //   //double x_center1 = (x_max1 + x_min1)/2.0;
    
  //   //cout<< "-->" << x_min1 << ":" << x_max1 << endl;
  //   //cout<< "-->" << x_center1 << endl;
  //   //xval1[i] = x_center1;
    
  //   xval1[i] = (bin_max1 + bin_min1)/2.0; //x_center1;
  //   yval1[i] = fit1->GetParameter(1);
    
  //   //cout<< "-->" << xval1[i] << ":" << yval1[i] << endl;
  // }
  
  // c1->cd(1);
  // TGraph *graph1 = new TGraph(NofProject1, &(xval1[0]), &(yval1[0]));
  // graph1->SetMarkerStyle(8);
  // graph1->SetMarkerColor(1);
  // graph1->Draw("psame");
  
  // TF1 *fn1 = new TF1("fn1","[0]+[1]/sqrt(x)+[2]*x");
  // fn1->SetParNames("a11","b11","c11");
  // graph1->Fit("fn1","","", 50,480);
  // double a11 = fn1->GetParameter(0);
  // double b11 = fn1->GetParameter(1);
  // double c11 = fn1->GetParameter(2);

  ///////////////////////////////////////////////////
  //Fit2

  int xNofBin2 = 500;
  
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
    int bin_min2 = i*stepProject2+1;
    int bin_max2 = (i+1)*stepProject2;
    
    TH1D *tmp2 = (TH1D*) hist[1]->ProjectionY("", bin_min2, bin_max2)->Clone();
    double center2 = tmp2->GetBinCenter(tmp2->GetMaximumBin());
    tmp2->Fit("fit2", "Q", "", center2-2.0, center2+2.0);
    
    //double x_min2 = tmp2->GetBinCenter(bin_min2*5.0);
    //double x_max2 = tmp2->GetBinCenter(bin_max2*5.0);
    //double x_center2 = (x_max2 + x_min2)/2.0;
    //xval2[i] = x_center2;
    
    xval2[i] = (bin_max2 + bin_min2)/2.0; 
    yval2[i] = fit2->GetParameter(1);
  }
  
  c1->cd(2);
  TGraph *graph2 = new TGraph(NofProject2, &(xval2[0]), &(yval2[0]));
  graph2->SetMarkerStyle(8);
  graph2->SetMarkerColor(1);
  graph2->Draw("psame");

  TF1 *fn2 = new TF1("fn2","[0]+[1]/sqrt(x)+[2]*x");
  fn2->SetParNames("a12","b12","c12");
  graph2->Fit("fn2","","", 50,480);  
  double a12 = fn2->GetParameter(0);
  double b12 = fn2->GetParameter(1);
  double c12 = fn2->GetParameter(2);

  ///////////////////////////////////////////////////
  //Fit3

  int xNofBin3 = 900;

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
    int bin_min3 = i*stepProject3+1;
    int bin_max3 = (i+1)*stepProject3;
    
    TH1D *tmp3 = (TH1D*) hist[2]->ProjectionY("", bin_min3, bin_max3)->Clone();
    double center3 = tmp3->GetBinCenter(tmp3->GetMaximumBin());
    tmp3->Fit("fit3", "Q", "", center3-2.0, center3+2.0);
    
    //double x_min3 = tmp3->GetBinCenter(bin_min3*5.0);
    //double x_max3 = tmp3->GetBinCenter(bin_max3*5.0);
    //double x_center3 = (x_max3 + x_min3)/2.0;
    //xval3[i] = x_center3;
    
    xval3[i] = (bin_max3 + bin_min3)/2.0; 
    yval3[i] = fit3->GetParameter(1);
  }
  
  c1->cd(3);
  TGraph *graph3 = new TGraph(NofProject3, &(xval3[0]), &(yval3[0]));
  graph3->SetMarkerStyle(8);
  graph3->SetMarkerColor(1);
  graph3->Draw("psame");

  TF1 *fn3 = new TF1("fn3","[0]+[1]/sqrt(x)+[2]*x");
  fn3->SetParNames("a13","b13","c13");
  graph3->Fit("fn3","","", 50,900);  
  double a13 = fn3->GetParameter(0);
  double b13 = fn3->GetParameter(1);
  double c13 = fn3->GetParameter(2);

  ///////////////////////////////////////////////////
  //Fit4

  int xNofBin4 = 900;

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
    int bin_min4 = i*stepProject4+1;
    int bin_max4 = (i+1)*stepProject4;

    TH1D *tmp4 = (TH1D*) hist[3]->ProjectionY("", bin_min4, bin_max4)->Clone();
    double center4 = tmp4->GetBinCenter(tmp4->GetMaximumBin());
    tmp4->Fit("fit4", "Q", "", center4-2.0, center4+2.0);
    
    //double x_min4 = tmp4->GetBinCenter(bin_min4*5.0);
    //double x_max4 = tmp4->GetBinCenter(bin_max4*5.0);
    //double x_center4 = (x_max4 + x_min4)/2.0;
    //xval4[i] = x_center4;

    xval4[i] = (bin_max4 + bin_min4)/2.0; 
    yval4[i] = fit4->GetParameter(1);
  }
  
  c1->cd(4);
  TGraph *graph4 = new TGraph(NofProject4, &(xval4[0]), &(yval4[0]));
  graph4->SetMarkerStyle(8);
  graph4->SetMarkerColor(1);
  graph4->Draw("psame");

  TF1 *fn4 = new TF1("fn4","[0]+[1]/sqrt(x)+[2]*x");
  fn4->SetParNames("a14","b14","c14");
  graph4->Fit("fn4","","", 50,900);  
  double a14 = fn4->GetParameter(0);
  double b14 = fn4->GetParameter(1);
  double c14 = fn4->GetParameter(2);

  TString fout_name = ("corr2.dat");
  ofstream fout(fout_name.Data());
  fout << a12 << "  " << b12 << "  " << c12 << "  " 
       << a13 << "  " << b13 << "  " << c13 << "  " 
       << a14 << "  " << b14 << "  " << c14 << "  " 
       << endl;

  c1->Print("timewalkcorr2.pdf");
}

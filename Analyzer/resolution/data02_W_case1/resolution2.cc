#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <fstream>   

void resolution2()
{
  TFile *fin = new TFile("../../test_run031.root");
  TTree *tree = (TTree*)fin->Get("tree");

  double amp1[16],amp2[16];
  double ltdc1[16],ltdc2[16];
  double ttdc1[16],ttdc2[16];

  tree->SetBranchAddress("amp1", amp1);
  tree->SetBranchAddress("amp2", amp2);
  tree->SetBranchAddress("ltdc1", ltdc1);
  tree->SetBranchAddress("ltdc2", ltdc2);
  tree->SetBranchAddress("ttdc1", ttdc1);
  tree->SetBranchAddress("ttdc2", ttdc2);

  const int Nh = 8;
  TH1F *hist[Nh];
  hist[0] = new TH1F("h1", "TOF1", 200, -1, 1);
  hist[1] = new TH1F("h2", "TOF2", 200, -1, 1);
  hist[2] = new TH1F("h3", "TOF3", 200, -1, 1);
  hist[3] = new TH1F("h4", "TOF4", 200, -1, 1);
  hist[4] = new TH1F("h5", "TOF5", 100, -1, 1);
  hist[5] = new TH1F("h6", "TOF6", 100, -1, 1);
  hist[6] = new TH1F("h7", "TOF7", 100, -1, 1);
  hist[7] = new TH1F("h8", "TOF8",  30, -1, 1);

  TH1F *hist21[Nh], *hist22[Nh];
  hist21[0] = new TH1F("h211", "WIDTH211", 300, 0, 30);
  hist21[1] = new TH1F("h212", "WIDTH212", 300, 0, 30);
  hist21[2] = new TH1F("h213", "WIDTH213", 300, 0, 30);
  hist21[3] = new TH1F("h214", "WIDTH214", 300, 0, 30);
  hist21[4] = new TH1F("h215", "WIDTH215", 300, 0, 30);
  hist21[5] = new TH1F("h216", "WIDTH216", 300, 0, 30);
  hist21[6] = new TH1F("h217", "WIDTH217", 300, 0, 30);
  hist21[7] = new TH1F("h218", "WIDTH218", 300, 0, 30);

  hist22[0] = new TH1F("h221", "WIDTH221", 300, 0, 30);
  hist22[1] = new TH1F("h222", "WIDTH222", 300, 0, 30);
  hist22[2] = new TH1F("h223", "WIDTH223", 300, 0, 30);
  hist22[3] = new TH1F("h224", "WIDTH224", 300, 0, 30);
  hist22[4] = new TH1F("h225", "WIDTH225", 300, 0, 30);
  hist22[5] = new TH1F("h226", "WIDTH226", 300, 0, 30);
  hist22[6] = new TH1F("h227", "WIDTH227", 300, 0, 30);
  hist22[7] = new TH1F("h228", "WIDTH228", 300, 0, 30);

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
    if(180.0<ltdc1[0]&&ltdc1[0]<190.0&&180.0<ltdc2[0]&&ltdc2[0]<190.0&&
       176.0<ttdc1[0]&&ttdc1[0]<186.0&&176.0<ttdc2[0]&&ttdc2[0]<186.0){

      double pe1 = 0.0892*amp1[0]+0.95;
      double pe2 = 0.0896*amp2[0]+1.25;

      double w1 = ltdc1[0]-ttdc1[0]-5.0;
      double w2 = ltdc2[0]-ttdc2[0]-4.7;
      
      double T1 =  a01+b01*w1+c01*w1*w1;
      double T2 =  a02+b02*w2+c02*w2*w2;

      double TOFa = ((ltdc1[0]-ltdc2[0])+0.58)-T1;
      double TOF1 = TOFa-T2;

      double T11 =  a11+b11*w1+c11*w1*w1;
      double T12 =  a12+b12*w2+c12*w2*w2;
      
      double TOFa2 = TOF1-T11;
      double TOF2  = TOFa2-T12;

      double T21 =  a21+b21*w1+c21*w1*w1;
      double T22 =  a22+b22*w2+c22*w2*w2;
      
      double TOFa3 = TOF2-T21;
      double TOF   = TOFa3-T22;

      double rw1 = w1+5.0;
      double rw2 = w2+4.7;

      if( pe1>10.0 && pe2>10.0 ){
	if( rw1>2.5 && rw2>2.5 ){
	  hist[0]->Fill(TOF);	
	  hist21[0]->Fill(rw1);
	  hist22[0]->Fill(rw2);
	}
	if( rw1>3.0 && rw2>3.0 ){
	  hist[1]->Fill(TOF);
	  hist21[1]->Fill(rw1);
	  hist22[1]->Fill(rw2);
	}
	if( rw1>3.5 && rw2>3.5 ){
	  hist[2]->Fill(TOF);
	  hist21[2]->Fill(rw1);
	  hist22[2]->Fill(rw2);
	}
	if( rw1>4.0 && rw2>4.0 ){
	  hist[3]->Fill(TOF);
	  hist21[3]->Fill(rw1);
	  hist22[3]->Fill(rw2);
	}
	if( rw1>4.5 && rw2>4.5 ){
	  hist[4]->Fill(TOF);
	  hist21[4]->Fill(rw1);
	  hist22[4]->Fill(rw2);
	}
	if( rw1>5.0 && rw2>5.0 ){
	  hist[5]->Fill(TOF);
	  hist21[5]->Fill(rw1);
	  hist22[5]->Fill(rw2);
	}
	if( rw1>5.5 && rw2>5.5 ){
	  hist[6]->Fill(TOF);
	  hist21[6]->Fill(rw1);
	  hist22[6]->Fill(rw2);
	}
	if( rw1>6.0 && rw2>6.0 ){
	  hist[7]->Fill(TOF);
	  hist21[7]->Fill(rw1);
	  hist22[7]->Fill(rw2);
	}
      }
    }
  }

  double resol[Nh], resolE[Nh];
  double iresol[Nh], iresolE[Nh];
  double cut[Nh] = { 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0};
  double cutE[Nh] = {0., 0., 0., 0., 0., 0., 0., 0.};
  double mean[Nh], mean1[Nh], mean2[Nh];
  double meanE[Nh] = {0., 0., 0., 0., 0., 0., 0., 0.};

  TF1 *fit1 = new TF1("fit1","gaus");

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(4,2);
  for(int i=0; i<Nh; ++i){
    c1->cd(i+1);
    hist[i]->Draw();
    
    hist[i]->Fit("fit1", "", "", -0.30, 0.30);

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
  TH1F* Hc1=c2->cd(1)->DrawFrame( 1,  0.0, 7.0, 200.0 );
  Hc1->GetXaxis()->SetLabelSize(0.040);
  Hc1->GetYaxis()->SetLabelSize(0.040);
  Hc1->GetYaxis()->SetTitle("Resolution [ps]");
  Hc1->GetYaxis()->SetTitleSize(0.05);
  Hc1->GetYaxis()->SetTitleOffset(1.0);
  Hc1->GetXaxis()->SetTitle("Width Cut Position [ns]");
  Hc1->GetXaxis()->SetTitleSize(0.05);
  
  TGraphErrors *graph1 = new TGraphErrors(Nh, cut, resol, cutE, resolE);
  graph1->SetMarkerStyle(8);
  graph1->SetMarkerColor(1);
  graph1->Draw("PEsame");

  TGraphErrors *graph2 = new TGraphErrors(Nh, cut, iresol, cutE, iresolE);
  graph2->SetMarkerStyle(8);
  graph2->SetMarkerColor(2);
  graph2->Draw("PEsame");

  TCanvas *c3 = new TCanvas("c3","c3"); 
  c3->cd(1)->SetLeftMargin(0.15);
  c3->cd(1)->SetRightMargin(0.1);
  c3->cd(1)->SetTopMargin(0.1);
  c3->cd(1)->SetBottomMargin(0.15);
  c3->cd(1)->SetGrid();
  TH1F* Hc2=c3->cd(1)->DrawFrame( 4,  0.0, 6.5, 200.0 );
  Hc2->GetXaxis()->SetLabelSize(0.040);
  Hc2->GetYaxis()->SetLabelSize(0.040);
  Hc2->GetYaxis()->SetTitle("Resolution [ps]");
  Hc2->GetYaxis()->SetTitleSize(0.05);
  Hc2->GetYaxis()->SetTitleOffset(1.0);
  Hc2->GetXaxis()->SetTitle("Width Mean [ns]");
  Hc2->GetXaxis()->SetTitleSize(0.05);
  
  TGraphErrors *graph3 = new TGraphErrors(Nh, mean, resol, meanE, resolE);
  graph3->SetMarkerStyle(8);
  graph3->SetMarkerColor(1);
  graph3->Draw("PEsame");

  TGraphErrors *graph4 = new TGraphErrors(Nh, mean, iresol, meanE, iresolE);
  graph4->SetMarkerStyle(8);
  graph4->SetMarkerColor(2);
  graph4->Draw("PEsame");

  c1 -> Print("resolution2.pdf");
  c2 -> Print("resolution2g.pdf");
  c3 -> Print("resolution2g2.pdf");
}

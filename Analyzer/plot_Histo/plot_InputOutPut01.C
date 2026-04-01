void plot_InputOutPut01()
{ 
  TCanvas* c1 = new TCanvas("c1","",1200,800);
  gROOT->SetStyle("Plain");
  gROOT->SetStyle("Plain");
  c1->Divide(2,2);

  TH2F *h1001=new TH2F("h1001","", 160,0,1600, 100,0,1000);
  TH2F *h1002=new TH2F("h1002","", 160,0,1600, 100,0,1000);
  TH2F *h1011=new TH2F("h1011","", 160,0,1600, 100,0,1000);
  TH2F *h1012=new TH2F("h1012","", 160,0,1600, 100,0,1000);

  TChain chain1("tree");
  chain1.Add( "test_run013.root" );
  chain1.Project("h1001","amp[2]:amp[0]","dt[2]>0&&dt[0]>0");
  chain1.Project("h1011","amp[2]:amp[0]","dt[2]>0&&dt[0]>0");
  chain1.Project("h1002","amp[3]:amp[1]","dt[3]>0&&dt[1]>0");
  chain1.Project("h1012","amp[3]:amp[1]","dt[3]>0&&dt[1]>0");

  c1->cd(1);
  c1->cd(1)->SetLeftMargin(0.15);
  c1->cd(1)->SetRightMargin(0.1);
  c1->cd(1)->SetTopMargin(0.1);
  c1->cd(1)->SetBottomMargin(0.15);
  c1->cd(1)->SetGrid();
  c1->cd(1)->SetLogy(0);
  TH1F* Hc1=c1->cd(1)->DrawFrame( 0.0, 0.0, 1600.0, 1000.0 );
  Hc1->GetXaxis()->SetLabelSize(0.050);
  Hc1->GetYaxis()->SetLabelSize(0.050);
  Hc1->GetYaxis()->SetTitle("Output Pulse Height [mV]");
  Hc1->GetYaxis()->SetTitleSize(0.055);
  Hc1->GetYaxis()->SetTitleOffset(1.3);
  Hc1->GetXaxis()->SetTitle("Input Pulse Height [mV]");
  Hc1->GetXaxis()->SetTitleSize(0.055);
  h1001->Draw("same,col");

  c1->cd(2);
  c1->cd(2)->SetLeftMargin(0.15);
  c1->cd(2)->SetRightMargin(0.1);
  c1->cd(2)->SetTopMargin(0.1);
  c1->cd(2)->SetBottomMargin(0.15);
  c1->cd(2)->SetGrid();
  c1->cd(2)->SetLogy(0);
  TH1F* Hc2=c1->cd(2)->DrawFrame( 0.0, 0.0, 1600.0, 1000.0 );
  Hc2->GetXaxis()->SetLabelSize(0.050);
  Hc2->GetYaxis()->SetLabelSize(0.050);
  Hc2->GetYaxis()->SetTitle("Output Pulse Height [mV]");
  Hc2->GetYaxis()->SetTitleSize(0.055);
  Hc2->GetYaxis()->SetTitleOffset(1.3);
  Hc2->GetXaxis()->SetTitle("Input Pulse Height [mV]");
  Hc2->GetXaxis()->SetTitleSize(0.055);
  h1002->Draw("same,col");

  c1->cd(3);
  c1->cd(3)->SetLeftMargin(0.15);
  c1->cd(3)->SetRightMargin(0.1);
  c1->cd(3)->SetTopMargin(0.1);
  c1->cd(3)->SetBottomMargin(0.15);
  c1->cd(3)->SetGrid();
  c1->cd(3)->SetLogy(0);
  TH1F* Hc3=c1->cd(3)->DrawFrame( 0.0, 0.0, 800.0, 400.0 );
  Hc3->GetXaxis()->SetLabelSize(0.050);
  Hc3->GetYaxis()->SetLabelSize(0.050);
  Hc3->GetYaxis()->SetTitle("Output Pulse Height [mV]");
  Hc3->GetYaxis()->SetTitleSize(0.055);
  Hc3->GetYaxis()->SetTitleOffset(1.3);
  Hc3->GetXaxis()->SetTitle("Input Pulse Height [mV]");
  Hc3->GetXaxis()->SetTitleSize(0.055);
  h1011->Draw("same,col");

  c1->cd(4);
  c1->cd(4)->SetLeftMargin(0.15);
  c1->cd(4)->SetRightMargin(0.1);
  c1->cd(4)->SetTopMargin(0.1);
  c1->cd(4)->SetBottomMargin(0.15);
  c1->cd(4)->SetGrid();
  c1->cd(4)->SetLogy(0);
  TH1F* Hc4=c1->cd(4)->DrawFrame( 0.0, 0.0, 800.0, 400.0 );
  Hc4->GetXaxis()->SetLabelSize(0.050);
  Hc4->GetYaxis()->SetLabelSize(0.050);
  Hc4->GetYaxis()->SetTitle("Output Pulse Height [mV]");
  Hc4->GetYaxis()->SetTitleSize(0.055);
  Hc4->GetYaxis()->SetTitleOffset(1.3);
  Hc4->GetXaxis()->SetTitle("Input Pulse Height [mV]");
  Hc4->GetXaxis()->SetTitleSize(0.055);
  h1012->Draw("same,col");
}

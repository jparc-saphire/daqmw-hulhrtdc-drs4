void plot_PHWidth01()
{ 
  TCanvas* c1 = new TCanvas("c1","",1200,800);
  gROOT->SetStyle("Plain");
  gROOT->SetStyle("Plain");
  c1->Divide(2,2);

  TH2F *h1001=new TH2F("h1001","", 150,1,600, 60,1,60);
  TH2F *h1002=new TH2F("h1002","", 150,1,600, 60,1,60);
  TH2F *h1011=new TH2F("h1011","", 150,1,600, 60,1,60);
  TH2F *h1012=new TH2F("h1012","", 150,1,600, 60,1,60);

  TChain chain1("tree");
  chain1.Add( "test_run008.root" );
  chain1.Project("h1001","hrdt[0]-hrdt_2nd[0]:amp[0]","hrdt[0]>0&&hrdt_2nd[0]>0");
  chain1.Project("h1011","hrdt[0]-hrdt_2nd[0]:amp[0]","hrdt[0]>0&&hrdt_2nd[0]>0");
  chain1.Project("h1002","hrdt[1]-hrdt_2nd[1]:amp[1]","hrdt[1]>0&&hrdt_2nd[1]>0");
  chain1.Project("h1012","hrdt[1]-hrdt_2nd[1]:amp[1]","hrdt[1]>0&&hrdt_2nd[1]>0");

  c1->cd(1);
  c1->cd(1)->SetLeftMargin(0.15);
  c1->cd(1)->SetRightMargin(0.1);
  c1->cd(1)->SetTopMargin(0.1);
  c1->cd(1)->SetBottomMargin(0.15);
  c1->cd(1)->SetGrid();
  c1->cd(1)->SetLogy(0);
  c1->cd(1)->SetLogz();
  TH1F* Hc1=c1->cd(1)->DrawFrame( 0.0, 0.0, 500.0, 60.0 );
  Hc1->GetXaxis()->SetLabelSize(0.050);
  Hc1->GetYaxis()->SetLabelSize(0.050);
  Hc1->GetYaxis()->SetTitle("Pulse Width [ns]");
  Hc1->GetYaxis()->SetTitleSize(0.055);
  Hc1->GetYaxis()->SetTitleOffset(1.3);
  Hc1->GetXaxis()->SetTitle("Pulse Height [mV]");
  Hc1->GetXaxis()->SetTitleSize(0.055);
  h1001->Draw("same,col");

  c1->cd(2);
  c1->cd(2)->SetLeftMargin(0.15);
  c1->cd(2)->SetRightMargin(0.1);
  c1->cd(2)->SetTopMargin(0.1);
  c1->cd(2)->SetBottomMargin(0.15);
  c1->cd(2)->SetGrid();
  c1->cd(2)->SetLogy(0);
  c1->cd(2)->SetLogz();
  TH1F* Hc2=c1->cd(2)->DrawFrame( 0.0, 0.0, 500.0, 60.0 );
  Hc2->GetXaxis()->SetLabelSize(0.050);
  Hc2->GetYaxis()->SetLabelSize(0.050);
  Hc2->GetYaxis()->SetTitle("Pulse Width [ns]");
  Hc2->GetYaxis()->SetTitleSize(0.055);
  Hc2->GetYaxis()->SetTitleOffset(1.3);
  Hc2->GetXaxis()->SetTitle("Pulse Height [mV]");
  Hc2->GetXaxis()->SetTitleSize(0.055);
  h1002->Draw("same,col");

  c1->cd(3);
  c1->cd(3)->SetLeftMargin(0.15);
  c1->cd(3)->SetRightMargin(0.1);
  c1->cd(3)->SetTopMargin(0.1);
  c1->cd(3)->SetBottomMargin(0.15);
  c1->cd(3)->SetGrid();
  c1->cd(3)->SetLogy(0);
  c1->cd(3)->SetLogz();
  TH1F* Hc3=c1->cd(3)->DrawFrame( 0.0, 0.0, 300.0, 40.0 );
  Hc3->GetXaxis()->SetLabelSize(0.050);
  Hc3->GetYaxis()->SetLabelSize(0.050);
  Hc3->GetYaxis()->SetTitle("Pulse Width [ns]");
  Hc3->GetYaxis()->SetTitleSize(0.055);
  Hc3->GetYaxis()->SetTitleOffset(1.3);
  Hc3->GetXaxis()->SetTitle("Pulse Height [mV]");
  Hc3->GetXaxis()->SetTitleSize(0.055);
  h1011->Draw("same,col");

  c1->cd(4);
  c1->cd(4)->SetLeftMargin(0.15);
  c1->cd(4)->SetRightMargin(0.1);
  c1->cd(4)->SetTopMargin(0.1);
  c1->cd(4)->SetBottomMargin(0.15);
  c1->cd(4)->SetGrid();
  c1->cd(4)->SetLogy(0);
  c1->cd(4)->SetLogz();
  TH1F* Hc4=c1->cd(4)->DrawFrame( 0.0, 0.0, 300.0, 40.0 );
  Hc4->GetXaxis()->SetLabelSize(0.050);
  Hc4->GetYaxis()->SetLabelSize(0.050);
  Hc4->GetYaxis()->SetTitle("Pulse Width [ns]");
  Hc4->GetYaxis()->SetTitleSize(0.055);
  Hc4->GetYaxis()->SetTitleOffset(1.3);
  Hc4->GetXaxis()->SetTitle("Pulse Height [mV]");
  Hc4->GetXaxis()->SetTitleSize(0.055);
  h1012->Draw("same,col");
}

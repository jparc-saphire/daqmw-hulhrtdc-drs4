void comp_SV1() {
  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
  TFile *f1 = new TFile("./ana/check_actsc/check_actsc_run5113.root");
  TH2D * h1 = (TH2D*)f1->Get("hlt_wt_SV1");
  TH1D *hwt_SV1_run3113 = h1->ProjectionX();
  TFile *f2 = new TFile("./ana/check_actsc/check_actsc_run5114.root");
  TH2D * h2 = (TH2D*)f2->Get("hlt_wt_SV1");
  TH1D *hwt_SV1_run3114 = h2->ProjectionX();


  hwt_SV1_run3113->SetLineColor(2);
  hwt_SV1_run3113->Draw();
  hwt_SV1_run3114->SetLineColor(4);
  hwt_SV1_run3114->Draw("Same");
  c1->SaveAs("comp_SV1_run3113_3114.png");
}

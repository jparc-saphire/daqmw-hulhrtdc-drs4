void hadcwt(Char_t tag[]) {
  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);

  hadc_wt_AC->SetAxisRange(0,60,"X");
  hadc_wt_AC->SetAxisRange(0,200,"Y");

  hadc_wt_AC->Draw("colz");

  c1->SaveAs(Form("hadc_wt_AC_%s.png",tag));
  c1->Update();

  hamp_wt_AC->Draw("colz");

  c1->SaveAs(Form("hamp_wt_AC_%s.png",tag));
  c1->Update();
	    
  
}

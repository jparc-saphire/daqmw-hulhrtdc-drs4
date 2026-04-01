void eff(Char_t tag[]) {
  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
  Double_t ne = hlt_SV1->GetEntries();
  Double_t nt = hlt_SV1_g->GetEntries();
  Double_t na_sh1 = hlt_SH1_g->GetEntries();
  Double_t na_sh2 = hlt_SH2_g->GetEntries();
  Double_t nd_ac  = hlt_AC_g->GetEntries();

  Double_t na = hlt_SH1_a->GetEntries();
  Double_t nad_ac  = hlt_AC_a->GetEntries();

  Double_t eff_trig = nt/ne;
  Double_t eeff_trig = ((nt>ne-nt) ? sqrt(nt) : sqrt(ne-nt)) /ne;
  Double_t acc = na/nt;
  Double_t eacc = ((na>nt-na) ? sqrt(na) : sqrt(nt-na)) /nt;
  Double_t acc_sh1 = na_sh1/nt;
  Double_t eacc_sh1 = ((na_sh1>nt-na_sh1) ? sqrt(na_sh1) : sqrt(nt-na_sh1)) /nt;
  Double_t acc_sh2 = na_sh2/nt;
  Double_t eacc_sh2 = ((na_sh2>nt-na_sh2) ? sqrt(na_sh2) : sqrt(nt-na_sh2)) /nt;
  
  Double_t eff_ac = nd_ac/nt;
  Double_t eeff_ac = ((nd_ac>nt-nd_ac) ? sqrt(nd_ac) : sqrt(nt-nd_ac)) /nt;
  Double_t aeff_ac = nad_ac/na;
  Double_t eaeff_ac = ((nad_ac>na-nad_ac) ? sqrt(nad_ac) : sqrt(na-nad_ac)) /na;

  cout << "eff_trig = " << eff_trig << "+-" << eeff_trig << endl;
  cout << "acc = " << acc << "+-" << eacc << endl;
  cout << "acc_sh1 = " << acc_sh1 << "+-" << eacc_sh1 << endl;
  cout << "acc_sh2 = " << acc_sh2 << "+-" << eacc_sh2 << endl;
  cout << "eff_ac = " << eff_ac << "+-" << eeff_ac << endl;
  cout << "aeff_ac = " << aeff_ac << "+-" << eaeff_ac << endl;

  ofstream ofs(Form("eff_run%s.out",tag));
  ofs << "eff_trig = " << eff_trig << "+-" << eeff_trig << endl;
  ofs << "acc = " << acc << "+-" << eacc << endl;
  ofs << "acc_sh1 = " << acc_sh1 << "+-" << eacc_sh1 << endl;
  ofs << "acc_sh2 = " << acc_sh2 << "+-" << eacc_sh2 << endl;
  ofs << "eff_ac = " << eff_ac << "+-" << eeff_ac << endl;
  ofs << "aeff_ac = " << aeff_ac << "+-" << eaeff_ac << endl;
  ofs.close();
}

double the_npefit(double *vars, double* pars);
  

static const int m = 2;
//static const double p0 = 0.;//pedestal
//static const double p1 = 2.9;//1 photon peak
static const double p0 = 0.;//pedestal
static const double p1 = 1.3;//1 photon peak

void npeana(Char_t tag[]) {
  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
  c1->SetLogy();
  //  TF1 *npefit = new TF1("npefit",the_npefit,0,100,4);
  TF1 *npefit = new TF1("npefit",the_npefit,0,100,5);
  Double_t cped = 10000;
  Double_t c0 = 10;
  Double_t l0 = 0.4;
  //  Double_t s1 = 2.5;
  //  Double_t s1 = 3.0;
  Double_t s1 = 1.0;

  npefit->SetParameter(0,cped);
  npefit->SetParameter(1,c0);
  npefit->SetParameter(2,l0);
  npefit->SetParameter(3,s1);
  npefit->SetParameter(4,p1);

  hadc_AC->SetAxisRange(0,10,"X");

  //  hadc_AC->Fit(npefit,"RME","",0,30);
  //  hadc_AC->Fit(npefit,"RME","",0,13);
  //  hadc_AC->Fit(npefit,"RLE","",0,10);
    hadc_AC->Fit(npefit,"RLE","",0,4);
  
}

double the_npefit(double *vars, double*pars) {
  double Cped = pars[0];
  double C = pars[1];
  double lambda = pars[2];//mean value of photoelectron
  double sigma1 = pars[3];//PMT resolution for 1 photon
  double pp1 = pars[4];//1 photon peak
  //  double pp1 = p1;//1 photon peak
  double x = vars[0];
  double_t value = 0;
  for (int i=0;i<=m;i++) {
    if (i==0) {
      double poisson = Cped;
      double gaussian = exp(-(x-p0)*(x-p0)/(2.*sigma1*sigma1))/sqrt(2.*TMath::Pi())/sigma1;
      
      value += (poisson*gaussian);
    } else {
      double i1 = 1.;
      for (int j=1;j<=i;j++) {
	i1 *= (double)j;
      }
      double poisson = C*pow(lambda,i)*exp(-lambda)/i1;
      double gaussian = exp(-(x-(p0+(double)i*pp1))*(x-(p0+(double)i*pp1))/(2.*(double)i*sigma1*sigma1))/sqrt(2.*TMath::Pi()*(double)i)/sigma1;

      value += (poisson*gaussian);
    }
    //    cout << "i, poisson, gaussian = " << i<< " " << poisson << " " << gaussian << endl;
  }
  //  cout << "value = " << value << endl;

  return value;
}

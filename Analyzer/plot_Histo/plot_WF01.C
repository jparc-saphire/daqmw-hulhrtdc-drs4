#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"

void plot_WF01( int num )
{
  TFile *fin = new TFile("test.root");
  TTree *tree = (TTree*)fin->Get("tree");

  std::vector<std::vector<double>> *WaveForm = 0;

  TBranch *BWaveForm = 0;
  
  tree->SetBranchAddress("WaveForm", &WaveForm, &BWaveForm );
  
  TH2F *hist[1];
  hist[0] = new TH2F("h1", "Waveform", 1024, 0, 1024, 1000, 0, 1600);

  const int N=1022;
  
  int x[N];
  int y[N];

  for(int j = 0; j<N; ++j){
    x[j] = 0;y[j] = 0;
  }

  int all_hits = 0;
  
  int n = tree->GetEntries();
  //for(int i = num; i<num+1; ++i){
  for(int i = 0; i<n; ++i){
    tree->GetEntry(i);

    int size_wf = WaveForm->at(0).size();

    int hits=0;
    bool hitflag=false;
      
    for(int j = 0; j<size_wf; ++j){
      int wf0 = WaveForm->at(0).at(j);
      hist[0]->Fill(j, wf0);
      x[j] = j;
      y[j] = wf0;
      
      if( 1000<wf0 ) hits++;
    }
    if( 0< hits && hits <num ){
      cout<< "Event Num = " << i << endl;
      all_hits++;
    }
  }

  cout<< "Good fraction = " << double(all_hits)/double(n) << endl;;
  
  TCanvas *c1 = new TCanvas("c1","c1");

  TGraph *graph1 = new TGraph(N, x, y);
  graph1->Draw("");
  
}

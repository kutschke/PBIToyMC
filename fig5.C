//
// Study properties of simulated PBI over one spill.
// Also study a toy MC of the ExtMon response to this spill
//
#include "TMath.h"
#include "TH1D.h"
#include "TNtuple.h"

#include <random>
#include <vector>

#include <unistd.h>

{
  gROOT->Reset();

  // Base name of the input file; will also be used for base name of the output files.
  std::string base_filename = "Normal";
  //std::string const base_filename = "Pathological";

  std::string const input_root_filename = base_filename + ".root";

  // Get rid of ugly grey background.
  gROOT->SetStyle("Plain");

  // No statistics box for histograms
  gStyle->SetOptStat("eou");

  // Title middle and top
  gStyle->SetTitleAlign(23);
  gStyle->SetTitleX(0.5);

  gStyle->SetHistLineColor(kBlue);
  gStyle->SetTitleSize(0.09, "XYZ");
  gStyle->SetTitleXOffset(0.4);
  gStyle->SetTitleYOffset(0.3);
  gStyle->SetTitleFontSize(0.09);
  gStyle->SetLabelSize(0.08,"XYZ");


  gROOT->ForceStyle();

  TFile* pbi = new TFile(input_root_filename.c_str(), "READ");

  TH1D* hReco_1;   pbi->GetObject("hReco_1",   hReco_1);
  TH1D* hReco_25;  pbi->GetObject("hReco_25",  hReco_25);
  TH1D* hReco_50;  pbi->GetObject("hReco_50",  hReco_50);
  TH1D* hReco_100; pbi->GetObject("hReco_100", hReco_100);

  std::string const title = "Spill Toy MC using " + input_root_filename;

  TCanvas *canvas = new TCanvas("c", title.c_str(), 1200, 900 );
  canvas->Divide(1,4);

  double t0=-0.000847486;  // ms
  double tend=43.1176;     // ms
  double mean=40.5*0.9;    // reco extmon tracks per pulse

  TLine* line = new TLine;
  line->SetLineStyle(2);
  line->SetLineColor(kRed);

  TText* text = new TText;
  cout << "Text size:  " << text->GetTextSize() << endl;
  text->SetTextColor(kRed);
  text->SetTextSize(0.08);

  canvas->cd(1);
  TH1* copy =hReco_1->DrawCopy("E");
  line->DrawLine( t0, mean, tend, mean);
  copy->SetTitle("Simulated Spill (Normal);Time in Spill         (ms); Reco ExtMon Tracks");
  copy->SetMinimum(0.);
  copy->SetMaximum(250.);


  canvas->cd(2);
  copy =hReco_25->DrawCopy("E");
  line->DrawLine( t0, mean, tend, mean);
  copy->SetTitle("Simulated Spill (Normal) Every 25th Pulse;Time in Spill         (ms); Reco ExtMon Tracks");
  copy->SetMinimum(0.);
  copy->SetMaximum(250.);

  canvas->cd(3);
  copy =hReco_50->DrawCopy("E");
  line->DrawLine( t0, mean, tend, mean);
  copy->SetTitle("Simulated Spill (Normal) Every 50th Pulse;Time in Spill         (ms); Reco ExtMon Tracks");
  copy->SetMinimum(0.);
  copy->SetMaximum(250.);

  canvas->cd(4);
  copy =hReco_100->DrawCopy("E");
  line->DrawLine( t0, mean, tend, mean);
  copy->SetTitle("Simulated Spill (Normal) Every 100th Pulse;Time in Spill            (ms); Reco ExtMon Tracks");
  copy->SetMinimum(0.);
  copy->SetMaximum(250.);

  canvas->Print("fig5.png");
  cerr << "Print complete." << endl;

  pbi->Close();

}

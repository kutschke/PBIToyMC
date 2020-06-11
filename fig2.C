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
  //std::string base_filename = "Normal";
  std::string const base_filename = "Pathological";

  std::string const input_root_filename = base_filename + ".root";

  // Get rid of ugly grey background.
  gROOT->SetStyle("Plain");

  // No statistics box for histograms
  gStyle->SetOptStat("eou");

  // Title middle and top
  gStyle->SetTitleAlign(23);
  gStyle->SetTitleX(0.5);

  gStyle->SetHistLineColor(kBlue);
  gStyle->SetTitleSize(0.06, "XYZ");
  gStyle->SetTitleXOffset(0.75);
  gStyle->SetTitleYOffset(0.5);
  gStyle->SetTitleFontSize(0.055);

  gROOT->ForceStyle();

  TFile* pbi = new TFile(input_root_filename.c_str(), "READ");

  TH1D* hSimSpill;  pbi->GetObject("hSimSpill",  hSimSpill);
  cout << "hSimSpill: " << hSimSpill << endl;
  if ( hSimSpill == 0 ) return;

  std::string const title = "Spill Toy MC using " + input_root_filename;

  TCanvas *canvas = new TCanvas("c", title.c_str(), 1200, 900 );
  canvas->Divide(1,2);

  double t0=-0.000847486;  // ms
  double tend=43.1176;     // ms
  double mean=38.6*1.E6;   // protons per pulse

  TLine* line = new TLine;
  line->SetLineStyle(2);
  line->SetLineColor(kRed);

  TText* text = new TText;
  text->SetTextColor(kBlue);

  canvas->cd(1);
  TH1* copy =hSimSpill->DrawCopy("HIST");
  line->DrawLine( t0, mean, tend, mean);
  gPad->SetLogy(1);
  copy->SetTitle("Simulated Spill Pathological Behaviour;Time in Spill            (ms); Protons per Pulse");
  text->DrawText( 20.,  3000000, "Mean protons per pulse: 38.6 Mp");
  text->DrawText( 20.,  1000000, "Spill Duty Factor: 0.49");
  text->DrawText( 20.,   300000, "From Mu2e-doc-33344");

  canvas->cd(2);
  copy=hSimSpill->DrawCopy("HIST");
  line->DrawLine( t0, mean, tend, mean);
  copy->SetTitle("Simulated Spill Pathological Behaviour;Time in Spill            (ms); Protons per Pulse");

  canvas->Print("fig2.png");
  cerr << "Print complete." << endl;

  pbi->Close();

}

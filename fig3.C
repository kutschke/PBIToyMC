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
  gStyle->SetTitleSize(0.08, "XYZ");
  gStyle->SetTitleXOffset(0.5);
  gStyle->SetTitleYOffset(0.25);
  gStyle->SetTitleFontSize(0.07);

  gROOT->ForceStyle();

  TFile* pbi = new TFile(input_root_filename.c_str(), "READ");

  TH1D* hSimFRMS;  pbi->GetObject("hSimFRMS",  hSimFRMS);
  TH1D* hSimSpill;  pbi->GetObject("hSimSpill",  hSimSpill);
  cout << "hSimFRMS: " << hSimFRMS << endl;
  if ( hSimFRMS == 0 || hSimSpill == 0) return;

  std::string const title = "Spill Toy MC using " + input_root_filename;

  TCanvas *canvas = new TCanvas("c", title.c_str(), 1200, 900 );
  canvas->Divide(1,3);

  double t0=-0.000847486;  // ms
  double tend=43.1176;     // ms
  double mean=40.5*1.E6;   // protons per pulse
  double four_percent=0.04; 

  TLine* line = new TLine;
  line->SetLineStyle(2);
  line->SetLineColor(kRed);

  TText* text = new TText;
  cout << "Text size:  " << text->GetTextSize() << endl;
  text->SetTextColor(kRed);
  text->SetTextSize(0.08);

  canvas->cd(1);
  TH1* copy =hSimSpill->DrawCopy("HIST");
  line->DrawLine( t0, mean, tend, mean);
  gPad->SetLogy(1);
  copy->SetTitle("Simulated Spill (Normal);Time in Spill            (ms); Protons per Pulse");

  canvas->cd(2);
  copy =hSimFRMS->DrawCopy("HIST");
  line->DrawLine( t0, four_percent, tend, four_percent);
  gPad->SetLogy(1);
  copy->SetTitle("Simulated Spill (Normal) Rebinned by 10;Time in Spill            (ms); RMS/Mean");
  text->DrawText( 18.,  0.05, "RMS/Mean=0.04 (for reference)");
  //text->DrawText( 15.,  1000000, "Spill Duty Factor: 0.60");
  //text->DrawText( 15.,   300000, "From Mu2e-doc-33344");

  canvas->cd(3);
  copy=hSimFRMS->DrawCopy("HIST");
  line->DrawLine( t0, four_percent, tend, four_percent);
  copy->SetTitle("Simulated Spill (Normal) Rebinned by 10;Time in Spill            (ms); RMS/Mean");
  copy->SetMinimum(0.);
  copy->SetMaximum(0.1);

  canvas->Print("fig3.png");
  cerr << "Print complete." << endl;

  pbi->Close();

}

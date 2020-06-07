//
// Study properties of simulated PBI over one spill.
// Also study a toy MC of the ExtMon response to this spill
//
#include "TMath.h"
#include "TH1D.h"
#include "TNtuple.h"

#include <random>

{
  // Configurable inputs
  // Number of reconstructed extmon tracks per POT, from Andrei.
  constexpr double efficiency = 0.9e-6;

  std::string input_filename = "Normal.txt";
  //std::string input_filename = "Pathological.txt";

  // End configurable inputs

  gROOT->Reset();

  // Get rid of ugly grey background.
  gROOT->SetStyle("Plain");

  // No statistics box for histograms
  gStyle->SetOptStat("");

  TFile* pbi = new TFile("pbi.root", "RECREATE");

  TCanvas *canvas = new TCanvas("c","Spill Model", 900, 900 );
  canvas->Divide(1,4);

  std::default_random_engine engine;

#include "SDF.cc+"
#include "Spill.cc+"

  // Read input file of a simulated spill.
  Spill spill{input_filename};
  cout << "Simulated spill filename: " << spill.filename << "  Number of entries: " << spill.t.size() <<endl;
  cout << "Simulated spill mean:     "
       << spill.stats.mean/1.E6 << " Mp," 
       << "   rms: " << spill.stats.rms/1.E6 << " Mp,"
       << "   SDF: " << spill.stats.sdf
       << endl;

#include "Measurement.cc+"

  // Create a simulated set of ExtMon measurements.
  Measurement meas(spill,efficiency,engine);
  cout << "Simulated measurement mean number of reco ExtMon tracks:  " << meas.mean_reco << endl;

#include "Group.cc+"

  // Properties of the simulated measurements, possibly rebinned.
  Group group(meas.spill.t, meas.m,4);

  // Properties of the raw simulated PBI, possibly rebinned.
  Group group_input(spill.t, spill.val,5);

  double half=spill.tick/2.;
  TH1D* h1 = new TH1D("h1", "Simulated Intensity", int(spill.size()), spill.t.front()-half, spill.t.back()+half );
  TH1D* h2 = new TH1D("h2", "Nominal Reco ExtMon Tracks", int(spill.size()), spill.t.front()-half, spill.t.back()+half );
  TH1D* h3 = new TH1D("h3", "Poisson Reco ExtMon Tracks", int(spill.size()), spill.t.front()-half, spill.t.back()+half );
  TH1D* h4 = new TH1D("h4", "Rebinned Poisson Reconstructed ExtMon Tracks", 
		      int(group.size()), group.t.front()-group.half_tick, group.t.back()+group.half_tick );
  TH1D* h5 = new TH1D("h5", "Bunch to Bunch Spread of PBI", 
		      int(group_input.size()), group_input.t.front()-group_input.half_tick, group_input.t.back()+group_input.half_tick );
  for ( size_t i=0; i<spill.size(); ++i ){
    h1->Fill( spill.t.at(i), spill.val.at(i));
    h2->Fill( spill.t.at(i), meas.meanExtMonTracks.at(i));
    h3->Fill( spill.t.at(i), meas.m.at(i) );
  }

  for ( size_t i=0; i<group.size(); ++i ){
    h4->Fill( group.t.at(i), group.val.at(i) );
  }
  h4->SetError( &group.err.front() );

  for ( size_t i=0; i<group_input.size(); ++i ){
    h5->Fill( group_input.t.at(i), group_input.fraction_minmax.at(i) );
  }

  // Ntuple of input simulated pbi model
  TNtuple* nt1 = new TNtuple ( "nt1", "Simulated PBI", "t:v:frms:fminmax");
  for ( size_t i=0; i<group_input.size(); ++i ){
    float a[4];
    a[0] = group_input.t.at(i);
    a[1] = group_input.val.at(i);
    a[2] = group_input.fraction_rms.at(i);
    a[3] = group_input.fraction_minmax.at(i);
    nt1->Fill(a);
  }

  // Ntuple of the simulated measurement
  TNtuple* nt2 = new TNtuple ( "nt2", "Simulated Measurment", "t:v:frms:fminmax");
  for ( size_t i=0; i<group.size(); ++i ){
    float b[4];
    b[0] = group.t.at(i);
    b[1] = group.val.at(i);
    b[2] = group.fraction_rms.at(i);
    b[3] = group.fraction_minmax.at(i);
    nt2->Fill(b);
  }


  TLine* line = new TLine;
  line->SetLineStyle(2);

  canvas->cd(1);
  h1->DrawCopy("HIST");
  line->DrawLine( spill.t.front(), spill.stats.mean, spill.t.back(), spill.stats.mean );
  gPad->SetLogy(1);
  canvas->cd(2);
  h1->DrawCopy("HIST");
  line->DrawLine( spill.t.front(), spill.stats.mean, spill.t.back(), spill.stats.mean );
  //gPad->SetLogy(1);

  canvas->cd(3);
  h3->Draw("HIST");
  //  gPad->SetLogy(1);

  //canvas->cd(4);
  TH1* copy = h4->DrawCopy("E");
  copy->SetLineColor(kRed);
  //  gPad->SetLogy(1);

  canvas->cd(4);
  h5->DrawCopy("HIST");

  canvas->Print("pbi.pdf");

  pbi->Write();
  pbi->Close();  

}

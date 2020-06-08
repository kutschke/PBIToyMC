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

  // Configurable inputs
  // Number of reconstructed extmon tracks per POT, from Andrei.
  constexpr double efficiency = 0.9e-6;

  // Base name of the input file; will also be used for base name of the output files.
  std::string base_filename = "Normal";
  //std::string const base_filename = "Pathological";

  // Prescale factor to use for making simulated measurements.
  int prescale = 10;

  // Rebin the input and reco time series in groups of:
  int const rebin_input = 5;
  std::vector<int> const rebin_reco{3, 5, 11};

  // End configurable inputs

  std::string const input_filename       = base_filename + ".txt";
  std::string const output_root_filename = base_filename + ".root";

  // Get rid of ugly grey background.
  gROOT->SetStyle("Plain");

  // No statistics box for histograms
  gStyle->SetOptStat("");

  TFile* pbi = new TFile(output_root_filename.c_str(), "RECREATE");

  std::string const title = "Spill Toy MC using " + input_filename;

  TCanvas *canvas = new TCanvas("c", title.c_str(), 900, 900 );
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
  Measurement meas(spill,prescale,efficiency,engine);
  cout << "Simulated measurement mean number of reco ExtMon tracks:  " << meas.mean_reco << endl;

#include "ReBin.cc+"

  // Properties of the input simulated PBI, rebinned.
  ReBin input_rebin(spill.t, spill.val, rebin_input);

  // Properties of the simulated measurements, rebinned.
  std::vector<ReBin> rebinned;
  for ( int i : rebin_reco ){
    rebinned.emplace_back(meas.t, meas.val, i );
  }

  // Create histograms and ntuples.

  const std::string title_input_rebin = "Fractional RMS of Sim Intensity rebinned by " + std::to_string(rebin_input);

  TH1D* hSimSpill    = new TH1D("hSimSpill",    "Simulated Intensity", int(spill.size()), spill.t0, spill.tend );
  TH1D* hSimFRMS     = new TH1D("hSimFRMS",     title_input_rebin.c_str(),
				input_rebin.size(), input_rebin.t0, input_rebin.tend );
  TH1D* hRecoNominal = new TH1D("hRecoNominal", "Nominal Reco ExtMon Tracks", meas.size(), meas.t0, meas.tend  );
  TH1D* hReco        = new TH1D("hReco",        "Poisson Reco ExtMon Tracks", meas.size(), meas.t0, meas.tend );

  std::vector<TH1D*> hrebinned;
  std::vector<TH1D*> hfrms;
  for ( auto const& r : rebinned ){
    std::string name   = "hRecoReBin_" + std::to_string(r.nrebin);
    std::string title  = "Reco ExtMon Tracks rebinned by " + std::to_string(r.nrebin);
    hrebinned.emplace_back ( new TH1D(name.c_str(),   title.c_str(), r.size(), r.t0, r.tend ));
    name  = "hRecoReBinFRMS_" + std::to_string(r.nrebin);
    title = "Fractional RMS Reco ExtMon Tracks rebinned by " + std::to_string(r.nrebin);
    hfrms.emplace_back ( new TH1D(name.c_str(), title.c_str(), r.size(), r.t0, r.tend ) );
  }

  TNtuple* ntPBI = new TNtuple ( "ntPBI", "Simulated PBI", "t:v:frms:fminmax");

  std::vector<TNtuple*> ntReco;
  for ( ReBin const& r : rebinned ){
    std::string const name  = "ntReco_" + std::to_string(r.nrebin);
    std::string const title = "Simulated Measurement Rebinned by " + std::to_string(r.nrebin);
    ntReco.emplace_back( new TNtuple ( name.c_str(), title.c_str(), "t:v:frms:fminmax") );
  }

  // Fill histograms and ntuples.

  for ( size_t i=0; i<spill.size(); ++i ){
    hSimSpill->Fill( spill.t.at(i), spill.val.at(i));
  }
  for ( size_t i=0; i<meas.size(); ++i ){
    hRecoNominal->Fill( meas.t.at(i), meas.meanExtMonTracks.at(i));
    hReco->Fill( meas.t.at(i), meas.val.at(i) );
  }
  hReco->SetError( &meas.err.front() );

  for ( size_t i=0; i<input_rebin.size(); ++i ){
    hSimFRMS->Fill( input_rebin.t.at(i), input_rebin.fraction_rms.at(i) );
  }

  for ( size_t i=0; i<rebinned.size(); ++ i ){
    ReBin const& r = rebinned.at(i);
    TH1D*        h = hrebinned.at(i);
    TH1D*        f = hfrms.at(i);
    for ( size_t j=0; j<r.size(); ++j ){
      h->Fill( r.t.at(j), r.val.at(j) );
      f->Fill( r.t.at(j), r.fraction_rms.at(j) );
    }
    h->SetError( &r.err.front() );
  }

  // Ntuple of input rebinned simulated pbi model
  for ( size_t i=0; i<input_rebin.size(); ++i ){
    float a[4];
    a[0] = input_rebin.t.at(i);
    a[1] = input_rebin.val.at(i);
    a[2] = input_rebin.fraction_rms.at(i);
    a[3] = input_rebin.fraction_minmax.at(i);
    ntPBI->Fill(a);
  }

  for ( size_t i=0; i<rebinned.size(); ++i){
    ReBin const& r = rebinned.at(i);
    TNtuple* nt    = ntReco.at(i);
    for ( size_t j=0; j<r.size(); ++j ){
      float b[4];
      b[0] = r.t.at(j);
      b[1] = r.val.at(j);
      b[2] = r.fraction_rms.at(j);
      b[3] = r.fraction_minmax.at(j);
      nt->Fill(b);
    }
  }

  TLine* line = new TLine;
  line->SetLineStyle(2);

  canvas->cd(1);
  hSimSpill->DrawCopy("HIST");
  line->DrawLine( spill.t.front(), spill.stats.mean, spill.t.back(), spill.stats.mean );
  gPad->SetLogy(1);

  canvas->cd(2);
  hSimSpill->DrawCopy("HIST");
  line->DrawLine( spill.t.front(), spill.stats.mean, spill.t.back(), spill.stats.mean );

  canvas->cd(3);
  hSimFRMS->DrawCopy("HIST");

  canvas->cd(4);
  TH1* copy = hSimFRMS->DrawCopy("HIST");
  copy->SetMinimum(0.);
  copy->SetMaximum(0.1);

  // Prompt and wait for response before continuing.
  cerr << "Double click in the last active pad to print and continue: " ;
  gPad->Update();
  gPad->WaitPrimitive();
  cerr << endl;

  std::string const output_pdf_filename1  = base_filename + "_1.pdf";
  canvas->Print(output_pdf_filename1.c_str());
  cerr << "Print complete." << endl;

  // Page 2
  canvas->cd(0);
  canvas->Clear();
  canvas->Divide(1,4);

  canvas->cd(1);
  copy = hReco->DrawCopy("");
  copy->SetMinimum(0.);

  for ( int i=0; i<3; ++i ){
    canvas->cd(i+2);  
    copy = hrebinned.at(i)->DrawCopy();
    copy->SetMinimum(0.);
  }

  // Prompt and wait for response before continuing.
  cerr << "Double click in the last active pad to print and continue: " ;
  gPad->Update();
  gPad->WaitPrimitive();
  cerr << endl;

  std::string const output_pdf_filename2  = base_filename + "_2.pdf";
  canvas->Print(output_pdf_filename2.c_str());
  cerr << "Print complete." << endl;

  // Page 3
  canvas->cd(0);
  canvas->Clear();
  canvas->Divide(1,4);

  canvas->cd(1);
  hSimSpill->DrawCopy("HIST");
  line->DrawLine( spill.t.front(), spill.stats.mean, spill.t.back(), spill.stats.mean );
  gPad->SetLogy(1);
  
  for ( int i=0; i<3; ++i ){
    canvas->cd(i+2);  
    copy = hfrms.at(i)->DrawCopy("HIST");
  }

  std::string const output_pdf_filename3  = base_filename + "_3.pdf";
  canvas->Print(output_pdf_filename3.c_str());
  cerr << "Print complete." << endl;

  pbi->Write();
  pbi->Close();

  // h->GetXaxis()->SetRange(21,34);

}

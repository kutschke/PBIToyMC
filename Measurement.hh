#ifndef Measurement_hh
#define Measurement_hh

//
// Given a spill computed the mean expected number of reconstructed ExtMon tracks
// and simulate one measurement as Poisson fluctuations about that mean.
// Compute sqrt(n) errors on the measurement - for use by root.
//

#include "Spill.hh"

#include <random>
#include <vector>

struct Measurement{

  Measurement( Spill const& spill,
	       int    prescale,
	       double efficiency,
	       std::default_random_engine& engine );

  size_t size() const { return spill.size(); }

  //std::vector<double> const& t() const { return spill.t; }

  // A reprentation of one spill.
  Spill const& spill;

  // Prescale factor; only keep every nth measurement from the input.
  int prescale;

  // Efficiency per POT to make one reconstructable ExtMon track
  double efficiency;

  // The random engine needed by the Poisson distribution generator.
  std::default_random_engine& engine;

  // Expected number of ExtMon tracks given POT and ExtMon efficiency
  std::vector<double> t, meanExtMonTracks;

  // Simulated measurement with Poisson fluctuations and it's sqrt(n) error.
  std::vector<double> val, err;

  // Mean number of reconstructed tracks
  double mean_reco;

  // Duration (seconds) between values in the time series.
  double tick;
  double half_tick;

  // Time limits for histograms ( put the measured times at bin centers ).
  double t0;
  double tend;

};

#endif

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
	       double efficiency,
	       std::default_random_engine& engine );

  size_t size() const { return spill.size(); }

  //std::vector<double> const& t() const { return spill.t; }

  // A reprentation of one spill.
  Spill const& spill;

  // The random engine needed by the Poisson distribution generator.
  std::default_random_engine& engine;

  // Efficiency per POT to make one reconstructable ExtMon track
  double efficiency;

  // Expected number of ExtMon tracks given POT and ExtMon efficiency
  std::vector<double> meanExtMonTracks;

  // Simulated measurement with Poisson fluctuations and it's sqrt(n) error.
  std::vector<double> m, err;

  // Mean number of reconstructed tracks
  double mean_reco;

};

#endif

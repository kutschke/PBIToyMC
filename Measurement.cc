#include "Measurement.hh"

#include <cmath>

using namespace std;

Measurement::Measurement( Spill const& spill,
			  int prescale,
			  double efficiency,
			  std::default_random_engine& engine ):
  spill(spill),
  prescale(prescale),
  efficiency(efficiency),
  engine(engine){

  val.reserve(spill.size());
  meanExtMonTracks.reserve(spill.size());

  double sum_reco{0.};
  for ( size_t i=0; i<spill.size(); ++i ){
    meanExtMonTracks.push_back( spill.val.at(i)*efficiency);
    std::poisson_distribution<int> poisson( meanExtMonTracks.back() );
    int nreco = poisson(engine);
    t.push_back(spill.t.at(i));
    val.push_back(nreco);
    err.push_back(std::sqrt(nreco));
    sum_reco += val.back();
  }
  mean_reco = sum_reco/spill.size();

  tick        = t.at(1)-t.at(0);
  half_tick   = tick/2.0;
  t0   = t.front() - half_tick;
  tend = t.back()  + half_tick;

  }

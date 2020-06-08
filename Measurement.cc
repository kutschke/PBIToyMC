#include "Measurement.hh"

#include <cmath>

using namespace std;

Measurement::Measurement( Spill const& spill,
			  double efficiency,
			  std::default_random_engine& engine ):
  spill(spill),
  efficiency(efficiency),
  engine(engine){

  m.reserve(spill.size());
  meanExtMonTracks.reserve(spill.size());

  double sum_reco{0.};
  for ( size_t i=0; i<spill.size(); ++i ){
    meanExtMonTracks.push_back( spill.val.at(i)*efficiency);
    std::poisson_distribution<int> poisson( meanExtMonTracks.back() );
    int nreco = poisson(engine);
    m.push_back(nreco);
    err.push_back(std::sqrt(nreco));
    sum_reco += m.back();
  }
  mean_reco = sum_reco/spill.size();

  }

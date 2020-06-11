#include "Measurement.hh"

#include <cmath>
#include <iostream>

using namespace std;

Measurement::Measurement( Spill const& spill,
			  int prescale,
			  double efficiency,
			  std::default_random_engine& engine ):
  spill(spill),
  prescale(prescale),
  efficiency(efficiency),
  engine(engine){

  _size=std::floor(double(spill.size())/double(prescale));
  cout << "Meas size: " << spill.size() << " " << double(spill.size())/double(prescale) << " " << _size << endl;

  val.reserve(_size);
  meanExtMonTracks.reserve(_size);

  double sum_reco{0.};
  for ( size_t i=0; i<_size; ++i ){
    size_t j = i*prescale;
    meanExtMonTracks.push_back( spill.val.at(j)*efficiency);
    std::poisson_distribution<int> poisson( meanExtMonTracks.back() );
    int nreco = poisson(engine);
    t.push_back(spill.t.at(j));
    val.push_back(nreco);
    err.push_back(std::sqrt(nreco));
    sum_reco += val.back();
  }
  mean_reco = sum_reco/_size;

  /*
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
  mean_reco = sum_reco/_size;
  */

  tick        = t.at(1)-t.at(0);
  half_tick   = tick/2.0;
  t0   = t.front() - half_tick;
  tend = t.back()  + half_tick;

  }

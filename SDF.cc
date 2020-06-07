#include "SDF.hh"

#include <cmath>

SDF::SDF():mean(0.), rms(0.), sdf(0.){}

SDF::SDF( std::vector<double> const& v ){

  double sum{0.}, sumsq{0.};
  auto n = v.size();

  for ( auto y : v){
    sum   += y;
    sumsq += y*y;
  }

  mean = sum/double(n);
  rms  = sqrt ( sumsq/double(n) - mean*mean);
  sdf  = 1./( 1. + (rms/mean)*(rms/mean));

}

#ifndef SDF_hh
#define SDF_hh

//
// Compute mean, rms and spill duty factor for a set of input data.
//

#include <vector>

struct SDF{
  SDF( );
  SDF( std::vector<double> const& v );

  double mean;
  double rms;
  double sdf;

};

#endif

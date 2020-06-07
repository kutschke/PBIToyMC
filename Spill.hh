#ifndef Spill_hh
#define Spill_hh

//
// Information about one simulated spill read from an input file.
// The format of each line in the input file is two numbers:
//   ( time in s, number of protons in this pulse)
// 

#include "SDF.hh"

#include <string>
#include <vector>

struct Spill{
  Spill( std::string const& filename );

  size_t size() const { return t.size(); }

  // Input filename.
  std::string filename;

  // Mean, rms and spill duty factor of number of protons per pulse, averaged over the spill
  SDF stats;

  // Duration (seconds) between pulses
  double tick;

  // Times series for the PBI: time (s) and value (number or protons).
  std::vector<double> t, val;

};

#endif

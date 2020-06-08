#ifndef ReBin_hh
#define ReBin_hh
//
// Given a time series of values, rebin the times and values by grouping bins
// bins by the argument nrebin.  If the size of the time series is not equally
// divisible by nrebin, the output time series will ignore the trailing elements
// of the input time series.  In the output time series the value of each bin 
// is the mean of the rebinned values from the input time series.
//
// c'tor  Arguments:
// in_t   : values of time in the input time series
// in_val : value of the quantity in the input time series
// nrebin : take the mean of nrebin values from the input time series to make
//          one entry in the output time series.

#include <string>
#include <vector>

struct ReBin{

  ReBin( std::vector<double>const& in_t, std::vector<double> const& in_val, int nrebin );

  // Number of entries in the rebinned data.
  size_t _size;
  size_t size() const { return _size; }

  // Rebinning factor
  int nrebin;

  // One tick of the rebinned time series;
  double tick;
 
  // One half of one tick of the rebinned time series.
  double half_tick;
 
  // Rebinned values of time, measurement and error bar on measurement.
  std::vector<double> t, val, err;

  // Values of rms and rms/mean for the rebinned data; these are only
  // meaningful if rebin factor is > 1.
  std::vector<double> rms, fraction_rms, fraction_minmax;

};

#endif

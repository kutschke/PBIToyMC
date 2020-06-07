#ifndef Group_hh
#define Group_hh
//
// Given a time series of values, rebin the times and values by grouping bins
// bins by the argument ngroup.  If the size of the time series is not equally
// divisible by ngroup, the output time series will ignore the trailing elements
// of the input time series.  In the output time series the value of each bin 
// is the mean of the group values from the input time series.
//
// c'tor  Arguments:
// in_t   : values of time in the input time series
// in_val : value of the quantity in the input time series
// ngroup : take the mean of ngroup values from the input time series to make
//          one entry in the output time series.

#include <string>
#include <vector>

struct Group{

  Group( std::vector<double>const& in_t, std::vector<double> const& in_val, int ngroup );

  // Number of entries in the rebinned data.
  size_t _size;
  size_t size() const { return _size; }

  // Regrouping factor
  int ngroup;

  // One tick of the rebinned time series;
  double tick;
 
  // One half of one tick of the rebinned time series.
  double half_tick;
 
  // Rebinned values of time, measurement and error bar on measurement.
  std::vector<double> t, val, err;

  // Values of rms and rms/mean for the rebinned data; these are only
  // meaningful if group factor is > 1; otherwise set to 0.
  std::vector<double> rms, fraction_rms, fraction_minmax;

};

#endif

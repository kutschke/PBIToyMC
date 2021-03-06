#include "ReBin.hh"

#include <cmath>
#include <iostream>
#include <limits>

using namespace std;

ReBin::ReBin( std::vector<double>const& in_t, std::vector<double> const& in_val, int nrebin ): _size(0), nrebin(nrebin){

  double rebin=nrebin;

  _size = std::floor(double(in_t.size())/double(nrebin));

  double in_tick = in_t.at(1) - in_t.at(0);

  cout << "Rebin factor: " << in_t.size()/rebin << " " << size() << endl;
  cout << "End test:     " << ( in_t.back()-in_tick*(in_t.size()-1)) << endl;

  t.reserve(size());
  val.reserve(size());
  err.reserve(size());
  rms.reserve(size());
  fraction_rms.reserve(size());
  fraction_minmax.reserve(size());

  cout << "Size is ... " << size() << " " << nrebin << endl;

  int j=0;
  for ( size_t i=0; i<size(); ++i ){
    double vsum{0.};
    double vsumsq{0.};
    double tsum{0.};
    double vmax{0.};
    double vmin{std::numeric_limits<double>::max()};
    for ( int k=0; k<nrebin; ++k){
      tsum += in_t.at(j);
      vsum += in_val.at(j);
      vsumsq += in_val.at(j)*in_val.at(j);
      vmax = std::max( vmax, in_val.at(j) );
      vmin = std::min( vmin, in_val.at(j) );
      ++j;
    }
    double tmean = tsum/rebin;
    double vmean = vsum/rebin;
    double vrms  = sqrt( vsumsq/rebin - vmean*vmean);
    double vdiff = vmax-vmin;
    err.push_back(sqrt(vsum)/rebin);
    t.push_back(tmean);
    val.push_back(vmean);
    rms.push_back(vrms);
    if ( vmean != 0. ) {
      fraction_rms.push_back(vrms/vmean);
      fraction_minmax.push_back(vdiff/vmean);
    }else{
      fraction_rms.push_back(1.);
      fraction_minmax.push_back(1.);
    }
  }
  tick      = t.at(1)-t.at(0);
  half_tick = tick/2.;

  t0   = t.front()-half_tick;
  tend = t.back() + half_tick;
}

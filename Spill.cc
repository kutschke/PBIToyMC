#include "Spill.hh"

#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

Spill::Spill( std::string const& filename ):filename(filename){
  ifstream in(filename.c_str());
  while (in){
    double tt, p;
    in >> tt >> p;
    if  ( !in ){
      break;
    }
    t.push_back(tt);
    val.push_back(p);
  }
  stats = SDF(val);

  tick = t.at(1)-t.at(0);
  half_tick = tick/2.0;
  t0   = t.front() - half_tick;
  tend = t.back()  + half_tick;

  // Debug
  //double tend  = tick*(t.size()-1);
  //double delta = (tend-t.back())/tick;
  //cout << "Spill check: " << tick << " " << tend << " " << tend-t.back() << " "  << delta << endl;

}

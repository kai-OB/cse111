// $Id: granularity.cpp,v 1.26 2021-02-26 16:38:14-08 - - $

#include <chrono>
#include <iomanip>
#include <vector>
#include <iostream>
using namespace std;

using hr_clock = chrono::high_resolution_clock;

struct info {
   size_t cycle_nr;
   double microsecs;
};

int main (int argc, char** argv) {
   vector<info> data;
   constexpr size_t MAXCYCLES = 1'000'000;
   data.reserve (MAXCYCLES);
   double threshhold_microsec = argc == 1 ? 0 : atof (argv[1]);
   hr_clock::time_point prev = hr_clock::now();
   cout << fixed << setprecision(6);
   for (size_t cycle = 0; cycle < MAXCYCLES; ++cycle) {
      hr_clock::time_point now = hr_clock::now();
      double seconds = chrono::duration<double> (now - prev).count();
      double microsec = seconds * 1.0e6;
      if (microsec > threshhold_microsec) {
         data.push_back ({cycle, microsec});
      }
      prev = now;
   }
   for (const auto& item: data) {
         cout << setw(12) << item.cycle_nr
              << setw(16) << item.microsecs << " microseconds" << endl;
   }
   return 0;
}


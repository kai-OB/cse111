// $Id: raceclick.cpp,v 1.9 2016-08-09 15:22:27-07 - - $

// Hello to threads.

#include <iostream>
#include <thread>
#include <vector>
using namespace std;

constexpr size_t LOOPS = 10'000'000;

void racer (size_t id, size_t* counter) {
   for (size_t i = 0; i < LOOPS; ++i) ++*counter;
   cout << "racer " << id << " looped " << LOOPS
        << " finished *counter " << *counter << endl;
}

struct comma_t: numpunct<char> {
   char do_thousands_sep() const { return ','; }
   string do_grouping() const { return "\03"; }
};
locale comma (locale(), new comma_t());

int main () {
   cout.imbue (comma);
   size_t counter {0};
   cout << "main starting" << endl;
   vector<thread> vecthr;
   for (size_t i = 0; i < 4; ++i) {
      vecthr.push_back (thread (racer, i, &counter));
   }
   for (auto& t: vecthr) t.join();
   cout << "main finished counter " << counter << endl;
   return 0;
}

//TEST// alias TIME='/usr/bin/time -f "%E elapsed, %S kernel, %U user"'
//TEST// for i in 1 2 3 4
//TEST// do
//TEST//    TIME raceclick >raceclick.out$i 2>&1
//TEST// done
//TEST// more raceclick.out* >raceclick.lis </dev/null
//TEST// mkpspdf raceclick.ps raceclick.cpp* raceclick.lis


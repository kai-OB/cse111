// $Id: atomicclick.cpp,v 1.6 2016-08-09 15:21:43-07 - - $

// Hello to threads.

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

constexpr size_t LOOPS = 10'000'000;

void clicker (size_t id, atomic<size_t>& atomic_counter) {
   for (size_t i = 0; i < LOOPS; ++i) ++atomic_counter;
   cout << "clicker " << id << " looped " << LOOPS
        << " finished atomic_counter " << atomic_counter << endl;
}

struct comma_t: numpunct<char> {
   char do_thousands_sep() const { return ','; }
   string do_grouping() const { return "\03"; }
};
locale comma (locale(), new comma_t());

int main () {
   cout.imbue (comma);
   atomic<size_t> atomic_counter {0};
   cout << "main starting" << endl;
   vector<thread> vecthr;
   for (size_t i = 0; i < 4; ++i) {
      vecthr.push_back (thread (clicker, i, ref (atomic_counter)));
   }
   for (auto& t: vecthr) t.join();
   cout << "main finished atomic_counter " << atomic_counter << endl;
   return 0;
}

//TEST// alias TIME='/usr/bin/time -f "%E elapsed, %S kernel, %U user"'
//TEST// for i in 1 2 3 4
//TEST// do
//TEST//    TIME atomicclick >atomicclick.out$i 2>&1
//TEST// done
//TEST// more atomicclick.out* >atomicclick.lis </dev/null
//TEST// mkpspdf atomicclick.ps atomicclick.cpp* atomicclick.lis


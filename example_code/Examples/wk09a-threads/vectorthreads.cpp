// $Id: vectorthreads.cpp,v 1.11 2021-03-04 17:56:55-08 - - $

// Have a vector of chatty threads.

#include <iostream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

void hello (size_t seqnr) {
   cout << "Hello " << seqnr << " is thread id "
        << this_thread::get_id() << endl;
}

int main () {
   cout << "thread::hardware_concurrency() = "
        << thread::hardware_concurrency() << endl;
   vector<thread> threads;
   for (size_t i = 0; i < 8; ++i) {
      threads.push_back (thread (hello, i));
      cout << "Starting thread " << i << " " << threads.back().get_id()
           << endl;
   }
   for (auto& thr: threads) {
      cout << "Joining " << thr.get_id() << endl;
      thr.join();
   }
}

//TEST// vectorthreads >vectorthreads.out
//TEST// mkpspdf vectorthreads.ps vectorthreads.cpp* vectorthreads.out

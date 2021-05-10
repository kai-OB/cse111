// $Id: invalid-mem.cpp,v 1.1 2021-02-04 13:30:09-08 - - $

// Illustrate invalid memory access.

#include <iostream>
#include <libgen.h>
using namespace std;

void deref_nullptr() {
   cout << __PRETTY_FUNCTION__ << endl;
   int* p = nullptr;
   cout << *p << endl;
}

void bad_pointer() {
   cout << __PRETTY_FUNCTION__ << endl;
   int* p = reinterpret_cast<int*> (0xFFFFFF);
   cout << *p << endl;
}

void deleted_pointer() {
   cout << __PRETTY_FUNCTION__ << endl;
   int* p = new int (1234);
   cout << *p << endl;
   delete p;
   cout << *p << endl;
}

void subscript_range() {
   cout << __PRETTY_FUNCTION__ << endl;
   int* p = new int [10];
   int n = p[1000];
   cout << n << endl;
}

void uninit_array() {
   cout << __PRETTY_FUNCTION__ << endl;
   int* p = new int [10];
   int n = p[5];
   cout << n << endl;
}

int main (int argc, char** argv) {
   const char* exec = basename (argv[0]);
   switch (argc > 1 ? argv[1][0] : 0) {
      case 'b': bad_pointer(); break;
      case 'd': deleted_pointer(); break;
      case 'n': deref_nullptr(); break;
      case 's': subscript_range(); break;
      case 'u': uninit_array(); break;
      default: cout << "Usage: " << exec << " b|d|n|s|u" << endl;
   }
}

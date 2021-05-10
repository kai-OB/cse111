// $Id: boolalpha.cpp,v 1.5 2018-06-20 14:16:54-07 - - $

// Printing booleans as alpha.

#include <cstdlib>
#include <iostream>

using namespace std;

void true_false () {
   cout << "true = " << true << ", false = " << false << endl;
}

int main() {
   true_false ();
   cout << boolalpha;
   true_false ();
   return EXIT_SUCCESS;
}

//TEST// ./boolalpha 1>boolalpha.out 2>&1
//TEST// catnv boolalpha.cpp.log boolalpha.out >boolalpha.lis
//TEST// rm -v boolalpha.cpp.log boolalpha.out
//TEST// mkpspdf boolalpha.ps boolalpha.cpp* boolalpha.lis


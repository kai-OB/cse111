// $Id: refs-ptrs.cpp,v 1.3 2021-03-30 18:27:07-07 - - $

//
// NAME
//    refs-ptrs - illustrate differences between pointers
//                and references.
//

#include <iostream>
using namespace std;

int main() {
   int n = 6; // simple integer.
   int* p = &n; // pointer to int n.
   int& r = n; // reference to int n.
   cout << "n = " << n << endl;
   cout << "p = " << p << endl;
   cout << "*p = " << *p << endl;
   cout << "r = " << r << endl;
   r = 99;
   cout << "n = " << n << endl;
   cout << "&r = " << &r << endl;
   return 0;
}

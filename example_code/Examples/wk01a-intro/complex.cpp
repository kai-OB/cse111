// $Id: complex.cpp,v 1.6 2021-01-05 01:01:36-08 - - $

#include <iostream>
using namespace std;

struct complex {
   double re;
   double im;
};

complex operator+ (complex a, complex b) {
   return {a.re + b.re, a.im + b.im};
}

ostream& operator<< (ostream& out, const complex x) {
   return out << "{" << x.re << "," << x.im << "}";
}

int main() {
   complex a {1,2};
   complex b {3,4};
   complex c = a + b;
   cout << a << " + " << b << " = " << c << endl;
   return 0;
}


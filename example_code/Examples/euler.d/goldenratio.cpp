// $Id: goldenratio.cpp,v 1.1 2021-01-18 23:43:17-08 - - $

#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

using real = long double;
constexpr real phi = (1 + sqrt(5)) / 2;
constexpr real psi = 1 / phi;
constexpr size_t digits10 = numeric_limits<real>::digits10;
constexpr real epsilon = numeric_limits<real>::epsilon();

real mathfib (long n) {
   return (pow (phi, n) - pow ((1 - phi), n)) / sqrt(5);
}

int main() {
   cout << setprecision(digits10);
   cout << "phi = " << phi << endl;
   cout << "psi = " << psi << endl;
   cout << "digits10 = " << digits10 << endl;
   cout << "epsilon = " << epsilon << endl;
   real prevfib = 0;
   for (long n = 0; n < 50; ++n) {
      real currfib = mathfib(n);
      real delta = prevfib / currfib;
      real diff = delta - psi;
      cout << "fib(" << setw(2) << n << ") = "
           << defaultfloat << setw(12) << round (currfib);
      if (n > 0) {
         cout << ": " << fixed << setw(digits10 + 2) << delta
              << ": " << scientific << setw(digits10 + 7) << diff;
      }
      cout << endl;
      prevfib = currfib;
      if (n > 0 and abs (diff) < epsilon) break;
   }
}

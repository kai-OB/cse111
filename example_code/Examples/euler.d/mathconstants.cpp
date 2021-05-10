// $Id: mathconstants.cpp,v 1.5 2021-01-19 00:01:37-08 - - $

#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

using long_double = long double;

int main() {

   constexpr auto PRECISION = numeric_limits<long_double>::digits10;
   cout << setprecision (PRECISION);

   long_double pi = acos (-1.0L);
   long_double e = exp (1.0L);
   long_double phi = (1.0L + sqrt (5.0L)) / 2.0L;

   cout << "pi  = " << pi  << endl;
   cout << "e   = " << e   << endl;
   cout << "phi = " << phi << endl;

   return 0;
}

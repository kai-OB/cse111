// $Id: long-double-pi.cpp,v 1.6 2020-09-06 13:04:14-07 - - $

#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

#define SHOW(X) cout << X << " = " << #X << endl;

int main() {
   using long_double = long double;
   constexpr auto LONG_DOUBLE_PRECISION
         = numeric_limits<long_double>::digits10;
   cout << setprecision (LONG_DOUBLE_PRECISION + 4);
   SHOW (LONG_DOUBLE_PRECISION);
   SHOW (3.141592653589793238462643383279502884197L);
   SHOW (M_PI);
   SHOW (M_PIl);
   SHOW (acos (-1.0L));
   SHOW (4.0L * atan (1.0L));
   SHOW (atan2 (0.0L, -1.0L));
   return 0;
}

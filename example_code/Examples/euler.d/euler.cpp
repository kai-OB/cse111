// $Id: euler.cpp,v 1.2 2020-09-06 12:37:31-07 - - $

#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
using namespace std;

#define SHOW(X) cout << #X << " = " << (X) << endl;

#define TEST(X) { \
           cout << endl; \
           SHOW (euler_exp (X)); \
           SHOW (euler_trig (X)); \
           SHOW (abs (euler_exp (X) - euler_trig (X)) <= D_EPSILON); \
        }


using d_complex = complex<double>;
constexpr auto D_PRECISION = numeric_limits<double>::digits10;
constexpr auto D_EPSILON = numeric_limits<double>::epsilon();

const d_complex DC_PI = acos (d_complex (-1.0L));
const d_complex DC_I = sqrt (d_complex (-1.0L));

d_complex euler_exp (d_complex arg) {
   return exp (DC_I * arg);
}

d_complex euler_trig (d_complex arg) {
   return cos (arg) + DC_I * sin (arg);
}

int main() {
   cout << boolalpha << setprecision (D_PRECISION + 4);
   SHOW (D_PRECISION);
   SHOW (D_EPSILON);
   SHOW (DC_PI);
   SHOW (DC_I);
   TEST (DC_PI);
   TEST (DC_I);
   TEST (0);

   cout << endl << "What is epsilon?" << endl;
   double test_epsilon = 1e-6L;
   for (;;) {
      test_epsilon /= 10.0L;
      double sum = 1.0L + test_epsilon;
      cout << 1.0 << " + " << test_epsilon << " = " << sum << endl;
      if (sum <= 1.0L) break;
   }
   return 0;
}

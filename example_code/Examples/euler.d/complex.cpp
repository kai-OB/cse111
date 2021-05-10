// $Id: complex.cpp,v 1.17 2021-01-29 21:51:48-08 - - $

#include <climits>
#include <complex>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

using dcomplex = complex<double>;
using ldouble = long double;
using ulonglong = unsigned long long;

constexpr auto DPRECISION = numeric_limits<double>::digits10;

dcomplex operator"" _i (ldouble x) { return dcomplex (0, x); }
dcomplex operator"" _i (ulonglong x) { return dcomplex (0, x); }
dcomplex operator"" _r (ldouble x) { return dcomplex (x, 0); }
dcomplex operator"" _r (ulonglong x) { return dcomplex (x, 0); }

ostream& operator<< (ostream& out, dcomplex c) {
   cout << c.real();
   if (c.imag()) cout << "+" << c.imag() << "i";
   return out;
}

int main(){
   cout << setprecision (DPRECISION);
   cerr << setprecision (DPRECISION);
   dcomplex d = 1;
   cout << d << endl;
   cout << sqrt (-1_r) << endl;
   cout << 10.0_i << endl;
   cout << -12_i << endl;
   cout << 0.0 - 12_i << endl;
}

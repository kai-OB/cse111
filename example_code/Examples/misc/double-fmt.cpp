// $Id: double-fmt.cpp,v 1.60 2021-04-26 20:12:34-07 - - $

#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

constexpr size_t double_digits = numeric_limits<double>::digits10;
constexpr size_t bits52 = 1LU << 52;

struct double_bits {
   long unsigned significand: 52;
   unsigned exponent: 11;
   unsigned sign: 1;
};

union double_union {
   double fvalue;
   uint64_t bits;
   double_bits fields;
};

void print (const char* label, double_union number) {
   for (int count = 0; count <= 1; ++count) {
      int exp;
      double fraction = frexp (number.fvalue, &exp);
      cout << (number.fields.sign ? '-' : '+')
           << " " << label << " == ";
      cout << setprecision (double_digits) << number.fvalue << " == "
           << dec << fraction << " * (2 ** " << exp << ")" << endl;
      cout << "        == " << "0x" << setfill('0') << hex
           << setw(16) << number.bits;
      cout << " == {" << number.fields.sign 
           << ", 0x" << hex << setw(3) << number.fields.exponent
           << ", 0x" << hex << setw(13) << number.fields.significand
           << "}" << endl;
      number.fields.sign = ~ number.fields.sign;
   }
}

#define PRINT(X) print (#X, {X});

int main() {
   PRINT (0.0);
   PRINT (0.0 / 0.0);
   PRINT (1.0);
   PRINT (1.0 / 0.0);
   PRINT (acos (M_PI));
   PRINT (1.0 / 3.0);
   PRINT (511.0);
   PRINT (512.0);
}


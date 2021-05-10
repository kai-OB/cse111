// $Id: traced-abs.cpp,v 1.2 2021-04-27 16:52:16-07 - - $

#include <bitset>
#include <climits>
#include <iomanip>
#include <iostream>
using namespace std;

using intsize_t = int16_t;
constexpr size_t BIT_SIZE = CHAR_BIT * sizeof (intsize_t);
constexpr size_t DIGIT_SIZE = numeric_limits<intsize_t>::digits10 + 2;

intsize_t abs (intsize_t n) {
   intsize_t mask = n >> (BIT_SIZE - 1);
   return (n ^ mask) - mask;
}

void print (const char* label, intsize_t x) {
   cout << setw (DIGIT_SIZE) << x << " = " << bitset<BIT_SIZE>(x)
        << " = " << label << endl;
}
#define PRINT(X) print (#X, X);

void traced_abs (intsize_t n) {
   cout << endl << __func__ << "(int" << BIT_SIZE << "_t)" << endl;
   PRINT (n);
   intsize_t mask = n >> (BIT_SIZE - 1);
   PRINT (mask);
   PRINT (n ^ mask);
   PRINT ((n ^ mask) - mask);
}

#define SHOWABS(X) cout << "abs(" << X << ") = " << abs(X) << endl;
int main (int argc, char** argv) {
   if (argc == 1) {
      cerr << "Usage: " << argv[0] << " int..." << endl;
      return 1;
   }
   cout << showpos;
   for (int i = 1; i < argc; ++i) {
      cout << argv[0] << " " << argv[i] << endl;
      intsize_t n = atoi (argv[i]);
      SHOWABS (+n);
      SHOWABS (-n);
      traced_abs (+n);
      traced_abs (-n);
   }
}


// $Id: numlimits.cpp,v 1.31 2019-11-01 17:58:07-07 - - $

#include <iomanip>
#include <iomanip>
#include <iostream>
#include <limits>
#include <typeinfo>

#include <cxxabi.h>

using namespace std;

template <typename T>
void show (const string &label) {
   cout << setw(2) << sizeof (T)
        << setw(3) << numeric_limits<T>::digits10
        << " " << setw(19) << setiosflags(ios::left)
               << label << resetiosflags(ios::left)
        << setprecision (numeric_limits<T>::digits10)
        << +numeric_limits<T>::min() << " .. "
        << +numeric_limits<T>::max() << endl;
}

#define SHOW(T) show<T>(#T)

int main() {
   SHOW (bool);
   SHOW (char);
   SHOW (signed char);
   SHOW (unsigned char);
   SHOW (short);
   SHOW (signed short);
   SHOW (unsigned short);
   SHOW (int);
   SHOW (signed int);
   SHOW (unsigned int);
   SHOW (unsigned);
   SHOW (long);
   SHOW (signed long);
   SHOW (unsigned long);
   SHOW (long long);
   SHOW (signed long long);
   SHOW (unsigned long long);
   SHOW (float);
   SHOW (double);
   SHOW (long double);
   SHOW (void*);
   SHOW (uintptr_t);
   SHOW (wchar_t);
   SHOW (char16_t);
   SHOW (char32_t);
   return 0;
}

//TEST// ./numlimits >numlimits.out 2>&1
//TEST// mkpspdf numlimits.ps numlimits.cpp* numlimits.out*


// $Id: ubigint.h,v 1.5 2020-10-11 12:25:22-07 - - $

#ifndef __UBIGINT_H__
#define __UBIGINT_H__

#include <exception>
#include <iostream>
#include <limits>
#include <vector>
#include <utility>
using namespace std;

#include "debug.h"

class ubigint {
   friend ostream& operator<< (ostream&, const ubigint&);
   private:
      using ubigvalue_t = vector<uint8_t>;
      ubigvalue_t uvalue;  
      //per instructions in assignment
   public:
      void multiply_by_2();
      void divide_by_2();

      ubigint() = default; // Need default ctor as well. //implements an empty vector
      ubigint (unsigned long);   //unsigned long can be used in combination with ubigint values
      ubigint (const string&);   //used in main becasue the scanner returns string as an argument

      ubigint operator+ (const ubigint&) const;
      ubigint operator- (const ubigint&) const;
      ubigint operator* (const ubigint&) const;
      ubigint operator/ (const ubigint&) const;
      ubigint operator% (const ubigint&) const;

      bool operator== (const ubigint&) const;
      bool operator<  (const ubigint&) const;
};

#endif


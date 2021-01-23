// $Id: ubigint.cpp,v 1.12 2020-10-19 13:14:59-07 - - $

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "debug.h"
#include "relops.h"
#include "ubigint.h"

ubigint::ubigint (unsigned long that): /*uvalue (that)*/ {
  // DEBUGF ('~', this << " -> " << uvalue); FIGURE OUT HOW TO DO DEBUG THEN UNCOMMENT 
   //take the rightmost digit and do a pushback into the result vector
   //delete this initializer list and make a loop inside the constructor
   //copy string constructor?
  // for (char digit: that) {
      //if (not isdigit (digit)) {
      //   throw invalid_argument ("ubigint::ubigint(" + that + ")");
     // }
     // uvalue = uvalue * 10 + digit - '0';
      //pushback
  // }

   while(that > 0){
      ubig_value.push_back(that% 0);
      (that/10);
   }
   
}

ubigint::ubigint (const string& that): /* init in default uvalue(0)*/ {
  // DEBUGF ('~', "that = \"" << that << "\""); FIGURE OUT HOW TO DO DEBUG THEN UNCOMMEMT
   for (char digit: that) {
      //for (char digit = that.crbegin(); digit!= that.crend(); ++ digit){
      if (not isdigit (digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }
     // uvalue = uvalue * 10 + digit - '0';//this code isnt going to work?
      ubig_value.push_back(digit-'0');
   }
   //get rid of the initializer list and loop over the string in reverse
   // direction from end to front
   // just reverse at the end actually
   //for each digit in the string subtract off "0" and do a pushback on 
   // to the vector
   reverse(ubig_value.begin(),ubig_value.end());
   //reverse ubig_value
}

ubigint ubigint::operator+ (const ubigint& that) const {
   DEBUGF ('u', *this << "+" << that);
   ubigint result (uvalue + that.uvalue);
   DEBUGF ('u', result);
   return result;
}

ubigint ubigint::operator- (const ubigint& that) const {
   if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
   return ubigint (uvalue - that.uvalue);
}

ubigint ubigint::operator* (const ubigint& that) const {
   return ubigint (uvalue * that.uvalue);
}

void ubigint::multiply_by_2() {
   uvalue *= 2;
}

void ubigint::divide_by_2() {
   uvalue /= 2;
}


struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, const ubigint& divisor_) {
   // NOTE: udivide is a non-member function.
   ubigint divisor {divisor_};
   ubigint zero {0};
   if (divisor == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 {1};
   ubigint quotient {0};
   ubigint remainder {dividend}; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   DEBUGF ('/', "quotient = " << quotient);
   DEBUGF ('/', "remainder = " << remainder);
   return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
   return udivide (*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
   return udivide (*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {
   return uvalue == that.uvalue;
}

bool ubigint::operator< (const ubigint& that) const {
   return uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const ubigint& that) { 
   return out << "ubigint(" << that.uvalue << ")";
}


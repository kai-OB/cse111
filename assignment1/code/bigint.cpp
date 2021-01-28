// $Id: bigint.cpp,v 1.3 2020-10-11 12:47:51-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
  
     
}

bigint::bigint (const ubigint& uvalue_, bool is_negative_):
                uvalue(uvalue_), is_negative(is_negative_) {//dont change?
               

}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0));


}

bigint bigint::operator+ () const { //returns argument
   return *this;
}

bigint bigint::operator- () const { //flips the sign
   return {uvalue, not is_negative};
}

bigint bigint::operator+ (const bigint& that) const {
   ubigint plus_result;
   bool sign = false; //(positive)
   // both negative
   if(is_negative and is_negative){   
      plus_result = uvalue + that.uvalue; //calls uvalue operator+
      sign = true; //sign is true (negative)
   }
   // both positive
   else if(!is_negative and !is_negative){  
      plus_result = uvalue + that.uvalue; //calls uvalue operator+
      sign = false;  //set sign to false 
   }

   // signs are different
   else{
   
      if(uvalue > that.uvalue){ //if one is greater or equal
         sign = is_negative; //sign = the larger sign
         plus_result = uvalue - that.uvalue;   //subtract 
      }
      else if (that.uvalue > uvalue){
         sign = that.is_negative;   //sign of bigger number
         plus_result = that.uvalue - uvalue;   //subtract 
      }
      else if(that.uvalue == uvalue){  //ex: +10 -10 = 0
         sign = false;  // 0 is positive
         plus_result = 0;
      }
   }

   return {plus_result, sign};   //returns appropriate bigint and sign
}

bigint bigint::operator- (const bigint& that) const {
   ubigint sub_result;
   bool sign = false; //(positive)

   // both negative
   if(is_negative and is_negative){   
      sub_result = uvalue + that.uvalue; //calls uvalue operator+
      sign = false; //sign is positive (negative)
   }
   else if(!is_negative and !is_negative){  
      if(uvalue > that.uvalue){ //if one is greater or equal
         sign = false; //both positive so remain positive
         sub_result = uvalue - that.uvalue;   //subtractfrom larger 
      }
      else if (that.uvalue > uvalue){
         sign = true;   // 1-2 = -1
         sub_result = that.uvalue - uvalue;   //subtract from larger
      }
      else if(that.uvalue == uvalue){  //ex: +10 -10 = 0
         sign = false;  // 0 is positive
         sub_result = 0;
      }
   }
   else{ //signs are different
      if(uvalue > that.uvalue){ //if one is greater or equal
         sign = is_negative; //sign of uvalue
         sub_result = uvalue + that.uvalue;   //subtract from larger 
      }
      else if (that.uvalue > uvalue){  //that.uvalue can be pos or neg
         sign = that.is_negative;   //sign of that.uvalue
         sub_result = that.uvalue + uvalue;   //subtract from larger
      }
   }
   return {sub_result , sign};
}


bigint bigint::operator* (const bigint& that) const {
   bool sign = true;
   if(is_negative == that.is_negative){
      sign = false;
   }
   ubigint times_result = uvalue * that.uvalue;
   

   return {times_result , sign};
}

bigint bigint::operator/ (const bigint& that) const {
   bool sign = true;
   if(is_negative == that.is_negative){
      sign = false;
   }
   ubigint div_result = uvalue / that.uvalue;
   return {div_result,sign};
}

bigint bigint::operator% (const bigint& that) const {
   bool sign = true;
   if(is_negative == that.is_negative){
      sign = false;
   }
   ubigint mod_result = uvalue % that.uvalue;
   return {mod_result,sign};
}

bool bigint::operator== (const bigint& that) const {
   return is_negative == that.is_negative and uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
   if (is_negative != that.is_negative) return is_negative;
   return is_negative ? uvalue > that.uvalue
                      : uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
   
   out << "bigint(" << (that.is_negative ? "-" : "+");
      out << "\n" << that.uvalue << ")";  //should call uvalue print statement
   //^cdfpq
   return out;
              
}


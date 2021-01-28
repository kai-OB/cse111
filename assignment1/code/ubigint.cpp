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
//////////////////////////////
//CLEAN UP CODE!!!!!!!!!!
///////////////////////////////
/////////////////////////////
///////////////////////
////////////////////////
////////////////////////////////////////////////////////////////////////////////

ubigint::ubigint (unsigned long that): uvalue (that) {
   // DEBUGF ('~', this << " -> " << uvalue); 
   //copy string constructor?
   string s = to_string(that);   //casting that as a string
   /* for (ssize_t index = s.size() - 1; index >= 0; --index){
     
      uvalue.push_back(index-'0');  //casting as int?
   }*/
   for (char digit: s) {
      /*if (not isdigit (digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }*/
      //uvalue = uvalue * 10 + digit - '0';
      uvalue.push_back(digit);
   }
}

ubigint::ubigint (const string& that): uvalue(0) {
   // DEBUGF ('~', "that = \"" << that << "\""); 
  
   for (ssize_t index = that.size() - 1; index >= 0; --index){
      uvalue.push_back(index-'0');  //casting as int?
   }
   //this should store in correct order??
}

ubigint ubigint::operator+ (const ubigint& that) const {
  // DEBUGF ('u', *this << "+" << that);
   ubigint add_result;
   unsigned int sum; // sum of two digits pairwise while adding the vectors
   unsigned int carry = 0;  // remainder to be carried if sum>=10
   ssize_t longer; //longer vector to be added (may be same length!) ssize_t is a typedef long
   
   if(uvalue.size() > that.uvalue.size()){      //sets longer and shorter values
      longer = uvalue.size();   //so that "When you run out of digits in the shorter number, [you can] 
   }
   else{ //this->value is the same as uvalue
      longer = that.uvalue.size();  //might not need this idk!!!!!!
    //  shorter = uvalue.size();
   }

   for(auto i = 0; i < longer; i++){   //should keep adding when the shorter vector runs out
      sum = carry;      //should i be auto?
      carry = 0;
      
      if( i <= uvalue.at(i) ){ // <= or <?   //if this hasnt run out then add
         sum+= uvalue.at(i);
      }
      if(i <= that.uvalue.at(i) ){  //if that hasnt run out then add
         sum += that.uvalue.at(i);
      }
      if(sum >9){ // if there is a remainder then:
         carry = 1;  //carry 1 to start loop over with sum = carry
         sum = (sum % 10);   // and mod 10
      }
      add_result.uvalue.push_back(sum); //push back the sum of two digits (or longer and 0)
   }
   /*After computing a value from any one of the six arithmetic
   operators, always trim the vector by removing all high-order zeros :*/
   while (add_result.uvalue.size() > 0 and add_result.uvalue.back() == 0){ 
      add_result.uvalue.pop_back();
   }
   return add_result;

}

ubigint ubigint::operator- (const ubigint& that) const {
   if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
   ubigint sub_result;  //empty vector
   unsigned int sub = 0; //subtraction of two digits
   unsigned int borrow = 0;   //digit to be borrowed

   for(unsigned int i = 0; i < that.uvalue.size(); i++){  //that will be bigger
      sub -= borrow; //subtract borrow 
      borrow = 0;

      sub = uvalue.at(i) - that.uvalue.at(i); // sub can be negative so just sub
      
      if( sub < 0){
         sub += 10;
         borrow = -1;
      }

      sub_result.uvalue.push_back(sub);
   }

   while (sub_result.uvalue.size() > 0 and sub_result.uvalue.back() == 0){ 
      sub_result.uvalue.pop_back();
   }
      
   return sub_result;
 
}

ubigint ubigint::operator* (const ubigint& that) const {
  // return ubigint (uvalue * that.uvalue);
  ubigint product(0);  //empty vector
  unsigned int digit = 0;   //mult of each digit
  unsigned int carry = 0;  //digit to be carried

  //make product the size of uvalue+that.uvalue
   int prod_size = (uvalue.size()+that.uvalue.size())-1;
   int k = 0;
   while(k < prod_size){                  //initializes the vector to all zeroes
      product.uvalue.push_back('0' + 0);  //allocating a new vector equal to the 
      k++;                                //sums of the sizes of the two vectors
   } 

  for(unsigned int i = 0; i < uvalue.size(); i++){  //i E [0,m)
      carry = 0;  // c <-0
      for(unsigned int j = 0; j < that.uvalue.size(); j++){  //j E [0,n)
         // d <- P(i+j) + u(i)v(j) + c
         digit = (product.uvalue.at(i+j)+(uvalue.at(i) * that.uvalue.at(i))+carry);
      
         product.uvalue.at(i+j) = digit % 10; // P(i+j) <- d rem 10

         carry = /*floor*/(digit / 10);   // c <- floor (d / 10)
         
      }
      // P(i+n) <-c
      product.uvalue.at(i + that.uvalue.size()) = carry;

  }
  //get rid of leading zeroes
   while (product.uvalue.size() > 0 and product.uvalue.back() == 0){ 
         product.uvalue.pop_back();
   }

   return product;
}

void ubigint::multiply_by_2() {
  
   //no return statement
   unsigned int digit = 0;   //mult of each digit
   unsigned int carry = 0;  //digit to be carried

   for(unsigned int i = 0; i < uvalue.size(); i++){  //just one loop cause one number
      digit = (uvalue.at(i)*2 + carry);
      carry = (digit / 10);
      digit = digit%10; //same code as above basically
     //no return so change values of uvalue

     uvalue.at(i) = digit; //change uvalue to digit
   }

   if(carry == 1){   //if there is a carry then pushback
       uvalue.push_back(carry); 
   }
  
   //check for leading zeroes
   while (uvalue.size() > 0 and uvalue.back() == 0){ 
      uvalue.pop_back();
   }
   //no return
}

void ubigint::divide_by_2() {
   //no return
   unsigned int rem = 0;   //remainder
   unsigned int quo = 0;   //quotient of two and digit

   for(unsigned int i = uvalue.size(); i > 0; i--){  //start at highest order digit (back of vector)

      quo = uvalue.at(i)/2;   //divide by 2
      rem = quo % 2; //remainder will be mod 2

      //if (rem > 0){     //if there is remainder
         quo += rem;    //add to quotient
        // rem = 0;
     // }
      uvalue.at(i) = quo;     //change uvalues to quotient
      
   }
   //check for leading zeroes
   while (uvalue.size() > 0 and uvalue.back() == 0){ 
      uvalue.pop_back();
   }
   //no return
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
   //DEBUGF ('/', "quotient = " << quotient);
  // DEBUGF ('/', "remainder = " << remainder);
   return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
   return udivide (*this, that).quotient; //shouldnt have to change if udivide works right?
}

ubigint ubigint::operator% (const ubigint& that) const {
   return udivide (*this, that).remainder;   //should have to change if udivide works right?
}

bool ubigint::operator== (const ubigint& that) const {
   if(uvalue.size()!=that.uvalue.size()){ //if sizes are different returns false
      return false;
   }
   unsigned int i = 0;
   while(i < uvalue.size()){ //<=?? inf loop check
      if(uvalue.at(i)!=that.uvalue.at(i)){   //if the digits are not the same
         return false;  //returns false
      }
      i++;  //increments 
   }
   return true;   //otherwise returns true
}

bool ubigint::operator< (const ubigint& that) const {
  // return uvalue < that.uvalue;
    if(uvalue.size() < that.uvalue.size()){ //this size is less than that size
      return true;  // than this is less than that, return true
   }
   
   if(uvalue.size() > that.uvalue.size()){ //this size is greater than that size
      return false;  // than this is greater than that, return false
   }
   //otherwise sizes are equal and evaluate digit by digit
   //go from highest to lowest order digit
   int i = uvalue.size();
   while(i > 0){ //<=??  and inf loop check
      if(uvalue.at(i) > that.uvalue.at(i)){   //if this > that then automatically false
         return false;  //returns false
      }
      i--;  //decrements 
   }
   return true; // if all digits of this are less than that, return true
}

ostream& operator<< (ostream& out, const ubigint& that) { //will have errors till implement
  if(that.uvalue.size()==0){
     out << '0';
  }

   else{
      
      for(unsigned int i = 0; i < that.uvalue.size(); i++){ //need to check if vector is bigger than 0?
            out << (that.uvalue.at(i)+'0');
      }
   }
  // return out << "ubigint(" << that.uvalue << ")";
  return out;
}


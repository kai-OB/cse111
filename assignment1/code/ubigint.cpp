// $Id: ubigint.cpp,v 1.12 2020-10-19 13:14:59-07 - - $
//Kai O'brien kimobrie
#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "debug.h"
#include "relops.h"
#include "ubigint.h"

ubigint::ubigint (unsigned long that)/*: uval (that)*/ {
  
   
   while(that > 0){
     
      uvalue.push_back((that % 10) -'0');
      // cout << ((that % 10) -'0'));
      
      that = that/10;
   }

   
}

ubigint::ubigint (const string& that): uval(0) {
 
   for (char digit: that) {
      if (not isdigit (digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }
      uval = uval * 10 + digit - '0';
      uvalue.push_back(digit- '0');
   }

}

ubigint ubigint::operator+ (const ubigint& that) const {
  
   ubigint add_result;
   unsigned int sum; 
   unsigned int carry = 0;  
   ssize_t shorter = that.uvalue.size(); 
   ssize_t longer = uvalue.size();
   bool thisislonger = true;
   if(uvalue.size() < that.uvalue.size()){//sets longer + shorter val
      shorter = uvalue.size();  
      longer = that.uvalue.size();
   } 
      for(unsigned int i = shorter; i > 0; i--){   
         sum = carry;      
         carry = 0;
         
        if(that.uvalue.size() > i ){ 
            sum = that.uvalue.at(i);
         }
         else if(uvalue.size() > i ){ 
            sum = uvalue.at(i);
         }
         
         else { 
            sum = uvalue.at(i)+ that.uvalue.at(i);
         }
         if(sum >9){ // if there is a remainder then:
            carry = 1;  
            sum = (sum % 10);   // and mod 10
         }
         add_result.uvalue.push_back(sum); //push back the sum 
      }

   //pushback for just longer vector
   /* trim the vector by removing all high-order zeros :*/

   if( longer == that.uvalue.size()){
      
   }
  
   while (add_result.uvalue.size()>0 and add_result.uvalue.back()==0){ 
      add_result.uvalue.pop_back();
   }
   return add_result; 

}

ubigint ubigint::operator- (const ubigint& that) const {
   if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
   ubigint sub_result;  //empty vector
   unsigned int sub = 0; //subtraction of two digits
   unsigned int borrow = 0;   //digit to be borrowed

   for(unsigned int i = 0; i<that.uvalue.size(); i++){
      sub = borrow; //subtract borrow 
      borrow = 0;

      sub = uvalue.at(i)-that.uvalue.at(i);
      

      if( uvalue.size() < that.uvalue.size()){
         sub += 10;
         borrow = -1;
      }

      sub_result.uvalue.push_back(sub);
      
      
   }

   while(sub_result.uvalue.size()>0 and sub_result.uvalue.back()==0){ 
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
   while(k < prod_size){//initializes the vector to all zeroes
      product.uvalue.push_back('0' + 0);//new vector size = the 
      k++; //sums of the sizes of the two vectors
   } 

   for(unsigned int i = 0; i < uvalue.size(); i++){  //i E [0,m)
      carry = 0;  // c <-0
      for(unsigned int j = 0; j < that.uvalue.size(); j++){  //j E [0,n)
         // d <- P(i+j) + u(i)v(j) + c
         digit=(product.uvalue.at(i+j)+  
            (uvalue.at(i)*that.uvalue.at(i))+carry);
      
         product.uvalue.at(i+j) = digit % 10; // P(i+j) <- d rem 10

         carry = /*floor*/(digit / 10);   // c <- floor (d / 10)
         
      }
      // P(i+n) <-c
      product.uvalue.at(i+that.uvalue.size()) = carry;

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

   for(unsigned int i = 0; i < uvalue.size(); i++){
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

   for(unsigned int i = uvalue.size(); i > 0; i--){
      quo = uvalue.at(i)/2;   //divide by 2
      rem = quo % 2; //remainder will be mod 2

     
         quo += rem;    //add to quotient
       
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
   return udivide (*this, that).quotient; 
}

ubigint ubigint::operator% (const ubigint& that) const {
   return udivide (*this, that).remainder;  
}

bool ubigint::operator== (const ubigint& that) const {
   if(uvalue.size()!=that.uvalue.size()){ //if size diff, false
      return false;
   }
   unsigned int i = 0;
   while(i < uvalue.size()){ //<=?? inf loop check
      if(uvalue.at(i)!=that.uvalue.at(i)){//if the digits ! same
         return false;  //returns false
      }
      i++;  //increments 
   }
   return true;   //otherwise returns true
}

bool ubigint::operator< (const ubigint& that) const {
 
   if(that.uvalue == uvalue){   //if equal then false
       return false;
    }
   else if(uvalue.size() < that.uvalue.size()){ 
      return true; 
   }
   else if(that.uvalue.size() < uvalue.size()){ 
      return false; 
   }
   //otherwise sizes are equal and evaluate digit by digit
   //go from highest to lowest order digit
   else{
      if(that.uvalue.size() == uvalue.size() && uvalue.size() > 0){
         for(int i = that.uvalue.size()-1; i >0; i--){//inf
               if(uvalue.at(i) < that.uvalue.at(i)){
                  return true;  //returns false
                  break;
               }
            }
      }
   }
   return false; 
}

ostream& operator<< (ostream& out, const ubigint& that) { 
  if(that.uvalue.size()==0){
     out << '0';
 }

  else{
      int linelen = 0;
   for(unsigned int i = 0; i < that.uvalue.size(); i++){ 
      if(linelen == 69){
         out<< "\\" << endl;
         linelen = 0;
      }
      out << static_cast <char> (that.uvalue.at(i) +'0');
      linelen++;
   }
}
  return out;
}


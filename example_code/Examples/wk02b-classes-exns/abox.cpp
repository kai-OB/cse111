// $Id: abox.cpp,v 1.30 2021-04-27 15:50:20-07 - - $

// Class abox showing default ctor, copy ctor, operator=, dtor.

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

using namespace std;

#define LINE "[" << __LINE__ << "] "
#define ABOX \
        cout << LINE << "ABOX{" << this << "->" << __PRETTY_FUNCTION__ \
             << ": value=" << value << "}" << endl; 
#define SHOW(STMT) \
        cout << LINE << "SHOW{" << #STMT << "}" << endl; \
        STMT \
        cout << endl; \

/////////////////////////////////////////////////////////////////
// abox.h
/////////////////////////////////////////////////////////////////

class abox {
   private:
      int value;
   public:
      abox();                          // default ctor
      abox (const abox&);              // copy ctor
      abox& operator= (const abox&);   // copy operator=
      ~abox();                         // dtor
      abox (abox&&);                   // C++11 move ctor
      abox& operator= (abox&&);        // C++11 move operator=
      abox (int);                      // implicit int->abox conv
      operator int() const;            // implicit abox->int conv
      abox& operator++();              // prefix ++
      abox operator++ (int);           // postfix ++
};

/////////////////////////////////////////////////////////////////
// abox.cpp                          // default ctor
/////////////////////////////////////////////////////////////////

abox::abox(): value(0) { // default ctor
   ABOX;
}

abox::abox (const abox& init): value(init.value) { // copy ctor
   ABOX;
}

abox& abox::operator= (const abox& that) { // copy operator=
   if (this != &that) this->value = that.value;
   ABOX;
   return *this;
}

abox::~abox() { // dtor
   ABOX;
}

abox::abox (abox&& init): value(init.value) { // C++11 move ctor
   ABOX;
}

abox& abox::operator= (abox&& that) { // C++11 move operator=
   if (this != &that) this->value = that.value;
   ABOX;
   return *this;
}

abox::abox (int init): value(init) { // implicit int->abox conv
   ABOX;
}

abox::operator int() const { // implicit abox->int conv
   ABOX;
   return value;
}

abox& abox::operator++() { // prefix ++
   ++value;
   return *this;
}

abox abox::operator++ (int) { // postfix ++
   abox result = value;
   ++value;
   return result;
}


/////////////////////////////////////////////////////////////////
// main.cpp
/////////////////////////////////////////////////////////////////

void ref_fn (const abox& that) {
   SHOW (int value = that;)
   SHOW (cout << "ref_fn, value=" << value << endl;)
}

void value_fn (const abox that) {
   SHOW (int value = that;)
   SHOW (cout << "value_fn, value=" << value << endl;)
}

int main() {
   SHOW (abox a;)
   SHOW (abox b = a;)
   SHOW (abox c (a);)
   SHOW (abox d {a};)
   SHOW (abox e = 6;)
   SHOW (b = 3;)
   SHOW (ref_fn (6);)
   SHOW (ref_fn (a);)
   SHOW (value_fn (a);)
   SHOW (abox ii = 255;)
   SHOW (int i = ii;)
   SHOW (cout << i << endl;)
   SHOW (abox *p = new abox (6);)
   SHOW (delete p;)
   SHOW (abox *bb = new abox[3];)
   SHOW (delete[] bb;)
   SHOW (vector<abox> vb;)
   SHOW (vb.push_back (6);)
   SHOW (vb.push_back (8);)
   SHOW (cout << (a = e++) << endl;)
   SHOW (cout << (a = ++e) << endl;)
   SHOW (auto sp = make_shared<abox> (98765);)
   SHOW (return EXIT_SUCCESS;)
}

//TEST// GRIND="valgrind --leak-check=full --show-reachable=yes"
//TEST// $GRIND abox >abox.lis 2>&1
//TEST// mkpspdf abox.ps abox.cpp* abox.lis*


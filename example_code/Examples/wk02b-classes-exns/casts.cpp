// $Id: casts.cpp,v 1.31 2021-04-15 17:51:46-07 - - $

// Show different kinds of casts.

#include <iostream>
using namespace std;

class base {
      string b = "base";
   public:
      virtual ~base() = default;
      virtual const string to_string() const { return b; }
};

class derived: public base {
      string d = "derived";
   public:
      virtual ~derived() = default;
      virtual const string to_string() const {
         return d + " + " + base::to_string(); 
      }
};

int main() {

   char c = 'A';
   int i = static_cast<int> (c);
   cout << "static_cast: " << c << " " << i << endl;

   base* b = new derived();
   derived* d = dynamic_cast<derived*> (b);
   b = d; // Does not need a cast.
   // d = b; // error: invalid conversion from 'base*' to 'derived*'
   cout << "dynamic_cast: " << d->to_string() << endl;
   delete b;

   uintptr_t st = reinterpret_cast<uintptr_t> (b);
   cout << "reinterpret_cast: " << b << " " << st << endl;

   int j = 40;
   const int* p = &j;
   // *p = 99; // error: assignment of read-only location '* p'
   *const_cast<int*> (p) = 100;
   cout << "const_cast: " << j << " " << *p << endl;

   return 0;
}

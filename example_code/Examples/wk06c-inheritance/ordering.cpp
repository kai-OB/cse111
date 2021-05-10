// $Id: ordering.cpp,v 1.7 2020-03-03 15:52:07-08 - - $

//
// Order of construction and destruction.
//

#include <iostream>
#include <string>
using namespace std;

struct name {
   string value;
   name (const string& v): value(v) {
      cout << __PRETTY_FUNCTION__ << ": " << value << endl;
   }
   ~name() {
      cout << __PRETTY_FUNCTION__ << ": " << value << endl;
   }
};

struct base {
   name first {"first"};
   name second {"second"};
   base() {
      cout << __PRETTY_FUNCTION__ << endl;
   }
   virtual ~base() {
      cout << __PRETTY_FUNCTION__ << endl;
   }
};

struct derived: public base {
   name third {"third"};
   name fourth {"fourth"};
   derived() {
      cout << __PRETTY_FUNCTION__ << endl;
   }
   virtual ~derived() {
      cout << __PRETTY_FUNCTION__ << endl;
   }
};

int main() {
   cout << __PRETTY_FUNCTION__ << ": start" << endl;
   derived x;
   cout << __PRETTY_FUNCTION__ << ": finish" << endl;
   return 0;
}

//TEST// ordering >ordering.out 2>&1
//TEST// mkpspdf ordering.ps ordering.cpp* ordering.out


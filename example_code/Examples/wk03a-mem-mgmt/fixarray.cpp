// $Id: fixarray.cpp,v 1.13 2020-10-29 17:16:18-07 - - $

//
// fixarray - implementation of an int vector, using templates,
// with the array inline in the struct, so size can not be changed.
// also shows checked operator[] instead of get/put.
//

#include <iostream>
#include <stdexcept>

using namespace std;

/////////////////////////////////////////////////////////////////
// fixarray.h
/////////////////////////////////////////////////////////////////

template <typename item_t, size_t size_>
class fixarray {
   private:
      item_t data_[size_] {};
   public:
      fixarray() = default;
      fixarray (const fixarray &) = default;            // copy ctor
      fixarray &operator= (const fixarray &) = default; // copy op=
      ~fixarray() = default;
      size_t size() const { return size_; }
      const item_t& operator[] (size_t index) const{ // get: x = a[i];
         return data_[index];
      }
      item_t& operator[] (size_t index){             // put: a[i] = x;
         return data_[index];
      }
};

/////////////////////////////////////////////////////////////////
// main.cpp
/////////////////////////////////////////////////////////////////

using tenvec = fixarray<int,10>;
int main () {
   tenvec v1;
   v1[3] = 99;
   int x = v1[3];
   cout << x << endl;
   tenvec v2 = v1;
   v2[3] = 1234;
   cout << v1[3] << " " << v2[3] << endl;
   v2 = v1;
   cout << v1[3] << " " << v2[3] << endl;
   for (size_t i = 0; i < v1.size(); ++i) cout << v1[i];
   cout << endl;
   return 0;
}

//TEST// alias grind='valgrind --leak-check=full --show-reachable=yes'
//TEST// grind fixarray >fixarray.out 2>&1
//TEST// mkpspdf fixarray.ps fixarray.cpp* fixarray.out*


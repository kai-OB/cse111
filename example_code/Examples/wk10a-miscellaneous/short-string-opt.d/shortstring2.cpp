// $Id: shortstring2.cpp,v 1.7 2016-08-03 13:16:57-07 - - $

// Alternate short string optimization.

#include <iostream>
using namespace std;

struct opt_string {
   char* pointer_;
   size_t size_;
   union {
      char data_[16];
      size_t capacity_;
   };

   bool is_long() const {
      return pointer_ != data_;
   }

   size_t size() const {
      return size_;
   }

   size_t capacity() const {
      return is_long() ? capacity_ : sizeof data_ - 1;
   }

   opt_string(): pointer_(data_), size_(0) {
      data_[0] = '\0';
   }

   ~opt_string() {
      if (is_long() and pointer_) delete[] pointer_;
   }

};

int offset (void* base, void* field) {
   return static_cast<char*> (field) - static_cast<char*> (base);
}

#define SHOW(X) cout << #X << " = " << X << endl;
int main() {
   opt_string u;
   SHOW (sizeof u);
   SHOW (sizeof u.pointer_);
   SHOW (sizeof u.size_);
   SHOW (sizeof u.data_);
   SHOW (sizeof u.capacity_);
   SHOW (offset (&u, &u.pointer_));
   SHOW (offset (&u, &u.size_));
   SHOW (offset (&u, &u.data_));
   SHOW (offset (&u, &u.capacity_));
}

//TEST// ./shortstring2 >shortstring2.out
//TEST// mkpspdf shortstring2.ps shortstring2.cpp* shortstring2.out


// $Id: shortstring.cpp,v 1.25 2016-08-03 12:54:41-07 - - $

// Simple short string optimization.
// Works only on a little-endian machine.

#include <iostream>
using namespace std;

struct opt_string {

   struct long_string {
      size_t size_; // stores size << 1 | 1.
      size_t capacity_;
      char* data_;
   };

   struct short_string {
      unsigned char size_ {0}; // stores size << 1.
      char data_[sizeof (long_string) - 1] {};
   };

   union {
      long_string long_;
      short_string short_;
   };

   bool is_long() const {
      return short_.size_ & 1;
   }

   size_t size() const {
      return is_long() ? short_.size_ >> 1 : long_.size_;
   }

   size_t capacity() const {
      return is_long() ? long_.capacity_ >> 1 : sizeof short_.data_ - 1;
   }

   opt_string() { short_.size_ = short_.data_[0] = 0; }

   ~opt_string() {
      if (is_long() and long_.data_) delete[] long_.data_;
   }

};


int offset (void* base, void* field) {
   return static_cast<char*> (field) - static_cast<char*> (base);
}

#define SHOW(X) cout << #X << " = " << X << endl;
int main() {
   opt_string u;
   SHOW (sizeof u);
   SHOW (sizeof u.long_);
   SHOW (sizeof u.long_.capacity_);
   SHOW (sizeof u.long_.size_);
   SHOW (sizeof u.long_.data_);
   SHOW (sizeof u.short_);
   SHOW (sizeof u.short_.size_);
   SHOW (sizeof u.short_.data_);
   SHOW (offset (&u, &u));
   SHOW (offset (&u, &u.long_));
   SHOW (offset (&u, &u.long_.capacity_));
   SHOW (offset (&u, &u.long_.size_));
   SHOW (offset (&u, &u.long_.data_));
   SHOW (offset (&u, &u.short_));
   SHOW (offset (&u, &u.short_.size_));
   SHOW (offset (&u, &u.short_.data_));
}

//TEST// ./shortstring >shortstring.out
//TEST// mkpspdf shortstring.ps shortstring.cpp* shortstring.out


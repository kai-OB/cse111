// $Id: iterintvec.cpp,v 1.64 2018-07-18 15:21:29-07 - - $

//
// iterintvec - implementation of an int vector with iterator.
//

#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace std;

/////////////////////////////////////////////////////////////////
// iterintvec.h
/////////////////////////////////////////////////////////////////

class iterintvec {
   public:
      using value_type = int;
      using reference = int&;
      using const_reference = const int&;
      using pointer = int*;
      using const_pointer = const int*;
      using difference_type = ptrdiff_t;
      using size_type = size_t;
   private:
      size_type size_ {};
      value_type *data_ {};
   public:
      class iterator {
         private:
            pointer curr;
            friend class iterintvec;
            iterator (pointer init): curr(init) {};
         public:
            iterator(): curr(nullptr) {};
            pointer operator->() { return curr; }
            reference operator*() { return *curr; }
            const_reference operator*() const { return *curr; }
            iterator& operator++() { ++curr; return *this; }
            iterator operator++ (int) {
               iterator tmp {*this};
               ++curr;
               return tmp;
            }
            bool operator== (const iterator that) const {
               return curr == that.curr; 
            }
            bool operator!= (const iterator that) const {
               return not (*this == that); 
            }
            operator bool() { return curr != nullptr; }
      };
      iterator begin() { return iterator (&data_[0]); }
      iterator end() { return iterator (&data_[size_]); }

      iterintvec() = default;                    // default ctor
      iterintvec (const iterintvec&);            // copy ctor
      iterintvec (iterintvec&&);                 // move ctor
      iterintvec& operator= (const iterintvec&); // copy operator=
      iterintvec& operator= (iterintvec&&);      // move operator=
      ~iterintvec();                             // dtor
      explicit iterintvec (size_type size);
      size_type size() const { return size_; }
      reference operator[] (size_type index) { return data_[index]; }
      const_reference operator[] (size_type index) const {
         return data_[index];
      }
};


/////////////////////////////////////////////////////////////////
// iterintvec.cpp
/////////////////////////////////////////////////////////////////

// Copy constructor.
iterintvec::iterintvec (const iterintvec& that):
            size_(that.size_), data_ (new value_type[that.size_]) {
   std::copy (&that.data_[0], &that.data_[that.size_], &data_[0]);
}

// Move constructor.
iterintvec::iterintvec (iterintvec&& that):
            size_(that.size_), data_ (that.data_) {
   that.size_ = 0;
   that.data_ = nullptr;
}

// Copy operator=
iterintvec& iterintvec::operator= (const iterintvec& that){
   if (this == &that) return *this;
   if (data_ != nullptr) delete[] data_;
   size_ = that.size_;
   data_ = new value_type[that.size_];
   std::copy (&that.data_[0], &that.data_[that.size_], &data_[0]);
   return *this;
}

// Move operator=
iterintvec& iterintvec::operator= (iterintvec&& that){
   if (this == &that) return *this;
   if (data_ != nullptr) delete[] data_;
   size_ = that.size_;
   data_ = that.data_;
   that.size_ = 0;
   that.data_ = nullptr;
   return *this;
}

// Destructor.
iterintvec::~iterintvec() {
   if (data_ != nullptr) delete[] data_;
}

// Fixed-size allocator.
iterintvec::iterintvec (size_type size):
               size_(size), data_ (new value_type[size_]) {
   std::fill (&data_[0], &data_[size_], 0);
}


/////////////////////////////////////////////////////////////////
// main.cpp
/////////////////////////////////////////////////////////////////

int main() {
   iterintvec v1(10);
   v1[3] = 99;
   int x = v1[3];
   cout << x << endl;
   cout << "v1[999] = " << v1[999] << endl;
   iterintvec v2 = v1;
   v2[3] = 1234;
   cout << v1[3] << " " << v2[3] << endl;
   v2 = v1;
   cout << v1[3] << " " << v2[3] << endl;
   for (iterintvec::iterator i = v1.begin(); i != v1.end(); ++i) {
      cout << " " << *i;
   }
   cout << endl;
   for (const auto& n: v1) { cout << " " << n; }
   cout << endl;
   for_each (v1.begin(), v1.end(), [](int n){ cout << " " << n; });
   cout << endl;
   return 0;
}

//TEST// alias grind='valgrind --leak-check=full --show-reachable=yes'
//TEST// grind iterintvec >iterintvec.out 2>&1
//TEST// mkpspdf iterintvec.ps iterintvec.cpp* iterintvec.out*


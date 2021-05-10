// $Id: intarray.cpp,v 1.88 2021-04-27 15:53:01-07 - - $

//
// Array of int class with iterator.
//

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

#define FUNCTION \
        cout << setw(4) << __LINE__ << ": " \
             << __PRETTY_FUNCTION__ << endl;

#define ENTER(addr) (addr)->display ("ENTER", #addr);
#define LEAVE(addr) (addr)->display ("LEAVE", #addr);

#define STMT(stmt) \
        cout << setw(4) << __LINE__ << ": STMT (" << #stmt \
             << ");" << endl; \
        stmt; 

class intarray {
   private:
      size_t size_ {};
      int* data_ {};
      void delete_data();
      void display (const char*, const char*) const;
   public:

      class iterator {
         friend class intarray;
         private:
            int* ptr;
            iterator (int* ptr_): ptr(ptr_) {}
         public:
            bool operator!= (iterator that) { return ptr != that.ptr; }
            int& operator*() { return *ptr; }
            iterator& operator++() { ++ptr; return *this; }
      };

      intarray() = delete;                   // default ctor
      intarray (const intarray&);            // copy ctor
      intarray (intarray&&);                 // move ctor
      intarray& operator= (const intarray&); // copy operator=
      intarray& operator= (intarray&&);      // move operator=
      ~intarray();                           // dtor
      intarray (size_t size);                // other ctor

      iterator begin() { return &data_[0]; }
      iterator end() { return &data_[size_]; }
      int& operator[] (size_t index) { return data_[index]; }
      size_t size() const { return size_; }
};

void intarray::delete_data() {
   if (data_ != nullptr) delete[] data_;
   size_ = 0;
   data_ = nullptr;
}

void intarray::display (const char* which, const char* label) const {
   cout << "      " << which << ": " << label << ": "
        << this << " -> {" << size_ << ", ";
   if (data_ == nullptr) cout << "nullptr";
                    else cout << data_;
   cout << "}" << endl;
}

intarray::intarray (const intarray& that):
          size_(that.size_), data_(new int[size_]) {
   FUNCTION;
   ENTER (&that);
   std::copy (that.data_, that.data_ + that.size_, data_);
   LEAVE (this);
   LEAVE (&that);
}

intarray::intarray (intarray&& that):
          size_(that.size_), data_(that.data_) {
   FUNCTION;
   ENTER (&that);
   that.size_ = 0;
   that.data_ = nullptr;
   LEAVE (this);
   LEAVE (&that);
}

intarray& intarray::operator= (const intarray& that) {
   FUNCTION;
   ENTER (this);
   ENTER (&that);
   if (this == &that) return *this;
   assert (that.data_);
   delete_data();
   size_ = that.size_;
   data_ = new int[size_];
   std::copy (that.data_, that.data_ + that.size_, data_);
   LEAVE (this);
   LEAVE (&that);
   return *this;
}

intarray& intarray::operator= (intarray&& that) {
   FUNCTION;
   ENTER (this);
   ENTER (&that);
   if (this != &that) {
      delete_data();
      size_ = that.size_;
      data_ = that.data_;
      that.size_ = 0;
      that.data_ = nullptr;
   }
   LEAVE (this);
   LEAVE (&that);
   return *this;
}

intarray::~intarray() {
   FUNCTION;
   ENTER (this);
   delete_data();
   LEAVE (this);
}

intarray::intarray (size_t size):
          size_(size), data_(new int[size]) {
   FUNCTION;
   std::fill (data_, data_ + size_, 0);
   LEAVE (this);
}

void print (int line, intarray& arr) {
   cout << setw(4) << line << ":";
   for (auto& item: arr) cout << " " << item;
   cout << endl;
}

int main() {
   STMT (intarray first (5););
   for (size_t i = 0; i < first.size(); ++i) first[i] = i * 111;
   cout << setw(4) << __LINE__ << ":";
   for (auto itor = first.begin(); itor != first.end(); ++itor) {
      cout << " " << *itor;
   }
   cout << endl;
   STMT (for (int& i: first) i *= 2;);
   STMT (for (int i: first) cout << " " << i;);
   cout << endl;
   STMT (intarray second (first););
   print (__LINE__, second);
   STMT (intarray third (std::move (first)););
   print (__LINE__, third);
   STMT (first = second;);
   print (__LINE__, first);
   STMT (first = std::move (third););
   print (__LINE__, first);
   STMT (return 0;);
}


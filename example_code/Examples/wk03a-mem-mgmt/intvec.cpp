// $Id: intvec.cpp,v 1.61 2021-01-19 16:54:38-08 - - $

//
// intvec - implementation of an int vector, similar to 
// Stroustrup, ch.17 example.
//

#include <iostream>
#include <stdexcept>

using namespace std;

#define DEBUG(X) { \
        cout << "[" << __LINE__ << "]" << __PRETTY_FUNCTION__ \
             << ": " << endl << "     " << *X << endl; \
        }

/////////////////////////////////////////////////////////////////
// intvec.h
/////////////////////////////////////////////////////////////////

class intvec {
   friend ostream& operator<< (ostream&, const intvec&);
   private:
      size_t size_;
      int *data_;
   public:
      static bool want_debug;
      intvec();                          // default ctor
      intvec (const intvec&);            // copy ctor
      intvec (intvec&&);                 // move ctor
      intvec& operator= (const intvec&); // copy operator=
      intvec& operator= (intvec&&);      // move operator=
      ~intvec();                         // dtor
      // Other members.
      explicit intvec (size_t size);
      size_t size() const { return size_; }
      const int& operator[] (size_t index) const {  // x = v[i];
         return data_[index];
      }
      int& operator[] (size_t index) {              // v[i] = x;
         return data_[index];
      }
};

bool intvec::want_debug = false;


/////////////////////////////////////////////////////////////////
// intvec.cpp
/////////////////////////////////////////////////////////////////

// operator<< intvec
ostream& operator<< (ostream& out, const intvec& vec) {
   out << &vec << "->intvec(" << vec.size_;
   if (vec.data_ == nullptr) out << ",nullptr";
   else {
      cout << ',' << vec.data_ << "->";
      for (size_t index = 0; index != vec.size_; ++index) {
         cout << (index == 0 ? "[" : ",") << vec.data_[index];
      }
      cout << "]";
   }
   out << ")";
   return out;
}

// Default constructor.
intvec::intvec(): size_(0), data_(nullptr) {
   if (want_debug) DEBUG(this);
}

// Copy constructor.
intvec::intvec (const intvec& that):
            size_(that.size_), data_ (new int[that.size_]) {
   std::copy (&that.data_[0], &that.data_[that.size_], this->data_);
   if (want_debug) DEBUG(this);
}

// Move constructor.
intvec::intvec (intvec&& that): size_(that.size_), data_ (that.data_) {
   that.size_ = 0;
   that.data_ = nullptr;
   if (want_debug) DEBUG(this);
}

// Copy operator=
intvec& intvec::operator= (const intvec& that){
   if (this == &that) return *this;
   if (data_ != nullptr) delete[] data_;
   size_ = that.size_;
   data_ = new int[that.size_];
   std::copy (&that.data_[0], &that.data_[that.size_], this->data_);
   if (want_debug) DEBUG(this);
   return *this;
}

// Move operator=
intvec& intvec::operator= (intvec&& that){
   if (this == &that) return *this;
   if (data_ != nullptr) delete[] data_;
   size_ = that.size_;
   data_ = that.data_;
   that.size_ = 0;
   that.data_ = nullptr;
   if (want_debug) DEBUG(this);
   return *this;
}


// Destructor.
intvec::~intvec() {
   if (data_ != nullptr) delete[] data_;
   size_ = 0;
   data_ = nullptr;
   if (want_debug) DEBUG(this);
}

// Fixed-size allocator.
intvec::intvec (size_t size): size_(size), data_ (new int[size_]) {
   std::fill (&data_[0], &data_[size_], 0);
   if (want_debug) DEBUG(this);
}


/////////////////////////////////////////////////////////////////
// main.cpp
/////////////////////////////////////////////////////////////////

int main (int argc, char**) {
   intvec::want_debug = argc > 1;
   intvec v1(10);
   v1[3] = 33;
   v1[5] = 55;
   intvec v2(v1);
   intvec v3;
   v3 = v2;
   cout << "v1 = " << v1 << endl;
   cout << "v2 = " << v2 << endl;
   v2 = std::move(v1);
   cout << "v1 = " << v1 << endl;
   cout << "v2 = " << v2 << endl;
   intvec* pv = new intvec(5);
   delete pv;
   return 0;
}

//TEST// alias grind='valgrind --leak-check=full --show-reachable=yes'
//TEST// grind intvec >intvec.out 2>&1
//TEST// mkpspdf intvec.ps intvec.cpp* intvec.out*


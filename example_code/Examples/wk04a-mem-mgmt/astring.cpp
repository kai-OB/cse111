// $Id: astring.cpp,v 1.82 2019-10-25 18:17:11-07 - - $

//
// NAME
//    astring - trivial implementation of a string using an array
//
// DESCRIPTION
//    We show how to implement a simple string class.
//

#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;

/////////////////////////////////////////////////////////////////
// astring.h
/////////////////////////////////////////////////////////////////

class astring {
   friend ostream& operator<< (ostream&, const astring&);
   private:
      static constexpr size_t DEFAULT_CAPACITY = 16;
      size_t capacity_;
      size_t size_;
      char* cstr_;
   public:

      // override implicit members
      astring();                                // default ctor
      astring (const astring&);                 // copy ctor
      astring& operator= (const astring&);      // operator=
      ~astring();                               // dtor
      astring (astring&&);                      // move ctor
      astring& operator= (astring&&);           // move operator=

      // other members
      astring (const char*);                // "" ctor
      explicit astring (size_t);            // length reservation
      astring& operator= (const char*);     // operator=
      astring& operator+= (const astring&); // += astring&
      astring& operator+= (const char);     // += char
      astring& operator+= (const char*);    // += char* 
      void reserve (size_t);                // ensure buffer size;
      const char& operator[] (size_t pos) const { return cstr_[pos]; }
      char& operator[] (size_t pos) { return cstr_[pos]; }
      size_t size() const { return size_; }
      size_t capacity() const { return capacity_; }
};

astring& operator+ (const astring&, const astring&);
astring& operator+ (const astring&, const char);
astring& operator+ (const astring&, const char*);
astring& operator+ (const char*, const astring&);

/////////////////////////////////////////////////////////////////
// astring.cpp
/////////////////////////////////////////////////////////////////

astring::astring(): capacity_ (DEFAULT_CAPACITY),
                    size_ (0),
                    cstr_ (new char[DEFAULT_CAPACITY]) {
   cstr_[size_] = '\0';
}

astring::astring (const astring& that):
                  capacity_ (that.capacity_),
                  size_ (that.size_),
                  cstr_ (new char[that.capacity_]) {
   strncpy (cstr_, that.cstr_, size_ + 1);
}

astring& astring::operator= (const astring& that) {
   if (this == &that) return *this;
   reserve (that.size_ + 1);
   size_ = that.size_;
   strncpy (cstr_, that.cstr_, size_ + 1);
   return *this;
}

astring::astring (astring&& that) {
   *this = that;
}

astring& astring::operator= (astring&& that) {
   if (this == &that) return *this;
   capacity_ = that.capacity_;
   size_ = that.size_;
   cstr_ = that.cstr_;
   that.size_ = that.capacity_ = 0;
   that.cstr_ = nullptr;
   return *this;
}

astring::~astring() {
   if (cstr_ != nullptr) delete[] cstr_;
}


astring::astring (const char* that) {
   size_ = strlen (that);
   capacity_ = max (DEFAULT_CAPACITY, size_ + 1);
   cstr_ = new char [capacity_];
   strncpy (cstr_, that, size_ + 1);
}

astring::astring (size_t capacity): capacity_ (capacity), size_ (0),
                    cstr_ (new char[size_]) {
   cstr_[size_] = '\0';
}

astring& astring::operator= (const char* that) {
   size_ = strlen (that);
   reserve (size_ + 1);
   strncpy (cstr_, that, size_ + 1);
   return *this;
}

astring& astring::operator+= (const astring& that) {
   return *this += that.cstr_;
}

astring& astring::operator+= (const char achar) {
   ++size_;
   reserve (size_ + 1);
   cstr_[size_ - 1] = achar;
   cstr_[size_] = '\0';
   return *this;
}

astring& astring::operator+= (const char* that) {
   size_t prev_size = size_;
   size_t that_size = strlen (that);
   size_ += that_size;
   reserve (size_ + 1);
   strncpy (&cstr_[prev_size], that, that_size + 1);
   return *this;
}

void astring::reserve (size_t capacity) {
   if (capacity < capacity_) return;
   capacity_ = max ({DEFAULT_CAPACITY, capacity, capacity_ * 2});
   char* old_cstr = cstr_;
   cstr_ = new char[capacity_];
   strncpy (cstr_, old_cstr, size_ + 1);
   delete[] old_cstr;
}


ostream& operator<< (ostream& out, const astring& that) {
   out << that.cstr_;
   return out;
}

astring& operator+ (const astring& left, const astring& right) {
   astring result (left);
   return result += right;
}

astring& operator+ (const astring& left, const char right) {
   astring result (left);
   return result += right;
}

astring& operator+ (const astring& left, const char* right) {
   astring result (left);
   return result += right;
}

astring& operator+ (const char* left, const astring& right) {
   astring result (left);
   return result += right;
}


/////////////////////////////////////////////////////////////////
// main.cpp
/////////////////////////////////////////////////////////////////

void print (const astring& str) {
   cout << str.size() << " " << str.capacity()
        << " \"" << str << "\"" << endl;
}

int main (int argc, char** argv) {
   astring first = "Hello, World!";
   cout << "first=" << first << endl;
   astring second;
   second = first;
   second += 'x'; second += 'y';
   for (int i = 0; i < 3; ++i) second[i] = i + '1';
   cout << "second=" << second << endl;
   for (size_t i = 5; i < second.size(); ++i) {
      cout << second[i] << endl;
   }
   astring allargs = "argv:";
   for (char** arg = &argv[0]; arg != &argv[argc]; ++arg) {
      print (allargs);
      ((allargs += " [") += *arg) += ']';
   }
   print (allargs);
   return EXIT_SUCCESS;
}

/*
//TEST// valgrind --leak-check=full --show-reachable=yes \
//TEST//          astring foo bar baz qux hello world \
//TEST//          >astring.out 2>&1
//TEST// mkpspdf astring.ps astring.cpp* astring.out*
*/


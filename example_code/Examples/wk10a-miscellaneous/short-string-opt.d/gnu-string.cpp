// $Id: gnu-string.cpp,v 1.21 2016-08-03 16:23:58-07 - - $

// G++ basic_string for gcc 4.*.
// Documentation taken from source code.
// Code cleaned up a little.

#include <cstring>
#include <iostream>
using namespace std;

template <typename char_type>
struct basic_gnustr {
   struct repr_ {
      size_t size_;
      size_t capacity_;
      size_t refcount_;
   }; // NOTE:  not a field.
   static constexpr size_t repr_size = sizeof (repr_);
   static constexpr size_t char_size = sizeof (char_type);
   static constexpr size_t repr_chars = repr_size / char_size;
   char_type* pointer_;

   repr_* repr() {
      repr_* repr_addr = reinterpret_cast<repr_*>(pointer_);
      return &repr_addr[-1];
   }

   size_t size() { return repr()->size_; }
   size_t capacity() { return repr()->capacity_; }
   char_type& operator[] (size_t index) { return pointer_[index]; }
   const char_type* c_str() { return pointer_; }

   basic_gnustr(): pointer_(nullptr) {}
   ~basic_gnustr() { if (pointer_) delete[] repr(); }

   basic_gnustr (size_t size) {
      pointer_ = new char_type[repr_chars + size + 1] + repr_chars;
      repr()->size_ = size;
      repr()->capacity_ = size + 1;
      repr()->refcount_ = 1;
      pointer_[0] = 0;
   }

   basic_gnustr (const char_type* str): basic_gnustr (
      [](const char_type* begin) {
         const char_type* end = begin;
         while(*end++) continue;
         return end - begin;
      }(str)
   ) {
      memcpy (pointer_, str, sizeof (char_type) * size());
      pointer_[size()] = 0;
   }

};


using gnu_string = basic_gnustr<char>;

#define SHOW(X) cout << #X << " = " << X << endl;
int main() {
   gnu_string s ("Hello");
   SHOW (s.repr());
   SHOW (static_cast<void*>(s.pointer_));
   SHOW (s.repr()->size_);
   SHOW (s.repr()->capacity_);
   SHOW (s.repr()->refcount_);
   SHOW (s.pointer_);
   SHOW (s.c_str());
}

//TEST// valgrind gnu-string >gnu-string.out 2>gnu-string.err
//TEST// more gnu-string.out gnu-string.err >gnu-string.lis </dev/null
//TEST// rm gnu-string.out gnu-string.err
//TEST// mkpspdf gnu-string.ps gnu-string.cpp* gnu-string.lis

/**
* @class basic_string basic_string.h <string>
* @brief  Managing sequences of characters and character-like objects.
*
* @ingroup strings
* @ingroup sequences
*
* @tparam _CharT  Type of character
* @tparam _Traits  Traits for character type, defaults to
*                  char_traits<_CharT>.
* @tparam _Alloc  Allocator type, defaults to allocator<_CharT>.
*
* Meets the requirements of a <a href="tables.html#65">container</a>, a
* <a href="tables.html#66">reversible container</a>, and a
* <a href="tables.html#67">sequence</a>.  Of the
* <a href="tables.html#68">optional sequence requirements</a>, only
* @c push_back, @c at, and @c %array access are supported.
*
* @doctodo
*
*
* Documentation?  What's that?
* Nathan Myers <ncm@cantrip.org>.
*

* A string looks like this:
*
* @code
*                                       [_Rep]
*                                       _M_length
*  [basic_string<char_type>]            _M_capacity
*  _M_dataplus                          _M_refcount
*  _M_p ---------------->               unnamed array of char_type
* @endcode
*
* Where the _M_p points to the first character in the string, and
* you cast it to a pointer-to-_Rep and subtract 1 to get a
* pointer to the header.
*
* This approach has the enormous advantage that a string object
* requires only one allocation.  All the ugliness is confined
* within a single %pair of inline functions, which each compile to
* a single @a add instruction: _Rep::_M_data(), and
* string::_M_rep(); and the allocation function which gets a
* block of raw bytes and with room enough and constructs a _Rep
* object at the front.
*
* The reason you want _M_data pointing to the character %array and
* not the _Rep is so that the debugger can see the string
* contents. (Probably we should add a non-inline member to get
* the _Rep for the debugger to use, so users can check the actual
* string length.)
*
* Note that the _Rep object is a POD so that you can have a
* static <em>empty string</em> _Rep object already @a constructed before
* static constructors have run.  The reference-count encoding is
* chosen so that a 0 indicates one reference, so you never try to
* destroy the empty-string _Rep object.
*
* All but the last paragraph is considered pretty conventional
* for a C++ string implementation.
*/

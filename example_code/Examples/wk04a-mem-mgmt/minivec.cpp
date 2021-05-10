// $Id: minivec.cpp,v 1.18 2020-11-03 18:11:42-08 - - $

//
// Simple minimal implementation of a vector.
// Uses raw pointers and storage.
//

#include <iostream>
using namespace std;

// begin points at the vector
// end points at the first free slot
// limit points off the end of the allocation
// end - begin is the number of elements used
// limit - end is the number of free slots
// limit - begin is the total number of slots allocated

/////////////////////////////////////////////////////////////////
// minivec.h
/////////////////////////////////////////////////////////////////

template <typename item>
class minivec {
   private:
      constexpr static size_t MIN_SIZE = 4;
      item* begin_ = nullptr;
      item* end_ = nullptr;
      item* limit_ = nullptr;
   public:
      minivec() = default;
      ~minivec() { if (begin_) delete[] begin_; }
      minivec (const minivec&);
      minivec& operator= (const minivec&);
      minivec (minivec&&);
      minivec& operator= (minivec&&);
      explicit minivec (size_t);
      void reserve (size_t);
      size_t size() { return end_ - begin_; }
      size_t capacity() { return limit_ - begin_; }
      class iterator;
      iterator begin() { return iterator (begin_); }
      iterator end() { return iterator (end_); }
      item& operator[] (size_t pos) { return begin_[pos]; }
      const item& operator[] (size_t pos) const { return begin_[pos]; }
      void push_back (const item&);
      void pop_back();
      item& back();
      const item& back() const;
};

template <typename item>
class minivec<item>::iterator {
   friend class minivec<item>;
   private:
      item* pos_ = nullptr;
      iterator (item* pos): pos_(pos) {}
   public:
      iterator() = default;
      // copiers, movers, dtor all OK.
      item& operator*() { return *pos_; }
      item* operator->() { return pos_; }
      iterator& operator++() { ++pos_; return *this; }
      iterator& operator--() { --pos_; return *this; }
      iterator operator++(int) {auto tmp = *this; ++*this; return tmp; }
      iterator operator--(int) {auto tmp = *this; --*this; return tmp; }
      bool operator!= (const iterator& that) const {
         return pos_ != that.pos_;
      }
      bool operator== (const iterator& that) const {
         return pos_ == that.pos_;
      }
};

/////////////////////////////////////////////////////////////////
// minivec.tcc
/////////////////////////////////////////////////////////////////

template <typename item>
minivec<item>::minivec (size_t size) {
   end_ = begin_ = new item[size];
   limit_ = &begin[size];
}

template <typename item>
void minivec<item>::reserve (size_t new_size) {
   if (new_size < size()) return;
   item* new_begin = new item[new_size];
   size_t old_size = size();
   limit_ = new_begin + new_size;
   std::move (begin_, end_, new_begin);
   if (begin_) delete[] begin_;
   begin_ = new_begin;
   end_ = begin_ + old_size;
}

template <typename item>
void minivec<item>::push_back (const item& it) {
   if (end_ == limit_) reserve (max (MIN_SIZE, size() * 2));
   *end_++ = it;
}

/////////////////////////////////////////////////////////////////
// main.cpp
/////////////////////////////////////////////////////////////////

#include <string>
using namespace std;

int main (int argc, char** argv) {
   minivec<string> vec;
   for (char** argp = &argv[1]; argp != &argv[argc]; ++argp) {
      vec.push_back (*argp);
   }
   cout << endl << "Colon for loop:" << endl;
   for (const auto& item: vec) {
      cout << " " << item;
   }
   cout << endl;
   cout << endl << "Explicit for loop:" << endl;
   for (auto itor = vec.begin(); itor != vec.end(); ++itor) {
      cout << " " << *itor;
   }
   cout << endl << endl;
}

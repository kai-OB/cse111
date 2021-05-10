// $Id: smart_ptr.cpp,v 1.7 2019-04-29 17:03:27-07 - - $

//
// Smart pointer class encapsulating a pointer, like shared_ptr.
//

#include <iostream>
using namespace std;

/////////////////////////////////////////////////////////////////
// pointer.h
/////////////////////////////////////////////////////////////////

template <typename Type>
class pointer {
      template <typename U>
      friend ostream& operator<< (ostream& , const pointer<U>& );
      template <typename U, typename... Args>
      friend pointer<U> make_pointer (Args&&...);

   private:

      // Invariant: ref_count == obj_ptr == nullptr
      //            ref_count->count on heap, obj_ptr->object itself

      size_t* ref_count {};
      Type* obj_ptr {};

      // Auxiliary functions.

      void increment() {
         if (ref_count != nullptr) ++*ref_count;
      }

      void decrement() {
         if (ref_count != nullptr and --*ref_count == 0) {
            delete ref_count;
            delete obj_ptr;
            ref_count = nullptr;
            obj_ptr = nullptr;
         }
      }

      void copy_that (const pointer& that) {
         decrement();
         ref_count = that.ref_count;
         obj_ptr = that.obj_ptr;
      }


   public:
      // Replace implicitly generated functions.

      pointer(): ref_count (0), obj_ptr (nullptr) {
      }

      pointer (const pointer& that): ref_count (that.ref_count),
                                     obj_ptr (that.obj_ptr) {
         increment();
      }

      pointer (pointer&& that): ref_count (that.ref_count),
                                obj_ptr (that.obj_ptr) {
         that.decrement();
      }

      pointer& operator= (const pointer& that) {
         if (this == &that) return *this;
         copy_that (that);
         increment();
         return *this;
      }

      pointer& operator= (pointer&& that) {
         if (this == &that) return *this;
         copy_that (that);
         that.decrement();
         return *this;
      }

      ~pointer() {
         decrement();
      }

      // Other constructors.

      pointer (nullptr_t) {
      }

      pointer (const Type& obj): ref_count (new size_t (1)),
                                 obj_ptr (new Type (obj)) {
      }

      // Mutators (non-const functions).

      Type& operator*() { return *obj_ptr; }

      Type* operator->() { return obj_ptr; }

      pointer& operator= (nullptr_t) {
         decrement();
      }

      // Accessors (const functions).

      const Type& operator*() const { return *obj_ptr; }

      const Type* operator->() const { return obj_ptr; }

      operator bool() const { return obj_ptr; }

      bool operator== (const pointer& that) const {
         return obj_ptr == that.obj_ptr;
      }

      bool operator== (nullptr_t) const { return obj_ptr == nullptr; }
      bool operator!= (nullptr_t) const { return obj_ptr != nullptr; }

      template <typename U>
      friend bool operator== (nullptr_t, const pointer<U>& that) {
         return nullptr == that.obj_ptr;
      }

      template <typename U>
      friend bool operator!= (nullptr_t, const pointer<U>& that) {
         return nullptr != that.obj_ptr;
      }

      bool operator!= (const pointer& that) const {
         return not (*this == that);
      }

      size_t users() const { return ref_count ? *ref_count : 0; }

      size_t unique() const { return users() == 1; }

      size_t empty() const { return users() == 0; }
};

template <typename Type>
ostream& operator<< (ostream& out, const pointer<Type>& that) {
   if (that == nullptr) {
      out << "nullptr";
   }else{
      out << that.obj_ptr << "[" << *that.ref_count << "]";
   }
   return out;
}

template <typename Type, typename... Args>
inline pointer<Type> make_pointer (Args&&... args) {
   pointer<Type> result;
   result.ref_count = new size_t (1);
   result.obj_ptr = new Type (std::forward<Args> (args)...);
   return result;
}


/////////////////////////////////////////////////////////////////
// Main program.
/////////////////////////////////////////////////////////////////

struct node {
   string str;
   pointer<node> link;
   node (const char* s, pointer<node> p = nullptr): str (s), link (p) {
   }
};

int main (int argc, char** argv) {
   pointer<node> head;
   pointer<node> tail;
   for (int i = 0; i < argc; ++i) {
      pointer<node> tmp = make_pointer<node> (argv[i]);
      if (head == nullptr) head = tmp;
                      else tail->link = tmp;
      tail = tmp;
   }
   for (pointer<node> p = head; p != nullptr; p = p->link) {
      cout << p << "-> {\"" << p->str << "\", " << p->link
           << "}" << endl;
   }
   return 0;
}

//TEST// alias grind='valgrind --leak-check=full --show-reachable=yes'
//TEST// grind smart_ptr hello world foo bar baz >smart_ptr.out 2>&1
//TEST// mkpspdf smart_ptr.ps smart_ptr.cpp* smart_ptr.out


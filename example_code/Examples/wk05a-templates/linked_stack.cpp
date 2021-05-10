// $Id: linked_stack.cpp,v 1.6 2020-11-03 12:00:21-08 - - $

//
// linked_stack - show the linked list implementation of a stackk
//
// Deleting pointers in nodes is probably a bad idea here, since
// that will prevent the stack from sharing pointee objects with
// other data structures.
//

#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////
// deleter classes
/////////////////////////////////////////////////////////////////

template <typename Type>
struct deleter {
   void operator() (const Type& p) {
      cout << "deleter(" << &p << ")" << endl;
   }
};

template <typename Type>
struct ptr_deleter {
   void operator() (const Type& p) {
      cout << "ptr_deleter(" << &p << ")" << endl;
      delete p;
   }
};

template <typename Type>
struct array_ptr_deleter {
   void operator() (const Type& p) {
      cout << "array_ptr_deleter(" << &p << ")" << endl;
      delete[] p;
   }
};


/////////////////////////////////////////////////////////////////
// linked_stack.h
/////////////////////////////////////////////////////////////////

template <typename Type, class Deleter = deleter<Type>>
class linked_stack {
   private:
      struct node {
         Type item;
         node* link;
         node (Type item_, node* link_): item(item_), link(link_) {}
      };
      node* top_ = nullptr;
      size_t size_ = 0;
      void copy_from (const linked_stack&);
   public:
      linked_stack(): top_(nullptr), size_(0) {}
      ~linked_stack();
      linked_stack (const linked_stack&);
      linked_stack& operator= (const linked_stack&);
      void push (const Type&);
      void pop();
      Type& top() { return top_->item; }
      const Type& top() const { return top_->item; }
      size_t size() const { return size_;}
      bool empty() const { return size_ == 0;}
};

/////////////////////////////////////////////////////////////////
// linked_stack.cpp
/////////////////////////////////////////////////////////////////

template <typename Type, class Deleter>
void linked_stack<Type,Deleter>::copy_from (const linked_stack& that) {
   node** thisptr = &top_;
   for (node* ptr = that.top_; ptr != nullptr; ptr = ptr->link) {
      *thisptr = new node (ptr->item, nullptr);
      thisptr = &((*thisptr)->link);
   }
   size_ = that.size_;
}

template <typename Type, class Deleter>
linked_stack<Type,Deleter>::~linked_stack() {
   while (not empty()) pop();
}

template <typename Type, class Deleter>
linked_stack<Type,Deleter>::linked_stack (const linked_stack& that) {
   copy_from (that);
}

template <typename Type, class Deleter>
linked_stack<Type,Deleter>&
linked_stack<Type,Deleter>::operator= (const linked_stack& that) {
   if (this == &that) return *this;
   while (not empty()) pop();
   copy_from (that);
   return *this;
}

template <typename Type, class Deleter>
void linked_stack<Type,Deleter>::push (const Type& item) {
   top_ = new node (item, top_);
   ++size_;
}

template <typename Type, class Deleter>
void linked_stack<Type,Deleter>::pop() {
   node* tmp = top_;
   top_ = top_->link;
   Deleter() (tmp->item);
   delete tmp;
   --size_;
}


/////////////////////////////////////////////////////////////////
// main.cpp
/////////////////////////////////////////////////////////////////

int main (int argc, char** argv) {
   vector<string> args (&argv[1], &argv[argc]);

   // First, with stack<string>:
   cout << "First:";
   linked_stack<string> stkstr;
   for (string arg: args) {
      cout << " " << arg;
      stkstr.push (arg);
   }
   cout << endl;
   while (stkstr.size() > size_t (argc / 2)) {
      cout << "popping: " << stkstr.top() << endl;
      stkstr.pop();
   }

   // Second, with stack<string*>:
   cout << endl << "Second:";
   linked_stack<string*, ptr_deleter<string*>> stkpstr;
   for (string arg: args) {
      string* str = new string (arg);
      cout << " " << *str;
      stkpstr.push (str);
   }
   cout << endl;
   while (stkpstr.size() > size_t (argc / 2)) {
      string* top = stkpstr.top();
      cout << "popping: " << top << "->" << *top << endl;
      stkpstr.pop();
   }

   // Finally, with stack<char[]>
   cout << endl << "Third:";
   linked_stack<char*, array_ptr_deleter<char*>> argvstk;
   for (char** argi = &argv[1]; argi != &argv[argc]; ++argi) {
      size_t len = strlen (*argi) + 1;
      char* str = new char[len];
      snprintf (str, len, "%s", *argi);
      cout << " " << str;
      argvstk.push (str);
   }
   cout << endl;

   return 0;
}

/*
//TEST// alias grind='valgrind --leak-check=full --show-reachable=yes'
//TEST// grind linked_stack this is some test data for the stack \
//TEST//       >linked_stack.out 2>&1
//TEST// mkpspdf linked_stack.ps linked_stack.cpp* linked_stack.out*
*/

